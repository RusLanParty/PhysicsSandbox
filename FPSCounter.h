#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class FPSCounter 
{
public:
	FPSCounter(float x, float y, sf::Font& font);
	void draw(sf::RenderWindow* window);
	void displayFps();
private:
	std::unique_ptr<sf::Text> _fpsText;
	sf::Clock _clock;
	int _frameCounter;
	float _deltaFrames;
	void updateColor(float fps);
	void normalizeHSV();
	sf::Color HSVtoRGB(float h, float s, float v);
	float _hue;
	float _sat;
	float _val;
	float _colorTransitionSpeed;
};