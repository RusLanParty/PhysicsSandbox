#include <iostream>;
#include "MyCircle.h";
#include "GameManager.h";

const int FPS = 144;

sf::RenderWindow* createWindow() 
{
    sf::VideoMode res = sf::VideoMode::getDesktopMode();
    int width = res.width;
    int height = res.height;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(width, height), "SCR", sf::Style::Fullscreen, settings);
    window->setFramerateLimit(FPS);
    return window;
}


int main()
{
    sf::RenderWindow* window = createWindow();
    GameManager game(window);
    delete window;
    return 0;
}

