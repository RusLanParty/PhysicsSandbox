#include "FPSCounter.h"
#include "GameManager.h"

FPSCounter::FPSCounter(float x, float y, float z, float w, sf::Font& font) :
	_colorTransitionSpeed(400),
	_deltaFrames(20.0f),
	_deltaFrames1(20.0f),
	_frameCounter(0),
	_hue(0),
	_sat(0.0f),
	_val(1.0f)
{
	_fpsText = std::make_shared<sf::Text>();
	_objectsText = std::make_shared<sf::Text>();
	_fpsText->setFont(font);
	_objectsText->setFont(font);
	_fpsText->setPosition(x, y);
	_objectsText->setPosition(z, w);
	_fpsText->setCharacterSize(20);
	_objectsText->setCharacterSize(20);
	sf::Color color(HSVtoRGB(this->_hue, this->_sat, this->_val));
	_fpsText->setFillColor(color);
	_objectsText->setFillColor(color);
}


void FPSCounter::displayFps(float deltaTime)
{	
	_frameCounter++;
	if (_frameCounter == _deltaFrames) 
	{
		_frameCounter = 0;
		// restarting the timer
		sf::Time elapsed = _clock.restart();
		int fps = _deltaFrames / elapsed.asSeconds();
		_fpsText->setString("FPS: " + std::to_string(static_cast<int>(fps)));
		updateColor(fps, deltaTime);
	}
}

void FPSCounter::displayOjbectCount()
{
	_frameCounter1++;
	if (_frameCounter1 == _deltaFrames1)
	{
		_frameCounter1 = 0;
		int objCount = GameManager::getObjectCount();
		_objectsText->setString("OBJECTS COUNT: " + std::to_string(static_cast<int>(objCount)));
	}
}

void FPSCounter::updateColor(float fps, float deltaTime)
{
	if (this->_sat < 1.0f) 
	{
		_sat += 2.5f * deltaTime;
	}
	if (fps < 30.0f) 
	{
		if (this->_hue > 0) 
		{
			this->_hue -= std::ceil(_colorTransitionSpeed * deltaTime);
		}
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 30.0f && fps < 60.0f) 
	{
		if (this->_hue > 30)
		{
			this->_hue -= std::ceil(_colorTransitionSpeed * deltaTime);
		}
		else if (this->_hue < 30)
		{
			this->_hue += std::ceil(_colorTransitionSpeed * deltaTime);
		}
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 60.0f && fps < 130.0f) 
	{
		if (this->_hue > 60)
		{
			this->_hue -= std::ceil(_colorTransitionSpeed * deltaTime);
		}
		else if (this->_hue < 60)
		{
			this->_hue += std::ceil(_colorTransitionSpeed * deltaTime);
		}
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	else if (fps > 130.0f)
	{
		if (this->_hue > 120)
		{
			this->_hue -= std::ceil(_colorTransitionSpeed * deltaTime);
		}
		else if (this->_hue < 120)
		{
			this->_hue += std::ceil(_colorTransitionSpeed * deltaTime);
		}
		normalizeHSV();
		sf::Color newColor(HSVtoRGB(this->_hue, this->_sat, this->_val));
		_fpsText->setFillColor(newColor);
	}
	//std::cout << "Hue= " << (float)this->_hue << "\n";
}

void FPSCounter::normalizeHSV()
{
	if (this->_hue > 360)
	{
		this->_hue -= 360;
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
	window->draw(*_objectsText);
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
