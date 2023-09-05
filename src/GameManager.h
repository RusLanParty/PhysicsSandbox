#pragma once
#include "PhysicsEngine.h"
#include "MyText.h"
#include "FPSCounter.h"
#include "Settings.h"
#include "Shaders.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class GameManager
{
public:
	GameManager(sf::RenderWindow* window, std::shared_ptr<sf::Font> font);
	static bool isIntroFinished();
	static int getObjectCount();
	static bool add;
private:
	std::shared_ptr<Shaders> shaders;
	std::shared_ptr<sf::RenderTexture> renderTexture;
	sf::RenderWindow* _window;
	sf::View view;
	float maxZoom;	
	PhysicsEngine Physics;
	FPSCounter _fpsCounter;
	std::shared_ptr<sf::Font> _font;
	sf::Clock _clock;
	sf::Time _dt;
	std::vector<std::shared_ptr<MyCircle>> _circs;
	std::vector<std::shared_ptr<MyText>> _texts;
	float _offsetX;
	float _offsetY;
	float _width;
	float _height;
	void handleInput(float deltaTime);
	void intro(float deltaTime);
	void update(float dt);
	void draw(float deltaTime);
	void run();
	void keyboardInput(float deltaTime, sf::Event);
	void mouseInput(float deltaTime, sf::Event);
	void disposeTrash();
	void showIntro();
	bool inBoundY();
	void spawnOnMouseClick(float deltaTime);
};