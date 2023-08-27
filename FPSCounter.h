#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class FPSCounter 
{
public:
	FPSCounter(float x, float y, float z, float w, sf::Font& font);
	void draw(sf::RenderWindow* window);
	void displayFps(float deltaTime);
	void displayOjbectCount();
private:
	std::unique_ptr<sf::Text> _fpsText;
	std::unique_ptr<sf::Text> _objectsText;
	sf::Clock _clock;
	int _frameCounter;
	float _deltaFrames;
	int _frameCounter1;
	float _deltaFrames1;
	void updateColor(float fps, float deltaTime);
	void normalizeHSV();
	sf::Color HSVtoRGB(float h, float s, float v);
	int _hue;
	float _sat;
	float _val;
	int _colorTransitionSpeed;
};