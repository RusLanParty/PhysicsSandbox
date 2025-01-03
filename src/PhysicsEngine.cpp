#include "PhysicsEngine.h"
#include "GameManager.h"
#include "MyText.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif


sf::Vector2f PhysicsEngine::_gravityValue(0.0f, 9.81f);
bool PhysicsEngine::_gravity = true;

PhysicsEngine::PhysicsEngine()

{    
    this->setDimensions(10.0f);
    //_bound = std::make_unique<sf::CircleShape>(5.0f * Settings::getConversionFactor());
   // _bound->setFillColor(sf::Color::Black);
    //_bound->setOrigin(_bound->getRadius(), _bound->getRadius());
    //_bound->setPosition(_width / 2 * Settings::getConversionFactor(), _height / 2 * Settings::getConversionFactor());
    //std::cout << "X= " << _bound->getPosition().x / Settings::getConversionFactor() << " Y= " << _bound->getPosition().y / Settings::getConversionFactor() << "\n";
}

void PhysicsEngine::applyPhysics(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    const uint32_t sub_step = 2;
    float sub_dt = deltaTime / (float)sub_step;

    // Updating (sub_step) times between each frame to increase stability
    for (int i = 0; i < sub_step - 1; i++)
    {
        // Performing leapfrog integration

            // Current position + velocity
            updatePosition(circle, sub_dt);
        
            // Applying all forces and accumulating acceleration
            if (_gravity)
            {
                applyGravity(circle, sub_dt);
            }

            // Current velocity + acceleration accumulated in the frame
            updateVelocity(circle, sub_dt);

            // Checking bounds and handling collisions with them
            checkBounds(circle, sub_dt);
    }
}
void PhysicsEngine::updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    sf::Vector2f newPos(circle->getPositionInMetersFromPixels() + (circle->getVelocity() / (deltaTime * 60)));
    circle->setPositionFromMetersToPixels(newPos);
}
void PhysicsEngine::drawBound(sf::RenderWindow* window)
{
    //window->draw(*_bound);
}
void PhysicsEngine::setDimensions(float conversionFactor)
{
    Settings::setConversionFactor(conversionFactor);
    sf::VideoMode windowRes = sf::VideoMode::getDesktopMode();
    this->_width = static_cast<float>(windowRes.width) / Settings::getConversionFactor();
    this->_height = static_cast<float>(windowRes.height) / Settings::getConversionFactor();
}
void PhysicsEngine::updateVelocity(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    sf::Vector2f newVel(circle->getVelocity() + (circle->getAcceleration() * deltaTime));
    circle->setVelocity(newVel);
    circle->resetAcceleration();
}
void PhysicsEngine::resolveCollision(std::shared_ptr<MyCircle> circle1, std::shared_ptr<MyCircle> circle2) 
{
    sf::Vector2f delta = circle2->getPositionInMetersFromPixels() - circle1->getPositionInMetersFromPixels();
    float distance = length(delta);

    sf::Vector2f normal = normalize(delta);

    // Calculate mass ratio
    float massRatio = circle2->getMass() / circle1->getMass();

    // Map mass ratio to restitution
    float restitution = MyCircle::calculateRestitution(massRatio);

    // Calculate relative velocity
    sf::Vector2f relativeVelocity = circle2->getVelocity() - circle1->getVelocity();
    float relativeSpeed = dot(relativeVelocity, normal);

    // Calculate impulse magnitude
    float impulseMagnitude = (1 + restitution) * relativeSpeed / (1 / circle1->getMass() + 1 / (circle2->getMass() * massRatio));

    // Apply impulse to both circles
    sf::Vector2f impulse = impulseMagnitude * normal;
    circle1->applyImpulse(impulse);
    circle2->applyImpulse(-impulse);

    // Separate circles to avoid penetration
    float overlap = circle1->getRadiusInMetersFromPixels() + circle2->getRadiusInMetersFromPixels() - distance;
    sf::Vector2f separation = -0.5f * overlap * normal;
    circle1->move(separation);
    circle2->move(-separation);

    if (!getGravityState()) {
        circle1->randomizeColor();
        circle2->randomizeColor();
    }
}

