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
void run() 
{

    static std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
    if (!font->loadFromFile("arial.ttf"))
    {
        std::cout << "Error loading arial.ttf";
    }
    else
    {         
        std::cout << "Font loaded successfully!" << std::endl;
        sf::RenderWindow* window = createWindow();
        GameManager game(window, font);
        delete window;
    }
}

int main()
{
    run();
    return 0;
}

