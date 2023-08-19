#include "SFML/Graphics.hpp";
#include <iostream>;
#include "MyCircle.h";

bool gravity = false;
int main()
{
    sf::Vector2f prevMousePos;
    sf::Vector2f offset(0.0f, 0.0f);
    bool isDragging = false;
    sf::Clock clock;
    sf::Time elapsedTime;
    unsigned int frames = 0;
    sf::VideoMode res = sf::VideoMode::getDesktopMode();
    int width = res.width;
    int height = res.height;    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "SCR", sf::Style::Fullscreen, settings);
    window.setFramerateLimit(144);
    
    std::vector<MyCircle*> circs;
    circs.push_back(new MyCircle(width / 2, height / 2, 0.0f, 0.0f, 70.0f));
    circs.push_back(new MyCircle(90.0f, 90.0f, 0.0f, 0.0f, 60.0f));
    circs.push_back(new MyCircle(width - 90.0f , height - 90.0f, 0.0f, 0.0f, 60.0f));
    circs.push_back(new MyCircle(90.0f, height - 90.0f, 0.0f, 0.0f, 60.0f));
    circs.push_back(new MyCircle(width - 90.0f, 90.0f, 0.0f, 0.0f, 60.0f));
    circs.push_back(new MyCircle(width - 200.0f, 350.0f, -24.0f, -32.0f, 35.0f));

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Error loading arial.ttf";
    }
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setString("   Use left click to spawn circles");
    text.move(width / 2.5, height / 4);
    sf::Text text1;
    text1.setFont(font);
    text1.setFillColor(sf::Color::White);
    text1.setString("Press ESC to exit");
    text1.move(width / 2.26, height / 2.5);
    sf::Text text2;
    text2.setFont(font);
    text2.setFillColor(sf::Color::White);
    text2.setString("Use right click to despawn circles");
    text2.move(width / 2.5, height / 3.1);

    bool fadingIn = true; // Initially fading in
    bool fadingOut = false; // Will start fading out after mouse click
    bool fadeComplete = false; // Will stop drawing the text when done
    float currentAlpha = 0.0f;
    float currentAlpha1 = 0.0f;
    float currentAlpha2 = 0.0f;
    float fadeSpeed = 2.0f;
    float fadeOutSpeed = 4.0f;
    
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::White);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                 window.close();
            }
            else if (event.type == sf::Event::KeyPressed) 
            {
                if (event.key.code == sf::Keyboard::Escape) 
                {
                    circs.clear();
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    gravity = !gravity;
                    if (gravity) 
                    {
                        for (auto& circle : circs)
                        {
                            circle->randomizeColor();
                        }
                    }
                    else if (!gravity) 
                    {
                        for (auto& circle : circs)
                        {
                            circle->randomizeSpeed();
                        }
                    }
                    
                }
                else if (event.key.code == sf::Keyboard::X)
                {
                    circs.clear();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
                    fadingIn = false;
                    fadingOut = true;
                    sf::Vector2i mousePos = sf::Mouse::getPosition();
                    float mouseX = (float)mousePos.x;
                    float mouseY = (float)mousePos.y;
                    sf::Vector2f mousePosF(mouseX, mouseY);
                    if (circs.empty()) 
                    {
                        circs.push_back(new MyCircle(mousePosF));
                    }
                    else
                    {
                        bool block = false;
                        for (auto& c : circs)
                        {
                            if (c->contains(mousePosF))
                            {
                                block = true;
                                isDragging = true;
                                offset = c->getPosition() - mousePosF;
                            }
                        }
                        if (!block)
                        {
                           circs.push_back(new MyCircle(mousePosF));
                        }
                    }
                }
                
                if (event.mouseButton.button == sf::Mouse::Right) 
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition();
                    float mouseX = (float)mousePos.x;
                    float mouseY = (float)mousePos.y;
                    sf::Vector2f mousePosF(mouseX, mouseY);

                    for (auto& c : circs) 
                    {
                        if (c->contains(mousePosF)) 
                        {
                            circs.erase(std::remove(circs.begin(), circs.end(), c), circs.end());
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                isDragging = false;               
            }
        }

        window.clear();
       
        for (auto& c : circs) 
        {
            if (isDragging)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (c->contains(mousePosF)) {
                    sf::Vector2f newPos = mousePosF + offset;
                    c->setPosition(newPos);
                }
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    prevMousePos = mousePosF;
                }
                else if (event.type == sf::Event::MouseMoved)
                {
                    float deltaTime = static_cast<float>(elapsedTime.asSeconds());

                    // Calculate the change in position (deltaPos)
                    sf::Vector2f deltaPos = (mousePosF - prevMousePos);

                    // Calculate the X and Y speeds separately
                    float xSpeed = std::clamp(deltaPos.x / deltaTime, -5.0f, 5.0f);
                    float ySpeed = std::clamp(deltaPos.y / deltaTime, -5.0f, 5.0f);
                    c->setSpeed(xSpeed, ySpeed);
                    std::cout << "X Speed: " << xSpeed << ", Y Speed: " << ySpeed << std::endl;

                    prevMousePos = mousePosF;
                }
            }
            for (auto& _c : circs) 
            {
                if (_c != c) 
                {
                    if (_c->isIntersect((*c))) 
                    { 
                        c->resolveIntersections(*_c);
                    }
                }
            }
            if (!gravity) 
            {
                c->decreaseSpeed();
                c->updateColor();
            }
            else if (gravity) 
            {
                c->setSpeed(c->getSpeed().x, c->getSpeed().y + 0.1f);
            }
            c->updateMovement();
            window.draw(*c);
        }  
        if (!fadeComplete) 
        {
            if (fadingIn)
            {
                currentAlpha += fadeSpeed;
                if (currentAlpha >= 255) 
                {
                    currentAlpha = 255;
                    currentAlpha2 += fadeSpeed;
                    if (currentAlpha2 >= 255) 
                    {
                        currentAlpha2 = 255;
                        currentAlpha1 += fadeSpeed;
                        if (currentAlpha1 >= 255) 
                        {
                            currentAlpha1 = 255;
                            fadingIn = false;
                        }
                    }
                }
            }
            else if (fadingOut)
            {
                currentAlpha -= fadeOutSpeed;
                if (currentAlpha <= 0)
                {
                    currentAlpha = 0;
                    currentAlpha2 -= fadeOutSpeed;
                    if (currentAlpha2 <= 0) 
                    {
                        currentAlpha2 = 0;
                        currentAlpha1 -= fadeOutSpeed;
                        if (currentAlpha1 <= 0) 
                        {
                            currentAlpha1 = 0;
                            fadeComplete = true;
                        }
                    }
                }
            }
            sf::Color fade(text.getFillColor());
            fade.a = currentAlpha;
            text.setFillColor(fade);
            sf::Color fade1(text1.getFillColor());
            fade1.a = currentAlpha1;
            text1.setFillColor(fade1);
            sf::Color fade2(text2.getFillColor());
            fade2.a = currentAlpha2;
            text2.setFillColor(fade2);
            window.draw(text);
            window.draw(text1);
            window.draw(text2);
        }
        frames++;
        elapsedTime += clock.restart();
        if (elapsedTime >= sf::seconds(1.0f))
        {
            float fps = static_cast<float>(frames) / elapsedTime.asSeconds();
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frames = 0;
            elapsedTime = sf::Time::Zero;
           
        }

        // Draw FPS text
        window.draw(fpsText);
        window.display();
    }
    return 0;
}

