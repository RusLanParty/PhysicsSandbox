#pragma once
#include <SFML/Graphics.hpp>
#include "MyCircle.h"
class PhysicsEngine 
{
public:
	PhysicsEngine(sf::RenderWindow*);
	void applyPhysics(std::shared_ptr<MyCircle> circle, float deltaTime);
	void toggleGravity();
	bool getGravityState();
	void resolveIntersections(std::shared_ptr<MyCircle> circle, MyCircle& other);
private:
	sf::RenderWindow* _window;
	static std::shared_ptr<sf::Vector2f> _gravityValue;
	static bool _gravity;
	void applyGravity(std::shared_ptr<MyCircle> circle);
	//void collisions();
	void updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime);
	void bounds(sf::RenderWindow* window, std::shared_ptr<MyCircle> circle);
	void bbox(std::shared_ptr<MyCircle> circle);
};