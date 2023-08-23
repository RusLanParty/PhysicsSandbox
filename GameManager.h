#pragma once
#include "MyCircle.h"
#include "PhysicsEngine.h"
#include "MyText.h"
#include "FPSCounter.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class GameManager
{
public:
	GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font);

private:
	PhysicsEngine _physicsEngine;
    std::shared_ptr<sf::Font> _font;
	FPSCounter _fpsCounter;
	sf::RenderWindow* _window;
	std::vector<std::shared_ptr<MyCircle>> _circs;
	std::vector<std::unique_ptr<MyText>> _texts;
	//std::vector<std::unique_ptr<MyText>> _trash;
	sf::Clock _clock;
	sf::Time _dt;
	void handleInput();
	void update(float dt);
	void draw();
	void run();
	void showIntroText();
	
};