void PhysicsEngine::resolveTextCollision(std::shared_ptr<MyCircle> circle, std::shared_ptr<MyText> text)
{
    sf::Vector2f circlePosition = circle->getPositionInMetersFromPixels();
    float circleRadius = circle->getRadiusInMetersFromPixels();

    sf::Vector2f textPosition = text->_text->getPosition() / Settings::getConversionFactor();
    sf::Vector2f textSize = sf::Vector2f(text->_text->getGlobalBounds().width / Settings::getConversionFactor(), text->_text->getGlobalBounds().height / Settings::getConversionFactor());

    // Calculate the corners of the text's bounding box
    sf::Vector2f textTopLeft = textPosition - textSize * 0.5f;
    sf::Vector2f textBottomRight = textPosition + textSize * 0.5f;

    // Calculate the closest point within the text's bounding box to the circle center
    float closestX = std::clamp(circlePosition.x, textTopLeft.x, textBottomRight.x);
    float closestY = std::clamp(circlePosition.y, textTopLeft.y, textBottomRight.y);

    // Calculate the normal vector from the collision point to the circle center
    sf::Vector2f collisionNormal = circlePosition - sf::Vector2f(closestX, closestY);
    float collisionNormalLength = std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
    collisionNormal /= collisionNormalLength;

    // Calculate the reflection of the circle's velocity
    sf::Vector2f circleVelocity = circle->getVelocity();
    sf::Vector2f reflectionVelocity = circleVelocity - 2.1f * dot(circleVelocity, collisionNormal) * collisionNormal;

    // Apply the reflected velocity to the circle
    circle->setVelocity(reflectionVelocity);

    // Apply VFX to text
    text->quickFlash();
    //text->setColor(circle->_circle->getFillColor());
}


void PhysicsEngine::toggleGravity()
{
    this->_gravity = !_gravity;
}

bool PhysicsEngine::getGravityState()
{
    return this->_gravity;
}

void PhysicsEngine::applyGravity(std::shared_ptr<MyCircle> circle, float subDt)
{
    sf::Vector2f scaledGrav(_gravityValue * subDt);
    circle->accelerate(scaledGrav);
}
void PhysicsEngine::checkBounds(std::shared_ptr<MyCircle> circle, float timeStep)
{
    // X bounds
    // Left
    if (circle->getPositionInMetersFromPixels().x < circle->getRadiusInMetersFromPixels()) 
    {
        circle->setPositionFromMetersToPixels(circle->getRadiusInMetersFromPixels(), circle->getPositionInMetersFromPixels().y);
        circle->invertXVelocity();
    }
    // Right
    else if (circle->getPositionInMetersFromPixels().x > _width - circle->getRadiusInMetersFromPixels())
    {
        circle->setPositionFromMetersToPixels(_width - circle->getRadiusInMetersFromPixels(), circle->getPositionInMetersFromPixels().y);
        circle->invertXVelocity();
    }

    // Y bounds
    // Top (disabled on intro)
    if (GameManager::isIntroFinished())
    {
        if (circle->getPositionInMetersFromPixels().y < circle->getRadiusInMetersFromPixels())
        {
            circle->setPositionFromMetersToPixels(circle->getPositionInMetersFromPixels().x, circle->getRadiusInMetersFromPixels() + 0.01f);
            circle->invertYVelocity();
        }
    }
    // Bottom
    if (circle->getPositionInMetersFromPixels().y > _height - circle->getRadiusInMetersFromPixels())
    {
        circle->setPositionFromMetersToPixels(circle->getPositionInMetersFromPixels().x, _height - circle->getRadiusInMetersFromPixels());
        circle->invertYVelocity();
    }
}

sf::Vector2f PhysicsEngine::normalize(const sf::Vector2f& vector) {
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length != 0.0f) {
        return sf::Vector2f(vector.x / length, vector.y / length);
    }
    else {
        return sf::Vector2f(0.0f, 0.0f);
    }
}
float PhysicsEngine::dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2) 
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}
float PhysicsEngine::length(const sf::Vector2f& vector) 
{
    return std::sqrt((vector.x * vector.x + vector.y * vector.y));
}
