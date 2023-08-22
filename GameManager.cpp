#include "GameManager.h"
#include "MyText.h"

GameManager::GameManager(sf::RenderWindow* window) :
    _window(window),
    _circs(),
    _texts(),
    _frames(0),
    _oldTime(0.0f),
    _newTime(0.0f)

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
        //showFps();
        draw();
    }
}

void GameManager::handleInput()
{
    sf::Event event;
    while (_window->pollEvent(event)) 
    {
        if (event.type == event.KeyPressed && event.key.code==sf::Keyboard::Escape) 
        {
            _window->close();
        }
        else if (event.type == event.MouseButtonPressed && event.mouseButton.button== sf::Mouse::Left) 
        {
            for (auto& tPtr : _texts) 
            {
                tPtr->fadeOut();
            }
            sf::Vector2f mousePosF((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);
            _circs.emplace_back(std::make_unique<MyCircle>(mousePosF));
        }
        else if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::X)
        {
            _circs.clear();
        }
    }
    
}

void GameManager::update(float dt)

{
    if (!_circs.empty()) 
    {
        for (auto& circlePtr : _circs)
        {
            MyCircle& circle = *circlePtr;
            circle.updateColor();
            checkForCollisions();
            circle.updateMovement(dt);
            checkOutOfB();
        }
    }
    if (!_texts.empty()) 
    {
        for (auto& textPtr : _texts) 
        {
            MyText& text = *textPtr;
            text.updateText(); 
            if (text.isSafeToRemove()) 
            {
                
            }
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
            _window->draw(t);
        }
    }
    if (!_circs.empty()) 
    {
        for (auto& cPtr : _circs)
        {
            MyCircle& c = *cPtr;
            _window->draw(c);
        }
    }
    _window->display();
}


void GameManager::checkForCollisions()
{
    for (auto& circlePtr : _circs)
    {
        for (auto& circle2Ptr : _circs)
        {
            MyCircle& circle = *circlePtr;
            MyCircle& circle2 = *circle2Ptr;
            if (&circle != &circle2)
            {
                if (circle.isIntersect(circle2))
                {
                    circle.resolveIntersections(circle2);
                }
            }
        }
    }
}

void GameManager::checkOutOfB()
{
    float leftBound = 0.0f;
    float topBound = 0.0f;
    float widthBound = static_cast<float>(_window->getSize().x);
    float heightBound = static_cast<float>(_window->getSize().y);

    for (auto& circlePtr : _circs)
    {
        MyCircle& circle = *circlePtr;
        float radius = circle.getRadius();
        sf::Vector2f newPos = circle.getPosition();

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
        circle.setPosition(newPos);
    }
}
void GameManager::showFps() 
{
    _frames++;
    _fpsTimer += _fpsClock.restart();
    _newTime = _fpsTimer.asSeconds();
    float elapsedTime = _newTime - _oldTime;
    if (elapsedTime >= 1.0f) 
    {
        std::cout << "FPS" << "\n";
        //fpsText.setString("FPS: " + std::to_string(static_cast<int>(_frames)));
        _frames = 0;
        _oldTime = _fpsTimer.asSeconds();
        
    }
}
void GameManager::showIntroText() 
{
    const std::string& intr = "LEFT CLICK - SPAWN CIRCLES";
    const std::string& intr1 = "RIGHT CLICK - DESPAWN CIRCLES";
    const std::string& intr2 = "SPACEBAR - TOGGLE GRAVITY";
    const std::string& intr3 = "X - CLEAR WINDOW";
    const std::string& intr4 = "INTERACTIVE PHYSICS SIMULATION BY RUSLAN LIBIN (https://github.com/RusLanParty)";
    _texts.emplace_back(std::make_unique<MyText>(intr4, _window->getSize().x / 2, _window->getSize().y / 2, _texts, sf::Color::White));
    _texts.emplace_back(std::make_unique<MyText>(intr, _window->getSize().x / 2, _window->getSize().y / 2 + 500.0f, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr1, _window->getSize().x / 2, _window->getSize().y / 2 + 550.0f, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr2, _window->getSize().x / 2, _window->getSize().y / 2 + 600.0f, _texts));
    _texts.emplace_back(std::make_unique<MyText>(intr3, _window->getSize().x / 2, _window->getSize().y / 2 + 650.0f, _texts));
}