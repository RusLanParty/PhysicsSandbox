#include "GameManager.h"
#include "MyText.h"
#include "FPSCounter.h"
static bool introComplete = false;
bool isMousePressed = false;
float timeSinceLastSpawn = 0.1f;
float spawnInterval = 0.1f;
uint32_t spwnCount = 0;
int OBJCOUNT = 0;


GameManager::GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font) :
    _window(window),
    _physicsEngine(),
    _font(font),
    _fpsCounter(0.0f, 0.0f, 100.0f, 0.0f, *font),
    _circs(),
    _texts()
{
    float offsetXPercent = 0.05f;
    float offsetYPercent = 0.05f;

    _width = _window->getSize().x / Settings::getConversionFactor();
    _height = _window->getSize().y / Settings::getConversionFactor(); 

    _offsetX = _width * offsetXPercent;
    _offsetY = _height * offsetYPercent;
    
    showIntro();
    std::cout << "game created" << "\n";
    run();    
}

bool GameManager::isIntroFinished()
{
    return introComplete;
}

int GameManager::getObjectCount()
{
    return OBJCOUNT;
}

void GameManager::run()
{
    while (_window->isOpen()) 
    {
        _dt = _clock.restart();
        float deltaTime = _dt.asSeconds();
        if (!introComplete) { intro(deltaTime); }

        handleInput(deltaTime);
        update(deltaTime);
        draw(deltaTime);
        disposeTrash();
    }
}

void GameManager::keyboardInput(float deltaTime, sf::Event event)
{
        if (event.type == event.KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                _window->close();
            }
            else if (event.key.code == sf::Keyboard::X)
            {
                _circs.clear();
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                for (int i = 0; i < 20; i++)
                {
                    _circs.emplace_back(std::make_unique<MyCircle>());
                }
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                for (auto& circPtr : _circs) 
                {
                    circPtr->randomizeVelocity();
                    std::string state = "SHAKE";
                    _texts.emplace_back(std::make_unique<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Green, true));
                }
            }
            else if (event.key.code == sf::Keyboard::G)
            {
                _physicsEngine.toggleGravity();
                if (_physicsEngine.getGravityState())
                {
                    std::string state = "GRAVITY ENABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Green, true));
                }
                else
                {
                    std::string state = "GRAVITY DISABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Red, true));
                }
            }
        }
}

void GameManager::mouseInput(float deltaTime, sf::Event event)
{
        if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            isMousePressed = true;
        }
        else if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            isMousePressed = false;

        }
        if (isMousePressed)
        {
            timeSinceLastSpawn += deltaTime;

            // Check if enough time has passed to spawn a new circle
            if (timeSinceLastSpawn >= spawnInterval)
            {
                sf::Vector2f mousePosF((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);
                _circs.emplace_back(std::make_unique<MyCircle>(mousePosF));
                timeSinceLastSpawn = 0.0f;
            }
        }
}

void GameManager::disposeTrash()
{
    // Text
    auto iteratorToRemove = std::remove_if(_texts.begin(), _texts.end(),
        [](std::unique_ptr<MyText>& textPtr) {
            return textPtr->isSafeToRemove();
        });

    for (auto it = iteratorToRemove; it != _texts.end(); ++it) {
        it->reset(); // Release the object pointed to by the shared_ptr
    }

    _texts.erase(iteratorToRemove, _texts.end()); // Erase the removed elements
}

void GameManager::handleInput(float deltaTime)
{
    sf::Event event;
    while (_window->pollEvent(event))
    {
        keyboardInput(deltaTime, event);
        mouseInput(deltaTime, event);
    }
}
    
void GameManager::update(float deltaTime)
{
    OBJCOUNT = _circs.size();
    if (!_circs.empty())
    {
        for (size_t i = 0; i < _circs.size(); ++i)
        {
            _physicsEngine.applyPhysics(_circs[i], deltaTime);
            _circs[i]->updateColor(deltaTime);
            for (size_t j = i + 1; j < _circs.size(); ++j)
            {
                if (_circs[i]->isIntersect(*_circs[j]))
                {
                    _physicsEngine.resolveCollision(_circs[i], _circs[j]);
                }
            }

            // Text intersections
            if (!_texts.empty())
            {
                for (auto& textPtr : _texts)
                {
                    if (textPtr->getCollisionsState() && textPtr->isIntersect(_circs[i]))
                    {
                        _physicsEngine.resolveTextIntersections(_circs[i], *textPtr);
                    }
                }
            }
        }
    }

    if (!_texts.empty())
    {
        for (auto& textPtr : _texts)
        {
            textPtr->updateText(deltaTime);
        }
    }
}

