#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>

class MyCircle
{
public:
	MyCircle();
	MyCircle(sf::Vector2f& pos);
	MyCircle(float w, float h, float sx, float sy, float rad);
	~MyCircle();
	std::unique_ptr<sf::CircleShape> _circle;
	float getMass() const;
	std::shared_ptr<sf::Vector2f> getOldPos() const;
	void setOldPos(const std::shared_ptr<sf::Vector2f>);
	void setOldPosX(float x);
	void setOldPosY(float y);
	std::shared_ptr<sf::Vector2f> getPosition();
	std::shared_ptr<sf::Vector2f> getAcceleration() const;
	void accelerate(float xs, float ys);
	void accelerate(std::shared_ptr<sf::Vector2f> acc);
	void invertXSpeed();
	void invertYSpeed();
	void resetAcceleration();
	void dim();
	void unDim();
	void updateColor();
	void randomizeColor();
	void randomizeSpeed();
	bool contains(sf::Vector2f &p) const;
	bool isIntersect(const MyCircle& c2) const;
private:	
	std::shared_ptr<sf::Vector2f> _acceleration;
	std::shared_ptr<sf::Vector2f> _oldPos;
	bool _dimmed;
	float _hue;
	float _saturation;
	float _value;
	float _mass;
	sf::Vector2f getRandomPos() const;
	float getRandomRad() const;
	sf::Color HSVtoRGB(float hue, float saturation, float value);
	
	
};