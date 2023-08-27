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


std::shared_ptr<sf::Vector2f> PhysicsEngine::_gravityValue = std::make_unique<sf::Vector2f>(0.0f, 9.81f);
bool PhysicsEngine::_gravity = true;

PhysicsEngine::PhysicsEngine()

{
    Settings::setConversionFactor(200.0f);
    sf::VideoMode windowRes = sf::VideoMode::getDesktopMode();
    _width = static_cast<float>(windowRes.width) / Settings::getConversionFactor();
    _height = static_cast<float>(windowRes.height) / Settings::getConversionFactor();
    //_bound = std::make_unique<sf::CircleShape>(5.0f * Settings::getConversionFactor());
   // _bound->setFillColor(sf::Color::Black);
    //_bound->setOrigin(_bound->getRadius(), _bound->getRadius());
    //_bound->setPosition(_width / 2 * Settings::getConversionFactor(), _height / 2 * Settings::getConversionFactor());
    //std::cout << "X= " << _bound->getPosition().x / Settings::getConversionFactor() << " Y= " << _bound->getPosition().y / Settings::getConversionFactor() << "\n";
}

void PhysicsEngine::applyPhysics(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    const uint32_t sub_step = 8;
    float sub_dt = deltaTime / (float)sub_step;

    // Updating (sub_step) times between each frame to increase precision
    for (int i = 0; i < sub_step - 1; i++)
    {
        // Applying all forces
        if (_gravity)
        {
            applyGravity(circle, sub_dt);
        }

        // Performing verlet integration, using acceleration accumulated from the forces applied above
        updatePosition(circle, sub_dt);

        // Checking bounds and handling collisions with them
        checkBounds(circle, sub_dt);
    }
}
void PhysicsEngine::updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    // Verlet Integration: nextPosition = 2 * currentPosition - previousPosition + acceleration * deltaTime
    std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(2.0f * *circle->getPositionInMetersFromPixels() - *circle->getOldPos() + *circle->getAcceleration() * deltaTime);

    // Update previous position with current
    circle->setOldPos(circle->getPositionInMetersFromPixels());
    circle->setPositionFromMetersToPixels(newPos);

    // Reset acceleration
    circle->resetAcceleration();
}
void PhysicsEngine::drawBound(sf::RenderWindow* window)
{
    //window->draw(*_bound);
}
void PhysicsEngine::resolveIntersections(std::shared_ptr<MyCircle> circle, MyCircle& other)
{
    sf::Vector2f delta = *other.getPositionInMetersFromPixels() - *circle->getPositionInMetersFromPixels();
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance < circle->getRadiusInMetersFromPixels() + other.getRadiusInMetersFromPixels())
    {
        float overlap = (circle->getRadiusInMetersFromPixels() + other.getRadiusInMetersFromPixels()) - distance;
        sf::Vector2f normal = delta / distance;

        sf::Vector2f newVelocity1 = *circle->getAcceleration() + normal * overlap * 0.1f;
        sf::Vector2f newVelocity2 = *other.getAcceleration() - normal * overlap * 0.1f;

        circle->accelerate(newVelocity1.x, newVelocity1.y);
        other.accelerate(newVelocity2.x, newVelocity2.y);

        std::shared_ptr<sf::Vector2f> circlePos = std::make_shared<sf::Vector2f>(*circle->getPositionInMetersFromPixels() - normal * overlap * 0.1f);
        std::shared_ptr<sf::Vector2f> otherPos = std::make_shared<sf::Vector2f>(*other.getPositionInMetersFromPixels() + normal * overlap * 0.1f);
        circle->setPositionFromMetersToPixels(circlePos);
        other.setPositionFromMetersToPixels(otherPos);

        std::shared_ptr<sf::Vector2f> newVel1 = std::make_shared<sf::Vector2f>(*other.getAcceleration());
        std::shared_ptr<sf::Vector2f> newVel2 = std::make_shared<sf::Vector2f>(*circle->getAcceleration());

        circle->accelerate(newVel1);
        other.accelerate(newVel2);
        if(!this->getGravityState())
        {
            other.randomizeColor();
            circle->randomizeColor();
        }
    }
}

