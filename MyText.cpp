#include "MyText.h"
#include "GameManager.h"
float MyText::fadeInSpeed = 0.002f;
float MyText::fadeOutSpeed = 0.002f;
int MyText::count = 0;
float MyText::seqHue = getRandomHue();
std::queue<MyText*>MyText::_fadeInQ;
std::queue<MyText*>MyText::_fadeOutQ;


MyText::MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts) :
	_texts(&texts),
	_rdyForRemove(false),
	_colorTransition(true),
	_alpha(0),
	_fadeState(FadeState::Idle),
	_hue(seqHue),
	_saturation(1.0f),
	_value(0.0f),
	_id(count)
{
	_text = std::make_unique<sf::Text>();
	count++;
	sf::Vector2f position(x, y);
	_text->setFont(font);
	_text->setString(text);
	sf::Color hsv(HSVtoRGB(this->_hue, this->_saturation, this->_value));
	_text->setFillColor(hsv);
	_text->setCharacterSize(30);
	float width = _text->getLocalBounds().width;
	float height = _text->getLocalBounds().height;
	_text->setOrigin(width / 2, height / 2);
	_text->setPosition(position);
	this->cycleColorOnConstruct();
	this->fadeIn();
}

MyText::MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts, sf::Color color) :
	_texts(&texts),
	_rdyForRemove(false),
	_colorTransition(false),
	_alpha(0),
	_fadeState(FadeState::Idle),
	_hue(0.0f),
	_saturation(1.0f),
	_value(0.0f),
	_id(count)
{
	_text = std::make_unique<sf::Text>();
	count++;
	sf::Vector2f position(x, y);
	_text->setString(text);
	_text->setFont(font);
	_text->setFillColor(color);
	sf::Color hsvColor = RGBtoHSV(_text->getFillColor());
	_hue = hsvColor.r;
	_saturation = hsvColor.g;
	sf::Color hsv(HSVtoRGB(this->_hue, this->_saturation, this->_value));
	_text->setFillColor(hsv);
	_text->setCharacterSize(35);
	float width = _text->getLocalBounds().width;
	float height = _text->getLocalBounds().height;
	_text->setOrigin(width / 2, height / 2);
	_text->setPosition(position);
	this->fadeIn();
}
MyText::~MyText()
{
}

void MyText::updateText()
{
	updateNoColor();
	if (_colorTransition)
	{
		this->updateColor();
	}
	switch (_fadeState) 
	{
	case FadeState::FadingIn:

		if (!_fadeInQ.empty())
		{
			MyText* text = _fadeInQ.front();
			if (!text->fadingIn()) 
			{
				_fadeState = FadeState::Idle;
				_fadeInQ.pop();
			}
		}
		break;

	case FadeState::FadingOut:
		if (!_fadeOutQ.empty())
		{
			MyText* text = _fadeOutQ.front();
			if (!text->fadingOut()) 
			{
				this->_fadeState = FadeState::Idle;
				_fadeOutQ.pop();
				this->_rdyForRemove = true;
			}
		}
		break;
	case FadeState::Idle:

		break;
	}
}

void MyText::fadeIn()
{
	_fadeInQ.push(this);
	this->_fadeState = FadeState::FadingIn;
}

void MyText::fadeOut()
{
	_fadeOutQ.push(this);
	this->_fadeState = FadeState::FadingOut;
}



bool MyText::fadingIn()
{
	if (this->_value < 1.0f)
	{
		this->_value += fadeInSpeed;
		return true;
	}
	else
	{ 
		return false; 
	}
}

bool MyText::fadingOut()
{
	if (this->_value > 0.0f)
	{
		this->_value -= fadeOutSpeed;
		return true;
	}
	else
	{
		return false;
	}
}
float MyText::getRandomHue() 
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 360);
	float hue = dis(gen);
	return hue;
}
void MyText::cycleColorOnConstruct()
{
	seqHue = seqHue + 12.0f;
}
void MyText::updateNoColor()
{	
	sf::Color hsv(HSVtoRGB(this->_hue, this->_saturation, this->_value));
	_text->setFillColor(hsv);
}
void MyText::updateColor()
{
		this->_hue += 0.3f;
		if (this->_hue >= 360.0f)
		{
			this->_hue -= 360.0f;
		}

		// Convert back to RGB and set fill color
		sf::Color hsv(HSVtoRGB(this->_hue, this->_saturation, this->_value));
		_text->setFillColor(hsv);
}
bool MyText::isSafeToRemove()
{
	return _rdyForRemove;
}
sf::Color MyText::HSVtoRGB(float h, float s, float v) const
{
	int hi = static_cast<int>(h / 60) % 6;
	float f = h / 60 - std::floor(h / 60);
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	sf::Uint8 r, g, b;

	switch (hi) {
	case 0: r = static_cast<sf::Uint8>(v * 255); g = static_cast<sf::Uint8>(t * 255); b = static_cast<sf::Uint8>(p * 255); break;
	case 1: r = static_cast<sf::Uint8>(q * 255); g = static_cast<sf::Uint8>(v * 255); b = static_cast<sf::Uint8>(p * 255); break;
	case 2: r = static_cast<sf::Uint8>(p * 255); g = static_cast<sf::Uint8>(v * 255); b = static_cast<sf::Uint8>(t * 255); break;
	case 3: r = static_cast<sf::Uint8>(p * 255); g = static_cast<sf::Uint8>(q * 255); b = static_cast<sf::Uint8>(v * 255); break;
	case 4: r = static_cast<sf::Uint8>(t * 255); g = static_cast<sf::Uint8>(p * 255); b = static_cast<sf::Uint8>(v * 255); break;
	default: r = static_cast<sf::Uint8>(v * 255); g = static_cast<sf::Uint8>(p * 255); b = static_cast<sf::Uint8>(q * 255); break;
	}

	return sf::Color(r, g, b);
}
sf::Color MyText::RGBtoHSV(const sf::Color& rgbColor) const 
{
    float r = (float)rgbColor.r / 255.0f;
    float g = (float)rgbColor.g / 255.0f;
    float b = (float)rgbColor.b / 255.0f;

    float maxColor = std::max(r, std::max(g, b));
    float minColor = std::min(r, std::min(g, b));
    float delta = maxColor - minColor;

    // Calculate hue
    float hue = 0.0f;
    if (delta > 0.0f) {
        if (maxColor == r) {
            hue = 60.0f * (fmodf(((g - b) / delta), 6.0f));
        }
        else if (maxColor == g) {
            hue = 60.0f * (((b - r) / delta) + 2.0f);
        }
        else if (maxColor == b) {
            hue = 60.0f * (((r - g) / delta) + 4.0f);
        }
    }
    if (hue < 0.0f) {
        hue += 360.0f;
    }

    // Calculate saturation
    float saturation = (maxColor > 0.0f) ? (delta / maxColor) : 0.0f;
	
    // Calculate value
    float value = maxColor;

    sf::Color hsvColor;
    hsvColor.r = hue;          // Set hue value
    hsvColor.g = saturation;   // Set saturation value
    hsvColor.b = value;        // Set value (brightness) value
    hsvColor.a = (float)rgbColor.a;   // Preserve alpha value

    return hsvColor;
}

