#include "FPSCounter.h"
#include "GameManager.h"

FPSCounter::FPSCounter(float x, float y, sf::Font& font) :
	_colorTransitionSpeed(2.0f),
	_deltaFrames(10.0f),
	_frameCounter(0),
	_hue(0.0f),
	_sat(1.0f),
	_val(1.0f)
{
	_fpsText = std::make_unique<sf::Text>();
	_fpsText->setFont(font);
	_fpsText->setPosition(x, y);
	_fpsText->setString("FPS: ");
	_fpsText->setCharacterSize(20);
	sf::Color color(HSVtoRGB(this->_hue, this->_sat, this->_val));
	_fpsText->setFillColor(color);
}


void FPSCounter::displayFps()
{
	_frameCounter++;
	if (_frameCounter == _deltaFrames) 
	{
		_frameCounter = 0;
		// restarting the timer
		sf::Time elapsed = _clock.restart();
		int fps = _deltaFrames / elapsed.asSeconds();
		_fpsText->setString("FPS: " + std::to_string(static_cast<int>(fps)));
		updateColor(fps);
	}
}

void FPSCounter::updateColor(float fps)
{
	if (fps < 30.0f) 
	{
		if (this->_hue > 0.0f) 
		{
			this->_hue -= _colorTransitionSpeed;
		}
		this->_sat = 1.0f;
		this->_val = 1.0f;
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 30.0f && fps < 60.0f) 
	{
		if (this->_hue > 30.0f)
		{
			this->_hue -= _colorTransitionSpeed;
		}
		else if (this->_hue < 30.0f)
		{
			this->_hue += _colorTransitionSpeed;
		}
		this->_sat = 1.0f;
		this->_val = 1.0f;
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 60.0f && fps < 130.0f) 
	{
		if (this->_hue > 60.0f)
		{
			this->_hue -= _colorTransitionSpeed;
		}
		else if (this->_hue < 60.0f)
		{
			this->_hue += _colorTransitionSpeed;
		}
		this->_sat = 1.0f;
		this->_val = 1.0f;
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 130.0f)
	{
		if (this->_hue > 120.0f)
		{
			this->_hue -= _colorTransitionSpeed;
		}
		else if (this->_hue < 120.0f)
		{
			this->_hue += _colorTransitionSpeed;
		}
		this->_sat = 1.0f;
		this->_val = 1.0f;
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
}

void FPSCounter::normalizeHSV()
{
	if (this->_hue > 360.0f)
	{
		this->_hue -= 360.0f;
	}
	else if (this->_hue < 0.0f)
	{
		this->_hue += 360.0f;
	}
	if (this->_sat > 1.0f)
	{
		this->_sat = 1.0f;
	}
	else if (this->_sat < 0.0f)
	{
		this->_sat = 0.0f;
	}
	if (this->_val > 1.0f)
	{
		this->_val = 1.0f;
	}
	else if (this->_val < 0.0f)
	{
		this->_val = 0.0f;
	}
}

void FPSCounter::draw(sf::RenderWindow* window)
{
	window->draw(*_fpsText);
}
sf::Color FPSCounter::HSVtoRGB(float h, float s, float v) 
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