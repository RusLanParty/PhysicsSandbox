#include "GameManager.h"
#include "MyText.h"
#include "FPSCounter.h"
static bool introComplete = false;
bool isMousePressed = false;
float timeSinceLastSpawn = 0.3f;
float spawnInterval = 0.3f;
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
    showIntro();
    std::cout << "game created" << "\n";
    run();    
}

bool GameManager::getIntroState()
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
            else if (event.key.code == sf::Keyboard::G)
            {
                _physicsEngine.toggleGravity();
                float width = _window->getSize().x;
                float height = _window->getSize().y;

                float offsetY = height * 0.05f;
                float offsetX = width * 0.05f;
                if (_physicsEngine.getGravityState())
                {
                    std::string state = "GRAVITY ENABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::Green, true));
                }
                else
                {
                    std::string state = "GRAVITY DISABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::Red, true));
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
                    _physicsEngine.resolveIntersections(_circs[i], *_circs[j]);
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
    _texts.emplace_back(std::make_unique<MyText>(intr4, width / 2, height - 10 * offsetY, *_font, _texts, sf::Color::White));
    _texts.emplace_back(std::make_unique<MyText>(intr, width / 2, height - 4 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr1, width / 2, height - 3 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr2, width / 2, height - 2 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr3, width / 2, height - 1 * offsetY, *_font, _texts));
}

void GameManager::intro(float deltaTime)
{
        float width = _window->getSize().x;
        float height = _window->getSize().y;

        // How many circles to spawn
        uint32_t quantity = 50;

        float offsetY = height * 0.05f;
        float offsetX = width / quantity;

        timeSinceLastSpawn += deltaTime;

        // Check if enough time has passed to spawn a new circle
        if (timeSinceLastSpawn >= spawnInterval)
        {
            if (spwnCount < quantity)
            {
                sf::Vector2f top;
                top.x = offsetX * spwnCount;
                top.y = -offsetY / 2;

                _circs.emplace_back(std::make_unique<MyCircle>(top));
                timeSinceLastSpawn = 0.0f;
                spwnCount++;
            }
            else
            {
                for (auto& tPtr : _texts)
                {
                    tPtr->fadeOut();
                }
                spawnInterval = 0.07f;
                timeSinceLastSpawn = 0.07f;

                _physicsEngine.toggleGravity();

                float width = _window->getSize().x;
                float height = _window->getSize().y;

                float offsetY = height * 0.05f;
                float offsetX = width * 0.05f;

                introComplete = true;

                if (_physicsEngine.getGravityState())
                {
                    std::string state = "GRAVITY ENABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::Green, true));
                }
                else
                {
                    std::string state = "GRAVITY DISABLED";
                    _texts.emplace_back(std::make_unique<MyText>(state, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::Red, true));
                }
            }
        }
}
