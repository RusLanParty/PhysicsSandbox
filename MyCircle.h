#include "SFML/Graphics.hpp";
#include <iostream>;
#include <random>;


class MyCircle : public sf::CircleShape 
{
public:
	MyCircle();
	MyCircle(sf::Vector2f& pos);
	~MyCircle();
	
	sf::Vector2f getSpeed() const;
	void setSpeed(float xs, float ys);
	void invertXSpeed();
	bool contains(sf::Vector2f &p) const;
	void invertYSpeed();
	void randomizeColor();
	void randomizeSpeed();
	void updateMovement();
	bool isIntersect(const sf::CircleShape& c2) const;
	void calculateCollision(MyCircle &c2);
	float getMass() const;
private:
	sf::Vector2f _velocity;
	float mass;
	sf::Vector2f getRandomPos() const;
	float getRandomRad() const;
	float getLimitedRad(float limit) const;
	void initMass();
	
};