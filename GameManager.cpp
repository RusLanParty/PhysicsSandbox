#include "GameManager.h"
#include "MyText.h"
#include "FPSCounter.h"

bool GameManager::isIntro = false;
bool isMousePressed = false;
float timeSinceLastSpawn = 0.1f;
float spawnInterval = 0.1f;
uint32_t spwnCount = 0;

GameManager::GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font) :
    _window(window),
    _physicsEngine(),
    _font(font),
    _fpsCounter(0.0f, 0.0f, *font),
    _circs(),
    _texts()
{
    showIntro();
    std::cout << "game created" << "\n";
    run();    
}

bool GameManager::getIntroState()
{
    return isIntro;
}

void GameManager::run()
{
    while (_window->isOpen()) 
    {
        _dt = _clock.restart();
        float deltaTime = _dt.asSeconds();
        if (isIntro) { intro(deltaTime); }

        handleInput(deltaTime);
        update(deltaTime);
        _fpsCounter.displayFps(deltaTime);
        draw();
    }
}

void GameManager::handleInput(float deltaTime)
{
    sf::Event event;
    while (_window->pollEvent(event)) 
    {
        if (event.type == event.KeyPressed) 
        {
            if (event.key.code == sf::Keyboard::Escape) 
            {
                _window->close();
            }
            else if (event.key.code == sf::Keyboard::X)
            {
                for (auto& tPtr : _texts)
                {
                    tPtr->fadeOut();
                }
                _circs.clear();
            }
            else if (event.key.code == sf::Keyboard::S) 
            {
                for (auto& tPtr : _texts)
                {
                    tPtr->fadeOut();
                }
                for (int i = 0; i < 20; i++) 
                {
                    _circs.emplace_back(std::make_unique<MyCircle>());
                }
            }
            else if (event.key.code == sf::Keyboard::G) 
            {
                for (auto& tPtr : _texts)
                {
                    tPtr->fadeOut();
                }
                _physicsEngine.toggleGravity();
            }
        }
        else if (event.type == event.MouseButtonPressed && event.mouseButton.button== sf::Mouse::Left) 
        {
            for (auto& tPtr : _texts) 
            {
                tPtr->fadeOut();
            }
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
    
}
void GameManager::update(float deltaTime)
{
    if (!_circs.empty())
    {
        for (auto& circlePtr : _circs)
        {
            _physicsEngine.applyPhysics(circlePtr, deltaTime);
            circlePtr->updateColor(deltaTime);
        }

        for (size_t i = 0; i < _circs.size(); ++i)
        {
            for (size_t j = i + 1; j < _circs.size(); ++j)
            {
                if (_circs[i]->isIntersect(*_circs[j]))
                {
                    _physicsEngine.resolveIntersections(_circs[i], *_circs[j]);
                }
            }

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

void GameManager::draw()
{
    _window->clear(sf::Color::Black);
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
    _fpsCounter.draw(_window);
    _window->display();
}


void GameManager::showIntro() 
{
    const std::string& intr = "LEFT CLICK - SPAWN CIRCLES";
    const std::string& intr1 = "RIGHT CLICK - DESPAWN CIRCLES";
    const std::string& intr2 = "SPACEBAR - TOGGLE GRAVITY";
    const std::string& intr3 = "X - CLEAR WINDOW";
    const std::string& intr4 = "PHYSICS SANDBOX BY RUSLAN LIBIN (https://github.com/RusLanParty)";
    float width = _window->getSize().x;
    float height = _window->getSize().y;

    float offsetXPercent = 0.05f;
    float offsetYPercent = 0.05f; 

    float offsetX = width * offsetXPercent;
    float offsetY = height * offsetYPercent;

    // Intro text
    _texts.emplace_back(std::make_unique<MyText>(intr4, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::White));
    _texts.emplace_back(std::make_unique<MyText>(intr, width / 2, height - 4 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr1, width / 2, height - 3 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr2, width / 2, height - 2 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr3, width / 2, height - 1 * offsetY, *_font, _texts));

    // Spawn circles
    isIntro = true;
}

void GameManager::intro(float deltaTime)
{
    float width = _window->getSize().x;
    float height = _window->getSize().y;

    // How many circles to spawn
    uint32_t quantity = 40;

    float offsetY = height * 0.05f;
    float offsetX = width / quantity;

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
                top.x = offsetX * spwnCount + offsetX * 0.55f; // Adjust the factor as needed
            }
            // For the last circle, spawn a bit further from the right edge
            else if (spwnCount == quantity - 1)
            {
                top.x = offsetX * spwnCount - offsetX * 0.55f; // Adjust the factor as needed
            }
            else
            {
                top.x = offsetX * spwnCount;
            }

            top.y = -offsetY;

            _circs.emplace_back(std::make_unique<MyCircle>(top));
            timeSinceLastSpawn = 0.0f;
            spwnCount++;
        }
        else
        { 
            isIntro = false; 
            spawnInterval = 0.08f;
            timeSinceLastSpawn = 0.08f;
        }
    }
}
