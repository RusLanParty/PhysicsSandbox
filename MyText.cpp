#include "MyText.h"
int MyText::fadeInSpeed = 1;
int MyText::fadeOutSpeed = 1;
sf::Font MyText::_font;

MyText::MyText(const std::string& text, float x, float y) :
	_alpha(0),
	_fadeState(FadeState::Idle)
{
	if (!_font.loadFromFile("arial.ttf")) {
		std::cout << "Error loading arial.ttf";
	}
	else {
		std::cout << "Font loaded successfully!" << std::endl;
	}
	int width = this->getLocalBounds().width;
	int height = this->getLocalBounds().height;
	sf::Vector2f position(x, y);
	this->randomizeColor();
	this->setFont(_font);
	sf::Color color(this->getFillColor());
	color.a = 0;
	this->setFillColor(color);
	this->setOrigin(width / 2, height / 2);
	this->setPosition(position);
	this->setString(text);
	this->setCharacterSize(20);
	this->fadeIn();
}

void MyText::updateText()
{
	switch (_fadeState) 
	{
	case FadeState::FadingIn:
		if (!fadingIn()) 
		{
			_fadeState = FadeState::Idle;
		}
		break;
	case FadeState::FadingOut:
		if (!fadingOut()) 
		{
			_fadeState = FadeState::Idle;
		}
		break;
	default:
		break;
	}
}

void MyText::fadeIn()
{
	_fadeState = FadeState::FadingIn;
}

void MyText::fadeOut()
{
	_fadeState = FadeState::FadingOut;
}

bool MyText::fadingIn()
{
	sf::Color color = getFillColor();
	if (color.a < 255)
	{
		_alpha += fadeInSpeed;
		color.a = static_cast<sf::Uint8>(_alpha);
		this->setFillColor(color); 
		return true;
	}
	else
	{ 
		return false; 
	}
}

bool MyText::fadingOut()
{
	sf::Color color = getFillColor();
	if (color.a < 255)
	{
		_alpha -= fadeOutSpeed;
		color.a = static_cast<sf::Uint8>(_alpha);
		this->setFillColor(color);
		return true;
	}
	else
	{
		return false;
	}
}

void MyText::randomizeColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	float hue = dis(gen);
	sf::Color randCol = HSVtoRGB(hue, 1.0f, 1.0f);
	this->setFillColor(randCol);
}
sf::Color MyText::HSVtoRGB(float h, float s, float v)
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
