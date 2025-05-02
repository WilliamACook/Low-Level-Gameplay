#pragma once

#include <SFML/Graphics.hpp>
#include "player.h"
#include "Enemy.h"
#include "GameStateManager.h"
#include "DeathParticle.h"
#include "Pickup.h"

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
	sf::Image enemyImage;
	sf::Texture enemyTexture;
	sf::Texture deathParticleTexture;
	std::vector<Enemy> enemies;
	std::vector<sf::Sprite> platforms;
	std::vector<DeathParticle> deathParticles;
	std::vector<Pickup> eggs;

	int currentWave = 1;
	int enemiesToSpawn = 4;
	float spawnSpeed = 1.f;
	sf::Clock spawnClock;
	std::vector<sf::Vector2f> spawnPoints;

	sf::Texture platformText, platformText1, platformText2, platformText3, platformText4, floorText, enemyText, lifeTexture, eggTexture;
	sf::Sprite floor;

	void loadAssets();
	void spawnWave(int numEnemies);

public:
	game();
	void run();
};

