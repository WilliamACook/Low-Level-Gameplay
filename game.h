#pragma once

#include <SFML/Graphics.hpp>
#include "player.h"
#include "Enemy.h"
#include "GameStateManager.h"

class game
{
private:
	sf::RenderWindow window;
	sf::Clock timer;
	sf::Time lastTime;
	sf::Time currentTime = sf::Time();
	int timeStep;

	GameStateManager gameState;
	player player;
	std::vector<Enemy> enemies;
	std::vector<sf::Sprite> platforms;

	sf::Texture platformText, platformText1, platformText2, floorText;
	sf::Sprite floor;

	void loadAssets();

public:
	game();
	void run();
};

