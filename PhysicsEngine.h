#pragma once
#include <SFML/Graphics.hpp>
#include "MyCircle.h"
#include "MyText.h"

class PhysicsEngine 
{
public:
	PhysicsEngine();
	void applyPhysics(std::shared_ptr<MyCircle> circle, float deltaTime);
	void toggleGravity();
	bool getGravityState();
	void resolveIntersections(std::shared_ptr<MyCircle> circle, MyCircle& other);
	void resolveTextIntersections(std::shared_ptr<MyCircle> circle, MyText& text);
	float _width;
	float _height;
	static std::shared_ptr<sf::Vector2f> _gravityValue;
	static bool _gravity;
	void applyGravity(std::shared_ptr<MyCircle> circle, float subDt);
	void updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime);
	void checkBounds(std::shared_ptr<MyCircle> circle, float deltaTime);
	sf::Vector2f normalize(const sf::Vector2f& vector);
	float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
	float length(const sf::Vector2f& vector);
};