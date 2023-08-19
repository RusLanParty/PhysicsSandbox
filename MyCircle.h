#include "SFML/Graphics.hpp";
#include <iostream>;
#include <random>;


class MyCircle : public sf::CircleShape 
{
public:
	MyCircle();
	MyCircle(sf::Vector2f& pos);
	MyCircle(float w, float h, float sx, float sy, float rad);
	~MyCircle();
	
	float getMass() const;
	sf::Vector2f getSpeed() const;
	void setSpeed(float xs, float ys);
	void invertXSpeed();
	void invertYSpeed();
	void decreaseSpeed();
	void updateMovement();
	void updateColor();
	void randomizeColor();
	void randomizeSpeed();
	bool contains(sf::Vector2f &p) const;
	bool isIntersect(const MyCircle& c2) const;
	void resolveIntersections(MyCircle& c2);
	void calculateCollision(MyCircle &c2);
	std::vector<sf::Vector2f> trail;
private:	
	float hue;
	float saturation;
	float value;
	sf::Vector2f _velocity;
	float mass;
	sf::Vector2f getRandomPos() const;
	float getRandomRad() const;
	void initMass();
	sf::Color HSVtoRGB(float hue, float saturation, float value);
	
};