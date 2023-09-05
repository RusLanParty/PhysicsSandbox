#pragma once
#include "SFML/Graphics.hpp"
class Shaders 
{
public:
	Shaders(sf::RenderWindow* window);
	void applyBloom(std::shared_ptr<sf::RenderTexture> renderTexture);
private:
	std::shared_ptr<sf::RenderWindow> GameWindow;
	sf::Shader bloomX;
	sf::Shader bloomY;
	sf::Shader add;
};