void PhysicsEngine::resolveTextIntersections(std::shared_ptr<MyCircle> circle, MyText& text)
{
    float bounciness = 0.08f;
    sf::Vector2f circleCenter = *circle->getPositionInMetersFromPixels();
    sf::Vector2f textPosition = text._text->getPosition() / Settings::getConversionFactor();
    sf::FloatRect globalBounds = text._text->getGlobalBounds();
    float globalWidth = globalBounds.width / Settings::getConversionFactor();
    float globalHeight = globalBounds.height / Settings::getConversionFactor();

    // Calculate the closest point on the rectangle to the circle
    sf::Vector2f closestPoint;
    closestPoint.x = std::max(textPosition.x - globalWidth / 2.0f,
        std::min(circleCenter.x, textPosition.x + globalWidth / 2.0f));
    closestPoint.y = std::max(textPosition.y - globalHeight / 2.0f,
        std::min(circleCenter.y, textPosition.y + globalHeight / 2.0f));

    // Calculate displacement between the circle's center and the closest point on the rectangle
    sf::Vector2f displacement = circleCenter - closestPoint;
    float distance = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);

    // Check if collision has occurred
    if (distance < circle->getRadiusInMetersFromPixels())
    {
        // Calculate overlap and correction
        float overlap = circle->getRadiusInMetersFromPixels() - distance;
        sf::Vector2f correction = (displacement / distance) * overlap;

        // Adjust circle's current and previous positions to resolve overlap
        circleCenter += correction;  // Apply correction to current position
        sf::Vector2f oldCircleCenter = *circle->getOldPos();

        // Reflect the position based on bounciness
        sf::Vector2f newCircleCenter = circleCenter + bounciness * (circleCenter - oldCircleCenter);

        // Calculate acceleration (change in position)
        sf::Vector2f acceleration = newCircleCenter - circleCenter;

        // Set the new position for the circle using Verlet integration
       std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(2.0f * newCircleCenter - circleCenter + acceleration);
        circle->setPositionFromMetersToPixels(newPos);

        // Store the current position as previous position for the next frame
        circle->setOldPos(std::make_shared<sf::Vector2f>(circleCenter));
    }
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
    std::shared_ptr<sf::Vector2f> scaledGrav = std::make_shared<sf::Vector2f>(*_gravityValue * subDt);
    circle->accelerate(scaledGrav);
}
void PhysicsEngine::checkBounds(std::shared_ptr<MyCircle> circle, float timeStep)
{
    std::shared_ptr<sf::Vector2f> newPos = std::make_shared<sf::Vector2f>(*circle->getPositionInMetersFromPixels());

    // X bounds
    if (newPos->x >= _width - circle->getRadiusInMetersFromPixels())
    {
        newPos->x = _width - circle->getRadiusInMetersFromPixels();

        // Reflect y velocity and adjust position to create bounce effect
        sf::Vector2f velocity = *circle->getPositionInMetersFromPixels() - *circle->getOldPos();
        velocity.y = -velocity.y;
        newPos->y -= 0.2f * (circle->getRadiusInMetersFromPixels() - (_width - newPos->x));

        // Update old position and set new position
        *circle->getOldPos() = *circle->getPositionInMetersFromPixels();
        circle->setPositionFromMetersToPixels(newPos);
    }
    else if (newPos->x <= circle->getRadiusInMetersFromPixels())
    {
        newPos->x = circle->getRadiusInMetersFromPixels();

        // Reflect y velocity and adjust position to create bounce effect
        sf::Vector2f velocity = *circle->getPositionInMetersFromPixels() - *circle->getOldPos();
        velocity.y = -velocity.y;
        newPos->y -= 0.2f * (circle->getRadiusInMetersFromPixels() - newPos->x);

        // Update old position and set new position
        *circle->getOldPos() = *circle->getPositionInMetersFromPixels();
        circle->setPositionFromMetersToPixels(newPos);
    }

    // Y bounds
    if (newPos->y >= _height - circle->getRadiusInMetersFromPixels())
    {
        newPos->y = _height - circle->getRadiusInMetersFromPixels();

        // Reflect x velocity and adjust position to create bounce effect
        sf::Vector2f velocity = *circle->getPositionInMetersFromPixels() - *circle->getOldPos();
        velocity.x = -velocity.x;
        newPos->x -= 0.2f * (circle->getRadiusInMetersFromPixels() - (_height - newPos->y));

        // Update old position and set new position
        *circle->getOldPos() = *circle->getPositionInMetersFromPixels();
        circle->setPositionFromMetersToPixels(newPos);
    }
    else if (GameManager::getIntroState())
    {
        if (newPos->y <= circle->getRadiusInMetersFromPixels())
        {
            newPos->y = circle->getRadiusInMetersFromPixels();

            // Reflect x velocity and adjust position to create bounce effect
            sf::Vector2f velocity = *circle->getPositionInMetersFromPixels() - *circle->getOldPos();
            velocity.x = -velocity.x;
            newPos->x -= 0.2f * (circle->getRadiusInMetersFromPixels() - newPos->y);

            // Update old position and set new position
            *circle->getOldPos() = *circle->getPositionInMetersFromPixels();
            circle->setPositionFromMetersToPixels(newPos);
        }
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
    float conversionFactor = Settings::getConversionFactor();
    return std::sqrt((vector.x * vector.x + vector.y * vector.y) * conversionFactor * conversionFactor);
}