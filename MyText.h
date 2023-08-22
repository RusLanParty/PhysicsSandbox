#pragma once
#include <random>
#include "SFML/Graphics.hpp"
#include <iostream>
#include <queue>
class MyText : public sf::Text 
{
public:
	enum class FadeState { Idle, FadingIn, FadingOut };
	float static fadeInSpeed;
	float static fadeOutSpeed;
	int static _count;

	MyText(const std::string& text, float x, float y, std::vector<std::unique_ptr<MyText>>& texts);
	MyText(const std::string& text, float x, float y, std::vector<std::unique_ptr<MyText>>& texts, sf::Color color);
	void updateText();
	void fadeIn();
	void fadeOut();
	static void loadFont();
	bool isSafeToRemove();
	
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
	static std::shared_ptr<sf::Font> _font;
	bool fadingIn();
	bool fadingOut();
	void randomizeColor();
	void updateColor();
	void updateNoColor();
	sf::Color HSVtoRGB(float h, float s, float v) const;
	sf::Color RGBtoHSV(const sf::Color& rgbColor) const;
};
