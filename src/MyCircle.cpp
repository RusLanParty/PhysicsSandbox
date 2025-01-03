#include "MyCircle.h"
#include "GameManager.h"
static float SEQ_HUE = 0.0f;
MyCircle::MyCircle():
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_acceleration(),
	_velocity()
{	
	this->_hue = SEQ_HUE;
	SEQ_HUE = SEQ_HUE + 0.01f;
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	sf::Vector2f curPos(this->getRandomPos());
	this->setPositionFromMetersToPixels(curPos);
	this->initMass();
	this->_circle->setPointCount(20);
}

MyCircle::MyCircle(sf::Vector2f& pos):
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_acceleration(),
	_velocity()
{	
	this->_hue = SEQ_HUE;
	SEQ_HUE = SEQ_HUE + 0.01f;
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	sf::Vector2f curPos(pos / Settings::getConversionFactor());
	this->setPositionFromMetersToPixels(curPos);
	this->initMass();
	this->_circle->setPointCount(20);
	//this->randomizeVelocity();
	//std::cout << "Mouse METERS X= " << (float)this->getPositionInMetersFromPixels()->x << " Mouse METERS Y= " << (float)this->getPositionInMetersFromPixels()->y << " Mouse PIXELS X= " << (float)this->_circle->getPosition().x << " Mouse PIXELS Y= " << (float)this->_circle->getPosition().y << "\n";
}

MyCircle::~MyCircle()
{

}


sf::Vector2f MyCircle::getAcceleration() const
{
	sf::Vector2f acc(_acceleration.x, _acceleration.y);
	return acc;
}

sf::Vector2f MyCircle::getPositionInMetersFromPixels() const
{
	sf::Vector2f pos(_circle->getPosition() / Settings::getConversionFactor());
	return pos;
}

void MyCircle::setPositionFromMetersToPixels(sf::Vector2f pos)
{
	this->_circle->setPosition(pos * Settings::getConversionFactor());
}
void MyCircle::setPositionFromMetersToPixels(float x, float y)
{
	this->_circle->setPosition(x * Settings::getConversionFactor(), y * Settings::getConversionFactor());
}


const float MyCircle::getRadiusInMetersFromPixels() const
{
	return this->_circle->getRadius() / Settings::getConversionFactor();
}

void MyCircle::accelerate(float xv, float yv)
{
	this->_acceleration.x += xv;
	this->_acceleration.y += yv;
}
void MyCircle::accelerate(sf::Vector2f acc)
{
	this->_acceleration += acc;
}

void MyCircle::updateColor(float deltaTime)
{
	
		this->_value += 1.0f * deltaTime;
		this->_hue += 20.0f * deltaTime;

	if (this->_hue >= 360.0f)
	{
		this->_hue -= 360.0f;
	}
	if (this->_value >= 0.9f)
	{
		this->_value = 0.9f;
	}
	sf::Color newColor = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(newColor);
}

bool MyCircle::isIntersect(const MyCircle& c2) const
{
	sf::Vector2f delta = c2.getPositionInMetersFromPixels() - this->getPositionInMetersFromPixels();
	float distanceSquared = delta.x * delta.x + delta.y * delta.y;
	float radiiSumSquared = (this->getRadiusInMetersFromPixels() + c2.getRadiusInMetersFromPixels());
	radiiSumSquared *= radiiSumSquared;

	if (distanceSquared < radiiSumSquared)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float MyCircle::getMass()
{
	float kg = this->_mass;
	//std::cout << "MASS: " << kg << "\n";
	return kg;
}

void MyCircle::move(sf::Vector2f vec)
{
	sf::Vector2f currentPosition(this->getPositionInMetersFromPixels().x, this->getPositionInMetersFromPixels().y);
	sf::Vector2f movePos(currentPosition += vec);
	this->setPositionFromMetersToPixels(movePos);
}

float MyCircle::getMass() const
{
	return this->_mass;
}
void MyCircle::setVelocity(sf::Vector2f vel)
{
	this->_velocity = vel;
}
void MyCircle::invertXVelocity()
{
	sf::Vector2f inv(this->getVelocity().x * -calculateRestitution(this->getMass()), this->getVelocity().y);
	this->setVelocity(inv);
}
void MyCircle::invertYVelocity()
{
	sf::Vector2f inv(this->getVelocity().x, this->getVelocity().y * -calculateRestitution(this->getMass()));
	this->setVelocity(inv);
}
sf::Vector2f MyCircle::getVelocity() const
{
	return this->_velocity;
}


bool MyCircle::contains(sf::Vector2f& p) const
{
	float dx = std::pow((_circle->getPosition().x - p.x), 2);
	float dy = std::pow((_circle->getPosition().y - p.y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist <= this->_circle->getRadius())
	{
		return true;
	}
	return false;
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

void MyCircle::initMass()
{
	// Mass/size ratio
	float ratio = 2.0f;

	this->_mass = this->getRadiusInMetersFromPixels() * ratio;
}

void MyCircle::resetAcceleration()
{
	sf::Vector2f resAcc(0.0f, 0.0f);
	this->_acceleration = resAcc;
}

sf::Vector2f MyCircle::getRandomPos() const
{
	// Get the dimensions of the screen in pixels
	float screenWidthPixels = static_cast<float>(sf::VideoMode::getDesktopMode().width);
	float screenHeightPixels = static_cast<float>(sf::VideoMode::getDesktopMode().height);

	// Calculate the valid range for random X and Y positions in pixels
	float minX = getRadiusInMetersFromPixels();
	float maxX = screenWidthPixels - getRadiusInMetersFromPixels();
	float minY = getRadiusInMetersFromPixels();
	float maxY = screenHeightPixels - getRadiusInMetersFromPixels();

	// Generate random X and Y positions in pixels within the valid range
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDis(minX, maxX);
	std::uniform_real_distribution<float> yDis(minY, maxY);

	// Create and return the random position vector in meters
	sf::Vector2f randomPosition(
		xDis(gen) / Settings::getConversionFactor(),
		yDis(gen) / Settings::getConversionFactor()
	);
	return randomPosition;
}
float MyCircle::getRandomRad() 
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.3f * Settings::getConversionFactor(), 0.7f * Settings::getConversionFactor());
	float randRad = dis(gen);
	return randRad;
}
void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	//this->_hue = dis(gen);
	this->_value = 0.03f;
	sf::Color randCol = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(randCol);
}
void MyCircle::randomizeVelocity()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1000.0f / Settings::getConversionFactor(), 1000.0f / Settings::getConversionFactor());
	float velX = dis(gen);
	float velY = dis(gen);
	sf::Vector2f newPos(velX , velY);
	this->accelerate(newPos);
}

void MyCircle::applyImpulse(const sf::Vector2f & impulse) {
	_velocity += impulse / _mass;
}

float MyCircle::calculateRestitution(float massRatio)
{
	float minRestitution = 0.001f;
	float maxRestitution = 1.0f;

	if (massRatio < 1.0f) 
	{
		return minRestitution + (maxRestitution - minRestitution) * massRatio;
	}
	else if (massRatio > 1.0f) 
	{
		return minRestitution + (maxRestitution - minRestitution) / massRatio;
	}
	
}
