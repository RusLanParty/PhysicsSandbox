#include "MyCircle.h";


MyCircle::MyCircle()
{
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(getRandomPos());
}

MyCircle::MyCircle(sf::Vector2f& pos)
{
	this->setRadius(getRandomRad());
	this->initMass();
	this->randomizeSpeed();
	this->randomizeColor();
	this->setOrigin(this->getRadius(), this->getRadius());
	this->setPosition(pos);
}

MyCircle::~MyCircle()
{
	delete this;
}


sf::Vector2f MyCircle::getSpeed() const
{
	return sf::Vector2f(this->_velocity.x, this->_velocity.y);
}

void MyCircle::setSpeed(float xs, float ys)
{
	this->_velocity.x = xs;
	this->_velocity.y = ys;
}

void MyCircle::invertXSpeed()
{
	this->_velocity.x *= -1.0f;
}
void MyCircle::invertYSpeed()
{
	this->_velocity.y *= -1.0f;
}

bool MyCircle::contains(sf::Vector2f &p) const
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
	std::uniform_int_distribution<int> dis(0, 255);
	sf::Color randCol(dis(gen), dis(gen), dis(gen));
	this->setFillColor(randCol);
}

void MyCircle::randomizeSpeed()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.2f, 0.2f);
	float velX = dis(gen);
	float velY = dis(gen);
	while (velX == 0.0f)
	{
		velX = dis(gen);
	}
	while (velY == 0.0f) 
	{
		velY = dis(gen);
	}
	this->_velocity.x = velX;
	this->_velocity.y = velY;
}
void MyCircle::updateMovement()
{
	sf::VideoMode res = sf::VideoMode::getDesktopMode();
	int width = res.width;
	int height = res.height;

	if (this->getPosition().x >= width - this->getRadius() || this->getPosition().x <= this->getRadius())
	{
		this->invertXSpeed();
		this->randomizeColor();
	}
	if (this->getPosition().y >= height - this->getRadius() || this->getPosition().y <= this->getRadius())
	{
		this->invertYSpeed();
		this->randomizeColor();
	}
	float newX = std::clamp(this->getPosition().x + _velocity.x, this->getRadius(), width - this->getRadius());
	float newY = std::clamp(this->getPosition().y + _velocity.y, this->getRadius(), height - this->getRadius());
	this->setPosition(newX, newY);
}

bool MyCircle::isIntersect(const sf::CircleShape& c2) const
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

void MyCircle::calculateCollision(MyCircle& c2)
{
	sf::Vector2f newVel1 = c2.getSpeed();
	sf::Vector2f newVel2 = this->getSpeed();
	this->_velocity = newVel1;
	c2._velocity = newVel2;
}

float MyCircle::getMass() const
{
	return this->mass;
}

sf::Vector2f MyCircle::getRandomPos() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDis(this->getRadius(), sf::VideoMode::getDesktopMode().width - 2*(this->getRadius()));
	std::uniform_real_distribution<float> yDis(this->getRadius(), sf::VideoMode::getDesktopMode().height - 2*(this->getRadius()));
	return sf::Vector2f(xDis(gen), yDis(gen));
}

float MyCircle::getRandomRad() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(30.0f, 100.0f);
	return dis(gen);
}

float MyCircle::getLimitedRad(float limit) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(30.0f, 100.0f);
	return dis(gen);
}

void MyCircle::initMass()
{
	this->mass = this->getRadius() * 0.1f;
}



	
	
