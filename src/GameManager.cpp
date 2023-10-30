#include "GameManager.h"
#include "MyText.h"
#include "FPSCounter.h"
static bool introComplete = false;
bool GameManager::add = false;
bool isMousePressed = false;
bool bloomEnabled = true;
uint32_t quantity = 10;
float timeSinceLastSpawn = 0.05f;
float spawnInterval = 0.05f;
uint32_t spwnCount = 0;
int OBJCOUNT = 0;


GameManager::GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font) :
    _window(window),
    Physics(),
    _font(font),
    _fpsCounter(0.0f, 0.0f, 100.0f, 0.0f, *font),
    _circs(),
    _texts(),
    maxZoom(2.0f)
{
    // Init shaders
    shaders = std::make_shared<Shaders>(window);

    view = _window->getDefaultView();

    float offsetXPercent = 0.05f;
    float offsetYPercent = 0.05f;

    _width = _window->getSize().x / Settings::getConversionFactor();
    _height = _window->getSize().y / Settings::getConversionFactor(); 

    _offsetX = _width * offsetXPercent;
    _offsetY = _height * offsetYPercent;

    renderTexture = std::make_shared<sf::RenderTexture>();
    if (!renderTexture->create(_window->getSize().x, _window->getSize().y))
    {
        std::cout << "GAMEMANAGER: Failed to create RenderTexture" << "\n";
    }
    else
    {
        std::cout << "GAMEMANAGER: RenderTexture created - SUCCESS" << "\n";
        std::cout << "CRITICAL: Ruslan.exe stopped working. Please contact your local Ruslan provider to get a replacement." << "\n";
    }

    showIntro();    
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
        if (!introComplete) 
        {
            intro(deltaTime); 
        }
       
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
            if (event.key.code == sf::Keyboard::B)
            {
                bloomEnabled = !bloomEnabled;
                if (bloomEnabled)
                {
                    std::string state = "BLOOM ENABLED";
                    _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Cyan, true));
                }
                else
                {
                    std::string state = "BLOOM DISABLED";
                    _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Cyan, true));
                }
            }
            else if (event.key.code == sf::Keyboard::M)
            {
                    add = !add;
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                _window->close();
            }
            else if (event.key.code == sf::Keyboard::X)
            {
                _circs.clear();
            }
            else if (event.key.code == sf::Keyboard::H)
            {
                for (int i = 0; i < 20; i++)
                {
                    _circs.emplace_back(std::make_shared<MyCircle>());
                }
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                for (auto& circPtr : _circs) 
                {
                    circPtr->randomizeVelocity();
                    std::string state = "SHAKE";
                    _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Magenta, true));
                }
            }
            else if (event.key.code == sf::Keyboard::G)
            {
                Physics.toggleGravity();
                if (Physics.getGravityState())
                {
                    std::string state = "GRAVITY ENABLED";
                    _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Green, true));
                }
                else
                {
                    std::string state = "GRAVITY DISABLED";
                    _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Red, true));
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    view.move(-10.0f, 0.0f); // Move the view left
                }
                else if (event.key.code == sf::Keyboard::D) {
                    view.move(10.0f, 0.0f); // Move the view right
                }
                else if (event.key.code == sf::Keyboard::W) {
                    view.move(0.0f, -10.0f); // Move the view up
                }
                else if (event.key.code == sf::Keyboard::S) {
                    view.move(0.0f, 10.0f); // Move the view down
                }
            }
        }
}

void GameManager::mouseInput(float deltaTime, sf::Event event)
{
    if (event.type == event.MouseWheelScrolled) 
    {
        sf::Vector2f mousePosF((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);
        view.setCenter(mousePosF);

        // Zooming in/out
        if (event.mouseWheelScroll.delta > 0)
        {
            if (view.getSize().x > _window->getSize().x / maxZoom) {
                view.zoom(0.9f); 
            }
        }
        else if (event.mouseWheelScroll.delta < 0)
        {
            if (view.getSize().x < _window->getSize().x)
            {
                
                view.zoom(1.1f); 
            }
        }   
    }
    if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        isMousePressed = true;
    }
    else if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        isMousePressed = false;

    }
}

