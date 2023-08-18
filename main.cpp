#include "SFML/Graphics.hpp";
#include <iostream>;
#include "MyCircle.h";


int main()
{
    sf::VideoMode res = sf::VideoMode::getDesktopMode();
    int width = res.width;
    int height = res.height;    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "SCR", sf::Style::Fullscreen, settings);
    std::vector<MyCircle*> circs;

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
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    circs.push_back(new MyCircle());
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                if (event.mouseButton.button == sf::Mouse::Left) 
                {
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
        }
        window.clear();
        for (auto& c : circs) 
        {
            for (auto& _c : circs) 
            {
                if (_c != c) 
                {
                    if (_c->isIntersect((*c))) 
                    { 
                        c->calculateCollision(*_c);
                    }
                }
            }

            c->updateMovement();
            window.draw(*c);
        }
        window.display();
    }
    return 0;
}