void GameManager::draw(float deltaTime)
{
    _window->clear(sf::Color::Black);
    _physicsEngine.drawBound(_window);
    if (!_texts.empty())
    {
        for (auto& tPtr : _texts)
        {
            MyText& t = *tPtr;
            _window->draw(*t._text);
        }
    }
    if (!_circs.empty()) 
    {
        for (auto& cPtr : _circs)
        {
            MyCircle& c = *cPtr;
            _window->draw(*c._circle);
        }
    }
    _fpsCounter.displayFps(deltaTime);
    _fpsCounter.displayOjbectCount();
    _fpsCounter.draw(_window);
    _window->display();
}


void GameManager::showIntro() 
{
    const std::string& intr = "LEFT CLICK - SPAWN CIRCLES";
    const std::string& intr1 = "RIGHT CLICK - DESPAWN CIRCLES";
    const std::string& intr2 = "G - TOGGLE GRAVITY";
    const std::string& intr3 = "X - CLEAR WINDOW";
    const std::string& intr4 = "PHYSICS SANDBOX BY RUSLAN LIBIN (https://github.com/RusLanParty)";
    

    // Intro text
    _texts.emplace_back(std::make_unique<MyText>(intr4, _width / 2, _height - 10 * _offsetY, *_font, _texts, sf::Color::White));
    _texts.emplace_back(std::make_unique<MyText>(intr, _width / 2, _height - 4 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr1, _width / 2, _height - 3 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr2, _width / 2, _height - 2 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr3, _width / 2, _height - 1 * _offsetY, *_font, _texts));
}

bool GameManager::inBoundY()
{
    for (auto& circPtr : _circs)
    {
        if (circPtr->getPositionInMetersFromPixels()->y < circPtr->getRadiusInMetersFromPixels())
        {
            return false;
        }
    }
    return true;
}
void GameManager::intro(float deltaTime)
{
        // How many circles to spawn
        uint32_t quantity = 100;
        float offsetY = _height * 0.05f;
        float offsetX = _width / quantity;

        timeSinceLastSpawn += deltaTime;

        // Check if enough time has passed to spawn a new circle
        if (timeSinceLastSpawn >= spawnInterval)
        {
            if (spwnCount < quantity)
            {
                sf::Vector2f top;
                // For the first circle, spawn a bit further from the left edge
                if (spwnCount == 0)
                {
                    top.x = offsetX * spwnCount + offsetX * Settings::getConversionFactor(); 
                }
                // For the last circle, spawn a bit further from the right edge
                else if (spwnCount == quantity - 1)
                {
                    top.x = offsetX * spwnCount - offsetX * 0.55f * Settings::getConversionFactor(); 
                }
                else
                {
                    top.x = offsetX * spwnCount * Settings::getConversionFactor();
                }

                top.y = -offsetY * 2;
                _circs.emplace_back(std::make_unique<MyCircle>(top));
                timeSinceLastSpawn = 0.0f;
                spwnCount++;
            }
            else
            {
               if (inBoundY())
               {
                   for (auto& tPtr : _texts)
                   {    
                   tPtr->fadeOut();   
                   }
                   spawnInterval = 0.07f;
                   timeSinceLastSpawn = 0.07f;
                   _physicsEngine.toggleGravity();

                   introComplete = true;

                   if (_physicsEngine.getGravityState())
                   {
                       std::string state = "GRAVITY ENABLED";
                       _texts.emplace_back(std::make_unique<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Green, true));
                       for (auto& circPtr : _circs)
                       {
                           circPtr->randomizeColor();
                       }
                   }
                   else
                   {
                       std::string state = "GRAVITY DISABLED";
                       _texts.emplace_back(std::make_unique<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Red, true));
                       for (auto& circPtr : _circs) 
                       {
                           circPtr->randomizeColor();
                       }

                   }
               }
                
            }
        }
}
