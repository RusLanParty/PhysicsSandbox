#include "PhysicsEngine.h"
std::shared_ptr<sf::Vector2f> PhysicsEngine::_gravityValue = std::make_unique<sf::Vector2f>(0.0f, 1000.81f);
bool PhysicsEngine::_gravity = false;

PhysicsEngine::PhysicsEngine(sf::RenderWindow* window):
_window(window)
{
}

void PhysicsEngine::applyPhysics(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    const uint32_t sub_step = 2;
    const float sub_dt = deltaTime / (float)sub_step;
    for (int i = sub_step; i >= 0; i--) 
    {
        if (_gravity)
        {
            applyGravity(circle);
        }
        bounds(_window, circle);
        updatePosition(circle, sub_dt);
    }
}
void PhysicsEngine::updatePosition(std::shared_ptr<MyCircle> circle, float deltaTime)
{
    if (circle->getOldPos()->x == 0.0f && circle->getOldPos()->y == 0.0f)
    {
            sf::Vector2f newPos = circle->_circle->getPosition() + *circle->getAcceleration();
            circle->setOldPos(circle->getPosition());
            circle->_circle->setPosition(newPos);
    }
    else
    {
        sf::Vector2f newPos = 2.0f * circle->_circle->getPosition() - *circle->getOldPos() + *circle->getAcceleration() * (deltaTime * deltaTime);
        circle->setOldPos(circle->getPosition());
        circle->_circle->setPosition(newPos);
    }
    bbox(circle);
    circle->resetAcceleration();
}
void PhysicsEngine::resolveIntersections(std::shared_ptr<MyCircle> circle, MyCircle& other)
{
    sf::Vector2f delta = other._circle->getPosition() - circle->_circle->getPosition();
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance < circle->_circle->getRadius() + other._circle->getRadius())
    {
        float overlap = (circle->_circle->getRadius() + other._circle->getRadius()) - distance;
        sf::Vector2f normal = delta / distance;

        sf::Vector2f newVelocity1 = *circle->getAcceleration() + normal * overlap * 0.01f;
        sf::Vector2f newVelocity2 = *other.getAcceleration() - normal * overlap * 0.01f;

        circle->accelerate(newVelocity1.x, newVelocity1.y);
        other.accelerate(newVelocity2.x, newVelocity2.y);

        circle->_circle->setPosition(circle->_circle->getPosition() - normal * overlap * 0.1f);
        other._circle->setPosition(other._circle->getPosition() + normal * overlap * 0.1f);

        std::shared_ptr<sf::Vector2f> newVel1 = std::make_shared<sf::Vector2f>(*other.getAcceleration());
        std::shared_ptr<sf::Vector2f> newVel2 = std::make_shared<sf::Vector2f>(*circle->getAcceleration());
        circle->accelerate(newVel1);
        other.accelerate(newVel2);
        other.randomizeColor();
        circle->randomizeColor();
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

void PhysicsEngine::applyGravity(std::shared_ptr<MyCircle> circle)
{

    circle->accelerate(_gravityValue);
    std::cout << "accX: " << circle->getAcceleration()->x << "accY: " << circle->getAcceleration()->y << "\n";
    //circle->dim();

}
void PhysicsEngine::bounds(sf::RenderWindow* window, std::shared_ptr<MyCircle> circle)
{
    float leftBound = 0.0f;
    float topBound = 0.0f;
    float widthBound = static_cast<float>(window->getSize().x);
    float heightBound = static_cast<float>(window->getSize().y);

        float radius = circle->_circle->getRadius();
        sf::Vector2f newPos = circle->_circle->getPosition();

        // Check and resolve X-axis collision
        if (newPos.x - radius < leftBound)
        {
            newPos.x = leftBound + radius;
        }
        else if (newPos.x + radius > leftBound + widthBound)
        {
            newPos.x = leftBound + widthBound - radius;
        }

        // Check and resolve Y-axis collision
        if (newPos.y - radius < topBound)
        {
            newPos.y = topBound + radius;
        }
        else if (newPos.y + radius > topBound + heightBound)
        {
            newPos.y = topBound + heightBound - radius;
        }

        // Update circle position
        circle->_circle->setPosition(newPos);

    
}
void PhysicsEngine::bbox(std::shared_ptr<MyCircle> circle)
{
    sf::VideoMode res = sf::VideoMode::getDesktopMode();
    int width = res.width;
    int height = res.height;
    if (circle->_circle->getPosition().x >= width - circle->_circle->getRadius())
    {
      circle->setOldPosX(circle->_circle->getPosition().x);
      circle->invertXSpeed();
      circle->randomizeColor();
    }
    else
        if (circle->_circle->getPosition().x <= circle->_circle->getRadius())
        {
            circle->setOldPosX(circle->_circle->getPosition().x);
            circle->invertXSpeed();
            circle->randomizeColor();
        }
        else
            if (circle->_circle->getPosition().y >= height - circle->_circle->getRadius())
            {
                circle->setOldPosY(circle->_circle->getPosition().y);
                circle->invertYSpeed();
                circle->randomizeColor();
            }
    if (circle->_circle->getPosition().y <= circle->_circle->getRadius())
    {
        circle->setOldPosY(circle->_circle->getPosition().y);
        circle->invertYSpeed();
        circle->randomizeColor();
    }
}
