#include "MyCircle.h";
#include "Global.h";
MyCircle::MyCircle() :
	_oldPos(),
	_velocity(),
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f)
{
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(getRandomPos());
}

MyCircle::MyCircle(sf::Vector2f& pos):
	_oldPos(),
	_velocity(),
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f)
{
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(pos);
}

MyCircle::MyCircle(float w, float h, float xs, float ys, float rad):
	_oldPos(),
	_velocity(),
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f)
{
	this->setRadius(rad);
	this->setVelocity(xs, ys);
	this->initMass();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->randomizeColor();
	this->setPosition(w, h);
	std::cout << "CONSTRUCTOR" << "\n";
}

MyCircle::~MyCircle()
{

}


sf::Vector2f MyCircle::getVelocity() const
{
	return sf::Vector2f(this->_velocity.x, this->_velocity.y);
}

void MyCircle::setVelocity(float xs, float ys)
{
	this->_velocity.x = xs;
	this->_velocity.y = ys;
}
void MyCircle::setVelocity(sf::Vector2f &vel)
{
	this->_velocity = vel;
	
}

void MyCircle::invertXSpeed()
{
	this->_velocity.x *= -1.0f;
}
void MyCircle::invertYSpeed()
{
	this->_velocity.y *= -1.0f;
}

void MyCircle::decreaseSpeed()
{
	float dx = 0.0f;
	float dy = 0.0f;
	if (this->getVelocity().x > 0.01f) 
	{
		dx = 0.006f;
	}
	if (this->getVelocity().y > 0.01f)
	{
		dy = 0.006f;
	}
	if (this->getVelocity().x < -0.01f)
	{
		dx = -0.006f;
	}
	if (this->getVelocity().y < -0.01f)
	{
		dy = -0.006f;
	}
	this->setVelocity(this->getVelocity().x - dx, this->getVelocity().y - dy);
}

bool MyCircle::contains(sf::Vector2f& p) const
{
	float dx = std::pow((this->getPosition().x - p.x), 2);
	float dy = std::pow((this->getPosition().y - p.y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist <= this->getRadius())
	{
		return true;
	}
	return false;
}

void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	this->_hue = dis(gen);
	this->_value = 0.15f;
	sf::Color randCol = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	this->setFillColor(randCol);
}

void MyCircle::updateColor()
{
	this->_value += 0.04f;
	this->_hue += 0.1f;

	if (this->_hue >= 360.0f)
	{
		this->_hue -= 360.0f;
	}
	if (this->_value >= 1.0f)
	{
		this->_value = 1.0f;
	}
	sf::Color newColor = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	this->setFillColor(newColor);	
}


void MyCircle::randomizeSpeed()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-10.34f, 12.02f);
	float velX = dis(gen);
	float velY = dis(gen);
	this->_velocity.x = velX;
	this->_velocity.y = velY;
}
void MyCircle::applyGravity() 
{
	this->_velocity.y += 10.0f;
}
void MyCircle::updateMovement(float deltaTime)
{
	//applyGravity();
	if (_oldPos == sf::Vector2f(0.0f, 0.0f))
	{
		sf::Vector2f newPos = this->getPosition() + _velocity;
		_oldPos = this->getPosition();
		setPosition(newPos);
	}
	else
	{
		sf::Vector2f newPos = 2.0f * this->getPosition() - _oldPos + this->_velocity * (deltaTime * deltaTime);
		_oldPos = this->getPosition();
		setPosition(newPos);
	}
	bbox();
}

bool MyCircle::isIntersect(const MyCircle& c2) const
{
	float dx = std::pow((c2.getPosition().x - this->getPosition().x), 2);
	float dy = std::pow((c2.getPosition().y - this->getPosition().y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist < this->getRadius() + c2.getRadius())
	{
		return true;
	}
	else { return false; }
}

void MyCircle::resolveIntersections(MyCircle& other)
{
	sf::Vector2f delta = other.getPosition() - this->getPosition();
	float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

	if (distance < this->getRadius() + other.getRadius())
	{
		float overlap = (this->getRadius() + other.getRadius()) - distance;
		sf::Vector2f normal = delta / distance;

		sf::Vector2f newVelocity1 = this->getVelocity() + normal * overlap * 0.01f;
		sf::Vector2f newVelocity2 = other.getVelocity() - normal * overlap * 0.01f;

		this->setVelocity(newVelocity1.x, newVelocity1.y);
		other.setVelocity(newVelocity2.x, newVelocity2.y);

		this->setPosition(this->getPosition() - normal * overlap * 0.1f);
		other.setPosition(other.getPosition() + normal * overlap * 0.1f);
		
		this->calculateCollision(other);
	}
}

void MyCircle::calculateCollision(MyCircle& c2)
{
	sf::Vector2f newVel1 = c2.getVelocity();
	sf::Vector2f newVel2 = this->getVelocity();
	sf::Vector2f tempPos(this->_oldPos);
	//this->_oldPos = c2._oldPos;
	//c2._oldPos = tempPos;
	this->_velocity = newVel1;
	c2._velocity = newVel2;	
	c2.randomizeColor();
	this->randomizeColor();
	
}

sf::Vector2f MyCircle::getAcceleration() const
{
	return sf::Vector2f();
}

float MyCircle::getMass() const
{
	return this->mass;
}

sf::Vector2f MyCircle::getRandomPos() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDis(this->getRadius(), sf::VideoMode::getDesktopMode().width - 2 * (this->getRadius()));
	std::uniform_real_distribution<float> yDis(this->getRadius(), sf::VideoMode::getDesktopMode().height - 2 * (this->getRadius()));
	return sf::Vector2f(xDis(gen), yDis(gen));
}

float MyCircle::getRandomRad() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(30.0f, 50.0f);
	return dis(gen);
}


void MyCircle::initMass()
{
	this->mass = this->getRadius() * 0.1f;
}

sf::Color MyCircle::HSVtoRGB(float h, float s, float v)
{
	int hi = static_cast<int>(h / 60) % 6;
	float f = h / 60 - std::floor(h / 60);
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	switch (hi) {
	case 0: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255));
	case 1: return sf::Color(static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255));
	case 2: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(t * 255));
	case 3: return sf::Color(static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255), static_cast<sf::Uint8>(v * 255));
	case 4: return sf::Color(static_cast<sf::Uint8>(t * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(v * 255));
	default: return sf::Color(static_cast<sf::Uint8>(v * 255), static_cast<sf::Uint8>(p * 255), static_cast<sf::Uint8>(q * 255));
	}
}

void MyCircle::MyCircle::bbox()
{
	sf::VideoMode res = sf::VideoMode::getDesktopMode();
	int width = res.width;
	int height = res.height;
	if (this->getPosition().x >= width - this->getRadius())
	{
		_oldPos.x = this->getPosition().x;
		this->invertXSpeed();
		this->randomizeColor();
	}
	else
	if (this->getPosition().x <= this->getRadius()) 
	{
		_oldPos.x = this->getPosition().x;
		this->invertXSpeed();
		this->randomizeColor();
	}
	else
	if (this->getPosition().y >= height - this->getRadius())
	{
		_oldPos.y = this->getPosition().y;
		this->invertYSpeed();
		this->randomizeColor();
	}
	if (this->getPosition().y <= this->getRadius()) 
	{
		_oldPos.y = this->getPosition().y;
		this->invertYSpeed();
		this->randomizeColor();
	}
}

