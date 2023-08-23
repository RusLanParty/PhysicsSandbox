#pragma once
#include "SFML/Graphics.hpp"
#include <random>
#include <iostream>
#include <queue>
class MyText 
{
public:
	enum class FadeState { Idle, FadingIn, FadingOut };
	float static fadeInSpeed;
	float static fadeOutSpeed;
	int static count;
	static float seqHue;
	std::unique_ptr<sf::Text> _text;

	MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts);
	MyText(const std::string& text, float x, float y, sf::Font& font, std::vector<std::unique_ptr<MyText>>& texts, sf::Color color);
	~MyText();
	void updateText();
	void fadeIn();
	void fadeOut();
	bool isSafeToRemove();
	static float getRandomHue();
	
private:
	bool _rdyForRemove;
	static std::queue<MyText*> _fadeInQ;
	static std::queue<MyText*> _fadeOutQ;
	int _id;
	std::vector<std::unique_ptr<MyText>>* _texts;
	bool _colorTransition;
	int _alpha;
	FadeState _fadeState;
	float _hue;
	float _saturation;
	float _value;
	bool fadingIn();
	bool fadingOut();
	void cycleColorOnConstruct();
	void updateColor();
	void updateNoColor();
	sf::Color HSVtoRGB(float h, float s, float v) const;
	sf::Color RGBtoHSV(const sf::Color& rgbColor) const;
};
