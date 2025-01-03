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
	void resolveCollision(std::shared_ptr<MyCircle> circle, std::shared_ptr<MyCircle> circle2);
	void resolveTextCollision(std::shared_ptr<MyCircle> circle, std::shared_ptr<MyText> text);
	void drawBound(sf::RenderWindow* window);
	void setDimensions(float conversionFactor);
	float _width;
	float _height;
private:
	//std::unique_ptr<sf::CircleShape> _bound;
	static sf::Vector2f _gravityValue;
	static bool _gravity;
	void applyGravity(std::shared_ptr<MyCircle> circle, float subDt);
	float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
	float length(const sf::Vector2f& vector);
	sf::Vector2f normalize(const sf::Vector2f& vector);
	void checkBounds(std::shared_ptr<MyCircle> circle, float deltaTime);
	void updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime);
	void updateVelocity(std::shared_ptr<MyCircle> circle, float deltaTIme);
};