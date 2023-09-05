#include "MyCircle.h"
#include "GameManager.h"
static float SEQ_HUE = 0.0f;
MyCircle::MyCircle():
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f)
{	
	this->_hue = SEQ_HUE;
	SEQ_HUE = SEQ_HUE + 1.5f;
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_velocity = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	std::shared_ptr<sf::Vector2f> curPos = std::make_shared<sf::Vector2f>(this->getRandomPos());
	this->setPositionFromMetersToPixels(curPos);
	this->initMass();
	this->_circle->setPointCount(20);
}

MyCircle::MyCircle(sf::Vector2f& pos):
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f)
	
{	
	this->_hue = SEQ_HUE;
	SEQ_HUE = SEQ_HUE + 1.5f;
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_velocity = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	float rad = this->getRandomRad();
	_circle = std::make_unique<sf::CircleShape>(rad);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	//this->randomizeColor();
	std::shared_ptr<sf::Vector2f> curPos = std::make_shared<sf::Vector2f>(pos / Settings::getConversionFactor());
	this->setPositionFromMetersToPixels(curPos);
	this->initMass();
	this->_circle->setPointCount(20);
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
	sf::Vector2f delta = *c2.getPositionInMetersFromPixels() - *this->getPositionInMetersFromPixels();
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
	std::shared_ptr <sf::Vector2f> movePos = std::make_shared<sf::Vector2f>(*this->getPositionInMetersFromPixels() += vec);
	this->setPositionFromMetersToPixels(movePos);
}

float MyCircle::getMass() const
{
	return this->_mass;
}
void MyCircle::setVelocity(std::shared_ptr<sf::Vector2f> vel)
{
	this->_velocity = vel;
}
void MyCircle::invertXVelocity()
{
	std::shared_ptr<sf::Vector2f> inv = std::make_shared<sf::Vector2f>(this->getVelocity()->x * -calculateRestitution(this->getMass()), this->getVelocity()->y);
	this->setVelocity(inv);
}
void MyCircle::invertYVelocity()
{
	std::shared_ptr<sf::Vector2f> inv = std::make_shared<sf::Vector2f>(this->getVelocity()->x, this->getVelocity()->y * -calculateRestitution(this->getMass()));
	this->setVelocity(inv);
}
std::shared_ptr<sf::Vector2f> MyCircle::getVelocity() const
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
	std::shared_ptr<sf::Vector2f> resAcc = std::make_shared<sf::Vector2f>(0.0f, 0.0f);
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
	std::uniform_real_distribution<float> dis(0.08f * Settings::getConversionFactor(), 0.2f * Settings::getConversionFactor());
	float randRad = dis(gen);
	return randRad;
}
void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	//this->_hue = dis(gen);
	this->_value = 0.1f;
	sf::Color randCol = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(randCol);
}
void MyCircle::randomizeVelocity()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-900.0f / Settings::getConversionFactor(), 800.0f / Settings::getConversionFactor());
	float velX = dis(gen);
	float velY = dis(gen);
	std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(velX , velY);
	this->accelerate(newPos);
}

void MyCircle::applyImpulse(const sf::Vector2f & impulse) {
	*_velocity += impulse / _mass;
}

float MyCircle::calculateRestitution(float massRatio)
{
	float minRestitution = 0.2f;
	float maxRestitution = 0.7f;

	if (massRatio < 1.0f) 
	{
		return minRestitution + (maxRestitution - minRestitution) * massRatio;
	}
	else if (massRatio > 1.0f) 
	{
		return minRestitution + (maxRestitution - minRestitution) / massRatio;
	}
	
}
