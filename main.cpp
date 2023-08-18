#include "SFML/Graphics.hpp";
#include <iostream>;
#include <random>;

void changeColor(sf::CircleShape& c)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    sf::Color randCol(dis(gen), dis(gen), dis(gen));
    c.setFillColor(randCol);
}

int main()
{
    int width = 800;
    int height = 600;    
    sf::RenderWindow window(sf::VideoMode(width, height), "SCR", sf::Style::None);
    sf::CircleShape circle(50.0f);
    circle.setFillColor(sf::Color::Magenta);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    sf::Vector2f vel(0.01f, 0.01f);
    sf::Vector2f position(window.getSize().x / 2, window.getSize().y / 2);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }

        position += vel;

        if (position.x > window.getSize().x - circle.getRadius() || position.x < circle.getRadius())
        {
            std::cout << "x= " << position.x << "\n";
            vel.x = -vel.x;
            changeColor(circle);
        }
        if (position.y > window.getSize().y - circle.getRadius() || position.y < circle.getRadius())
        {
            std::cout << "y= " << position.y << "\n";
            vel.y = -vel.y;
            changeColor(circle);
        }

        circle.setPosition(position);
        window.clear();
        window.draw(circle);
        window.display();
    }
    return 0;
}

