#pragma once
#include "MyCircle.h"
#include "MyText.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class GameManager
{
public:
	GameManager(sf::RenderWindow* window);
	
private:
	sf::RenderWindow* _window;
	std::vector<std::unique_ptr<MyCircle>> _circs;
	std::vector<std::unique_ptr<MyText>> _texts;
	//std::vector<std::unique_ptr<MyText>> _trash;
	sf::Clock _clock;
	sf::Clock _fpsClock;
	sf::Time _dt;
	sf::Time _fpsTimer;
	int _frames;
	float _newTime;
	float _oldTime;
	void handleInput();
	void update(float dt);
	void checkOutOfB();
	void checkForCollisions();
	void draw();
	void run();
	void showFps();
	void showIntroText();
};