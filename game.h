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

	sf::Sprite sp_platform, sp_platform1, sp_platform2, sp_floor;
	sf::Texture platformText, platformText1, platformText2, floorText;

	void loadAssets();

public:
	game();
	void run();
};

