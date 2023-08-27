#include "MyCircle.h"
#include "GameManager.h"
MyCircle::MyCircle():
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),	
	_velocity(0.0f, 0.0f)
{
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_oldPos = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	std::shared_ptr<sf::Vector2f> curPos = std::make_shared<sf::Vector2f>(this->getRandomPos());
	this->setPositionFromMetersToPixels(curPos);
	this->setOldPos(curPos);
	//std::cout << "Random METERS X= " << (float)this->getPositionInMetersFromPixels()->x << " Random METERS Y= " << (float)this->getPositionInMetersFromPixels()->y << " Random PIXELS X= " << (float)this->_circle->getPosition().x << " Random PIXELS Y= " << (float)this->_circle->getPosition().y << "\n";
}

MyCircle::MyCircle(sf::Vector2f& pos):
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),	
	_velocity(0.0f, 0.0f)
{
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_oldPos = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	std::shared_ptr<sf::Vector2f> curPos = std::make_shared<sf::Vector2f>(pos / Settings::getConversionFactor());
	this->setPositionFromMetersToPixels(curPos);
	this->setOldPos(curPos);
	//this->randomizeVelocity();
	//std::cout << "Mouse METERS X= " << (float)this->getPositionInMetersFromPixels()->x << " Mouse METERS Y= " << (float)this->getPositionInMetersFromPixels()->y << " Mouse PIXELS X= " << (float)this->_circle->getPosition().x << " Mouse PIXELS Y= " << (float)this->_circle->getPosition().y << "\n";
}


MyCircle::~MyCircle()
{

}


std::shared_ptr<sf::Vector2f> MyCircle::getAcceleration() const
{
	std::shared_ptr<sf::Vector2f> acc = std::make_shared<sf::Vector2f>(
		_acceleration->x,
		_acceleration->y
	);

	return acc;
}

std::shared_ptr<sf::Vector2f> MyCircle::getPositionInMetersFromPixels() const
{
	std::shared_ptr<sf::Vector2f> pos = std::make_shared<sf::Vector2f> (_circle->getPosition() / Settings::getConversionFactor());
	return pos;
}

void MyCircle::setPositionFromMetersToPixels(std::shared_ptr<sf::Vector2f> pos)
{
	this->_circle->setPosition(*pos * Settings::getConversionFactor());
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
	this->_acceleration->x += xv;
	this->_acceleration->y += yv;
}
void MyCircle::accelerate(std::shared_ptr<sf::Vector2f> acc)
{
	*this->_acceleration += *acc;

}

void MyCircle::updateColor(float deltaTime)
{
	
		this->_value += 1.3f * deltaTime;
		this->_hue += 20.0f * deltaTime;

	if (this->_hue >= 360.0f)
	{
		this->_hue -= 360.0f;
	}
	if (this->_value >= 1.0f)
	{
		this->_value = 1.0f;
	}
	sf::Color newColor = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(newColor);
}

bool MyCircle::isIntersect(const MyCircle& c2) const
{
	sf::Vector2f delta = *c2.getPositionInMetersFromPixels() - *this->getPositionInMetersFromPixels();
	float distanceSquared = delta.x * delta.x + delta.y * delta.y;
	float radiiSumSquared = (this->getRadiusInMetersFromPixels() + c2.getRadiusInMetersFromPixels()) * (this->getRadiusInMetersFromPixels() + c2.getRadiusInMetersFromPixels());

	if (distanceSquared < radiiSumSquared)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float MyCircle::getMass() const
{
	return this->_mass;
}
std::shared_ptr<sf::Vector2f> MyCircle::getOldPos() const
{
	std::shared_ptr<sf::Vector2f> oldPos = std::make_shared<sf::Vector2f>(*this->_oldPos);
	return oldPos;
}
void MyCircle::setOldPos(std::shared_ptr<sf::Vector2f> pos)
{
	*this->_oldPos = *pos;
}
void MyCircle::setOldPosX(float x)
{
	this->_oldPos->x = x;
}
void MyCircle::setOldPosY(float y)
{
	this->_oldPos->y = y;
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

void MyCircle::resetAcceleration()
{
	std::shared_ptr<sf::Vector2f> resAcc = std::make_shared<sf::Vector2f>(0.0f, 0.0f);
	this->_acceleration = resAcc;
}
void MyCircle::reflectVelocity(const sf::Vector2f& collisionNormal, float restitutionCoefficient, float timeStep)
{
	// Calculate the velocity using the change in position
	sf::Vector2f velocity = (*this->getPositionInMetersFromPixels() - *getOldPos()) / timeStep;

	// Calculate the dot product between velocity and collision normal
	float dotProduct = velocity.x * collisionNormal.x + velocity.y * collisionNormal.y;

	// Calculate the angle of incidence
	float angleOfIncidence = std::atan2(collisionNormal.y, collisionNormal.x);

	// Calculate the angle of reflection
	float angleOfReflection = 2.0f * angleOfIncidence - std::atan2(velocity.y, velocity.x);

	// Calculate the magnitude of the reflected velocity
	float reflectedVelocityMagnitude = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) * restitutionCoefficient;

	// Calculate the reflected velocity components
	sf::Vector2f reflectedVelocity;
	reflectedVelocity.x = reflectedVelocityMagnitude * std::cos(angleOfReflection);
	reflectedVelocity.y = reflectedVelocityMagnitude * std::sin(angleOfReflection);

	// Update the circle's position and old position
	std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(*this->getPositionInMetersFromPixels() + reflectedVelocity * timeStep);
	setOldPos(std::make_shared<sf::Vector2f>(*this->getPositionInMetersFromPixels()));
	//this->setPositionFromMetersToPixels(newPos);
	this->accelerate(reflectedVelocity.x, reflectedVelocity.y);
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
	std::uniform_real_distribution<float> dis(0.1f * Settings::getConversionFactor(), 0.2f * Settings::getConversionFactor());
	float randRad = dis(gen);
	return randRad;
}
void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	this->_hue = dis(gen);
	this->_value = 0.1f;
	sf::Color randCol = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(randCol);
}
void MyCircle::randomizeVelocity()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.0008f , 0.0008f);
	float velX = dis(gen);
	float velY = dis(gen);
	std::shared_ptr<sf::Vector2f> curPos = std::make_shared<sf::Vector2f>(*this->getPositionInMetersFromPixels());
	this->setOldPos(curPos);
	std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(this->getPositionInMetersFromPixels()->x + velX, this->getPositionInMetersFromPixels()->y + velY);
	this->setPositionFromMetersToPixels(newPos);
}