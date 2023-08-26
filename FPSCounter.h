#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class FPSCounter 
{
public:
	FPSCounter(float x, float y, sf::Font& font);
	void draw(sf::RenderWindow* window);
	void displayFps(float deltaTime);
private:
	std::unique_ptr<sf::Text> _fpsText;
	sf::Clock _clock;
	int _frameCounter;
	float _deltaFrames;
	void updateColor(float fps, float deltaTime);
	void normalizeHSV();
	sf::Color HSVtoRGB(float h, float s, float v);
	int _hue;
	float _sat;
	float _val;
	int _colorTransitionSpeed;
};