void GameManager::disposeTrash()
{
    // Text
    auto iteratorToRemove = std::remove_if(_texts.begin(), _texts.end(),
        [](std::shared_ptr<MyText>& textPtr) {
            return textPtr->isSafeToRemove();
        });

    for (auto it = iteratorToRemove; it != _texts.end(); ++it) {
        it->reset(); 
    }

    _texts.erase(iteratorToRemove, _texts.end()); 
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
    spawnOnMouseClick(deltaTime);
    OBJCOUNT = _circs.size();
    if (!_circs.empty())
    {
        for (size_t i = 0; i < _circs.size(); ++i)
        {
             Physics.applyPhysics(_circs[i], deltaTime);
            _circs[i]->updateColor(deltaTime);
            for (size_t j = i + 1; j < _circs.size(); ++j)
            {
                if (_circs[i]->isIntersect(*_circs[j]))
                {
                    Physics.resolveCollision(_circs[i], _circs[j]);
                }
            }

            // Text intersections
            if (!_texts.empty())
            {
                for (auto& textPtr : _texts)
                {
                    if (textPtr->getCollisionsState() && textPtr->isIntersect(_circs[i]))
                    {
                        Physics.resolveTextCollision(_circs[i], textPtr);
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
    renderTexture->clear(sf::Color::Black);
    //Physics.drawBound(_window);

    if (bloomEnabled) 
    {
        renderTexture->setActive(true);
        
        if (!_texts.empty())
        {
            for (auto& tPtr : _texts)
            {
                MyText& t = *tPtr;
                renderTexture->draw(*t._text);
            }
        }
        if (!_circs.empty())
        {
            for (auto& cPtr : _circs)
            {
                MyCircle& c = *cPtr;
                renderTexture->draw(*c._circle);
            }
        }    
        shaders->applyBloom(renderTexture);
    }
    else 
    {
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
    }

    _fpsCounter.displayFps(deltaTime);
    _fpsCounter.displayOjbectCount();
    _fpsCounter.draw(_window);
    _window->setView(view);
    _window->display();
}


void GameManager::showIntro() 
{
    introComplete = false;
    const std::string& intr = "LEFT CLICK - SPAWN CIRCLES";
    const std::string& intr1 = "B - TOGGLE BLOOM";
    const std::string& intr2 = "G - TOGGLE GRAVITY";
    const std::string& intr3 = "X - CLEAR WINDOW";
    //const std::string& intr4 = "PHYSICS SANDBOX(https://github.com/RusLanParty)";
    

    // Intro text
    //_texts.emplace_back(std::make_shared<MyText>(intr4, _width / 2, _height - 10 * _offsetY, *_font, _texts, sf::Color::White));
    _texts.emplace_back(std::make_shared<MyText>(intr, _width / 2, _height - 4 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_shared<MyText>(intr1, _width / 2, _height - 3 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_shared<MyText>(intr2, _width / 2, _height - 2 * _offsetY, *_font, _texts));
    _texts.emplace_back(std::make_shared<MyText>(intr3, _width / 2, _height - 1 * _offsetY, *_font, _texts));
}

bool GameManager::inBoundY()
{
    for (auto& circPtr : _circs)
    {
        if (circPtr->getPositionInMetersFromPixels().y < circPtr->getRadiusInMetersFromPixels())
        {
            return false;
        }
    }
    return true;
}
void GameManager::spawnOnMouseClick(float deltaTime)
{
    if (isMousePressed)
    {
        timeSinceLastSpawn += deltaTime;

        // Check if enough time has passed to spawn a new circle
        if (timeSinceLastSpawn >= spawnInterval)
        {
            sf::Vector2f mousePosF((float)sf::Mouse::getPosition().x, (float)sf::Mouse::getPosition().y);
            _circs.emplace_back(std::make_shared<MyCircle>(mousePosF));
            timeSinceLastSpawn = 0.0f;
        }
    }
}
void GameManager::intro(float deltaTime)
{
        float offsetY = _height * 0.5f;
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
                    top.x = offsetX * spwnCount + 1 * Settings::getConversionFactor(); 
                }
                // For the last circle, spawn a bit further from the right edge
                else if (spwnCount == quantity - 1)
                {
                    top.x = offsetX * spwnCount - 1 * Settings::getConversionFactor(); 
                }
                else
                {
                    top.x = offsetX * spwnCount * Settings::getConversionFactor();
                }

                top.y = -offsetY * 30;
                _circs.emplace_back(std::make_shared<MyCircle>(top));
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
                   spawnInterval = 0.05f;
                   timeSinceLastSpawn = 0.05f;
                   Physics.toggleGravity();

                   introComplete = true;

                   if (Physics.getGravityState())
                   {
                       std::string state = "GRAVITY ENABLED";
                       _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Green, true));
                       for (auto& circPtr : _circs)
                       {
                           circPtr->randomizeColor();
                       }
                   }
                   else
                   {
                       std::string state = "GRAVITY DISABLED";
                       _texts.emplace_back(std::make_shared<MyText>(state, _width / 2, _height - 15 * _offsetY, *_font, _texts, sf::Color::Red, true));
                       for (auto& circPtr : _circs) 
                       {
                           circPtr->randomizeColor();
                       }

                   }
               }
                
            }
        }
}
