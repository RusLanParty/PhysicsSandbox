#include "GameManager.h"
#include "MyText.h"
#include "FPSCounter.h"

GameManager::GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font) :
    _window(window),
    _physicsEngine(window),
    _font(font),
    _fpsCounter(0.0f, 0.0f, *font),
    _circs(),
    _texts()
{
    showIntroText();
    std::cout << "game created" << "\n";
    run();    
}

void GameManager::run()
{
    while (_window->isOpen()) 
    {
        _dt = _clock.restart();
        float deltaTime = _dt.asSeconds();

        handleInput();
        update(deltaTime);
        _fpsCounter.displayFps();
        draw();
        
    }
}

void GameManager::handleInput()
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
            if (_physicsEngine.getGravityState()) 
            {
                _circs.emplace_back(std::make_unique<MyCircle>());
            }
            else 
            {
                sf::Vector2f mousePosF((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);
                _circs.emplace_back(std::make_unique<MyCircle>(mousePosF));
            }
        }
    }
    
}
void GameManager::update(float dt)
{
    if (!_circs.empty())
    {
        for (auto& circlePtr : _circs)
        {
            _physicsEngine.applyPhysics(circlePtr, dt);
            circlePtr->updateColor();
            for (auto& circle2Ptr : _circs)
            {
                if (&circlePtr != &circle2Ptr)
                {
                    if (circlePtr->isIntersect(*circle2Ptr))
                    {
                         _physicsEngine.resolveIntersections(circlePtr, *circle2Ptr);
                    }
                }
            }
        }
    }
    if (!_texts.empty()) 
    {
        for (auto& textPtr : _texts) 
        {
            textPtr->updateText(); 
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


void GameManager::showIntroText() 
{
    const std::string& intr = "LEFT CLICK - SPAWN CIRCLES";
    const std::string& intr1 = "RIGHT CLICK - DESPAWN CIRCLES";
    const std::string& intr2 = "SPACEBAR - TOGGLE GRAVITY";
    const std::string& intr3 = "X - CLEAR WINDOW";
    const std::string& intr4 = "INTERACTIVE PHYSICS SIMULATION BY RUSLAN LIBIN (https://github.com/RusLanParty)";
    float width = _window->getSize().x;
    float height = _window->getSize().y;

    float offsetXPercent = 0.05f;
    float offsetYPercent = 0.05f; 

    float offsetX = width * offsetXPercent;
    float offsetY = height * offsetYPercent;

    _texts.emplace_back(std::make_unique<MyText>(intr4, width / 2, height - 15 * offsetY, *_font, _texts, sf::Color::White));
    _texts.emplace_back(std::make_unique<MyText>(intr, width / 2, height - 4 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr1, width / 2, height - 3 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr2, width / 2, height - 2 * offsetY, *_font, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr3, width / 2, height - 1 * offsetY, *_font, _texts));
}
