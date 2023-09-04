#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>

class MyCircle
{
public:
	MyCircle();
	MyCircle(sf::Vector2f& pos);
	~MyCircle();
	std::unique_ptr<sf::CircleShape> _circle;
	float getMass() const;
	void setVelocity(std::shared_ptr<sf::Vector2f> vel);
	void invertXVelocity();
	void invertYVelocity();
	void applyImpulse(const sf::Vector2f& impulse);
	static float calculateRestitution(float massRatio);
	std::shared_ptr<sf::Vector2f> getVelocity() const;
	std::shared_ptr<sf::Vector2f> getAcceleration() const;
	std::shared_ptr<sf::Vector2f> getPositionInMetersFromPixels() const;
	void setPositionFromMetersToPixels(std::shared_ptr<sf::Vector2f> pos);
	void setPositionFromMetersToPixels(float x, float y);
	const float getRadiusInMetersFromPixels() const;
	void accelerate(float xs, float ys);
	void accelerate(std::shared_ptr<sf::Vector2f> acc);
	void resetAcceleration();	
	void updateColor(float deltaTime);
	void randomizeColor();
	void randomizeVelocity();
	bool contains(sf::Vector2f &p) const;
	bool isIntersect(const MyCircle& c2) const;
	float getMass();
	void move(sf::Vector2f vec);
private:	
	std::shared_ptr<sf::Vector2f> _acceleration;	
	std::shared_ptr<sf::Vector2f> _velocity;
	float _hue;
	float _saturation;
	float _value;
	float _mass;
	sf::Vector2f getRandomPos() const;
	static float getRandomRad();
	sf::Color HSVtoRGB(float hue, float saturation, float value);
	void initMass();
	
	
};