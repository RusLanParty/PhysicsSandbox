#include "MyCircle.h"
#include "Global.h"

MyCircle::MyCircle():
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_dimmed(false)
{
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_oldPos = std::make_unique<sf::Vector2f>();
	_circle = std::make_unique<sf::CircleShape>();
	_circle->setRadius(getRandomRad());
	this->randomizeColor();
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	sf::Vector2f pos(this->getRandomPos());
	_circle->setPosition(pos);
}

MyCircle::MyCircle(sf::Vector2f& pos):
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_dimmed(false)
{
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_oldPos = std::make_unique<sf::Vector2f>();
	_circle = std::make_unique<sf::CircleShape>();
	_circle->setRadius(getRandomRad());
	this->randomizeSpeed();
	this->randomizeColor();
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	_circle->setPosition(pos);
}

MyCircle::MyCircle(float x, float y, float xs, float ys, float rad):
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_dimmed(false)
{
	_acceleration = std::make_unique<sf::Vector2f>(0.0f, 0.0f);
	_oldPos = std::make_unique<sf::Vector2f>();
	_circle->setRadius(rad);
	this->accelerate(xs, ys);
	_circle->setOrigin(_circle->getRadius(), _circle->getRadius());
	this->randomizeColor();
	_circle->setPosition(x, y);	
}

MyCircle::~MyCircle()
{

}


std::shared_ptr<sf::Vector2f> MyCircle::getAcceleration() const
{
	return this->_acceleration;
}

void MyCircle::accelerate(float xv, float yv)
{
	this->_acceleration->x += xv;
	this->_acceleration->y += yv;
}
void MyCircle::accelerate(std::shared_ptr<sf::Vector2f> acc)
{
	std::shared_ptr<sf::Vector2f> addAcc = std::make_shared<sf::Vector2f>(*this->_acceleration + *acc);
	this->_acceleration = addAcc;
}
void MyCircle::invertXSpeed()
{
	this->_acceleration->x *= -1.0f;
}
void MyCircle::invertYSpeed()
{
	this->_acceleration->y *= -1.0f;
}
void MyCircle::updateColor()
{
	if (!_dimmed) 
	{
		this->_value += 0.04f;
		this->_hue += 0.1f;
	}
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
	float dx = std::pow((c2._circle->getPosition().x - _circle->getPosition().x), 2);
	float dy = std::pow((c2._circle->getPosition().y - _circle->getPosition().y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist < _circle->getRadius() + c2._circle->getRadius())
	{
		return true;
	}
	else { return false; }
}

float MyCircle::getMass() const
{
	return this->_mass;
}
std::shared_ptr<sf::Vector2f> MyCircle::getOldPos() const
{
	return this->_oldPos;
}
void MyCircle::setOldPos(std::shared_ptr<sf::Vector2f> pos)
{
	this->_oldPos = pos;
}
void MyCircle::setOldPosX(float x)
{
	this->_oldPos->x = x;
}
void MyCircle::setOldPosY(float y)
{
	this->_oldPos->y = y;
}
std::shared_ptr<sf::Vector2f> MyCircle::getPosition()
{
	float x = this->_circle->getPosition().x;
	float y = this->_circle->getPosition().y;
	std::shared_ptr<sf::Vector2f> pos = std::make_shared<sf::Vector2f>(x, y);
	return pos;
}
bool MyCircle::contains(sf::Vector2f& p) const
{
	float dx = std::pow((_circle->getPosition().x - p.x), 2);
	float dy = std::pow((_circle->getPosition().y - p.y), 2);
	float dist = std::sqrt(dx + dy);
	if (dist <= _circle->getRadius())
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
void MyCircle::dim()
{
	_dimmed = true;
	if (this->_value > 0.3f) 
	{
		this->_value - 0.05f;
	}
}
void MyCircle::unDim()
{
	_dimmed = false;
	if (this->_value < 1.0f)
	{
		this->_value + 0.05f;
	}
}
sf::Vector2f MyCircle::getRandomPos() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDis(_circle->getRadius(), sf::VideoMode::getDesktopMode().width - 2 * (_circle->getRadius()));
	std::uniform_real_distribution<float> yDis(_circle->getRadius(), sf::VideoMode::getDesktopMode().height - 2 * (_circle->getRadius()));
	return sf::Vector2f(xDis(gen), yDis(gen));
}
float MyCircle::getRandomRad() const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(10.5f, 70.0f);
	return dis(gen);
}
void MyCircle::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	this->_hue = dis(gen);
	this->_value = 0.15f;
	sf::Color randCol = HSVtoRGB(this->_hue, this->_saturation, this->_value);
	_circle->setFillColor(randCol);
}
void MyCircle::randomizeSpeed()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-10.13f, 10.25f);
	float velX = dis(gen);
	float velY = dis(gen);
	std::shared_ptr<sf::Vector2f> acc = std::make_shared<sf::Vector2f>(velX, velY);
	this->_acceleration = acc;
}