#pragma once
#ifndef MYTEXT_H
#define MYTEXT_H
#include <random>;
#include "SFML/Graphics.hpp";
#include <iostream>;
class MyText : public sf::Text 
{
public:
	enum class FadeState { Idle, FadingIn, FadingOut };
	int static fadeInSpeed;
	int static fadeOutSpeed;

	MyText(const std::string& text, float x, float y);
	void updateText();
	void fadeIn();
	void fadeOut();
	
private:
	FadeState _fadeState;
	static sf::Font _font;
	int _alpha;
	bool fadingIn();
	bool fadingOut();
	void randomizeColor();
	sf::Color HSVtoRGB(float h, float s, float v);
};
#endif
