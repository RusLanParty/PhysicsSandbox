#pragma once
#include "SFML/Graphics.hpp"
#include "MyCircle.h"
#include <random>
#include <iostream>
#include <queue>
class MyText 
{
public:
	std::unique_ptr<sf::Text> _text;
	MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts);
	MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts, sf::Color color);
	MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts, sf::Color color, bool instantFade);
	~MyText();
	void updateText(float deltaTime);
	void fadeIn();
	void fadeOut();
	bool isSafeToRemove();
	bool getCollisionsState() const;
	bool isIntersect(std::shared_ptr<MyCircle> circle) const;
	
private:
	static bool BLOCKSPAWN;
	float static FADEINSPEED;
	float static FADEOUTSPEED;
	int static COUNT;
	static float SEQ_HUE;
	static float getRandomHue();
	bool _instantFade;
	bool _collision;
	bool _rdyForRemove;
	static std::deque<MyText*> _fadeInQ;
	static std::deque<MyText*> _fadeOutQ;
	int _id;
	std::vector<std::unique_ptr<MyText>>* _texts;
	bool _colorTransition;
	int _alpha;
	float _hue;
	float _saturation;
	float _value;
	void normalize();
	bool fadingIn(float deltaTime);
	bool fadingOut(float deltaTime);
	void cycleColorOnConstruct();
	void updateColor(float deltaTime);
	void updateNoColor(float deltaTime);
	sf::Color HSVtoRGB(float h, float s, float v) const;
	sf::Color RGBtoHSV(const sf::Color& rgbColor) const;
};
