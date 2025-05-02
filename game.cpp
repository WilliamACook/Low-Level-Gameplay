#include "game.h"
#include <iostream>

game::game() : window(sf::VideoMode({533, 400}), "Joust"), timeStep(6), player(235.f, 300.f, sf::Image("assets/idle.png")), platformText(), 
platformText1(), platformText2(), platformText3(), platformText4(), floorText(), floor(floorText)
{
	lastTime = timer.getElapsedTime();
	loadAssets();
	gameState.setPlayerReference(&player);
}

void game::loadAssets()
{
	const sf::Image platform1("assets/platform1.png");
	bool platformresult1 = platformText1.loadFromImage(platform1, false, sf::IntRect({ 0, 0 }, { 176, 18 }));
	sf::Sprite sp_platform1(platformText1);
	sp_platform1.setPosition({ 146.f, 100.f });
	platforms.push_back(sp_platform1);

	const sf::Image platform2("assets/platform2.png");
	bool platformresult2 = platformText2.loadFromImage(platform2, false, sf::IntRect({ 0, 0 }, { 128, 16 }));
	sf::Sprite sp_platform2(platformText2);	
	sp_platform2.setPosition({ 483.f, 200.f });
	platforms.push_back(sp_platform2);

	const sf::Image platform3("assets/platform3.png");
	bool platformresult3 = platformText3.loadFromImage(platform3, false, sf::IntRect({ 0, 0 }, { 128, 16 }));
	sf::Sprite sp_platform3(platformText3);
	sp_platform3.setPosition({ 370.f, 185.f });
	platforms.push_back(sp_platform3);

	const sf::Image platform4("assets/platform4.png");
	bool platformresult4 = platformText4.loadFromImage(platform4, false, sf::IntRect({ 0, 0 }, { 128, 16 }));
	sf::Sprite sp_platform4(platformText4);
	sp_platform4.setPosition({ 10.f, 200.f });
	platforms.push_back(sp_platform4);

	const sf::Image floorImage("assets/floorPlatform.png");
	bool floorresult = floorText.loadFromImage(floorImage, false, sf::IntRect({ 0, 0 }, { 149, 30 }));
	sf::Sprite sp_floor(floorText);
	sp_floor.setScale(sf::Vector2f(2.f, 2.f));
	sp_floor.setPosition({ 120.f,340.f });
	platforms.push_back(sp_floor);

	enemyTexture.loadFromFile("assets/enemyFlyingSpritesheet.png");
	lifeTexture.loadFromFile("assets/life.png");
	deathParticleTexture.loadFromFile("assets/DeathParticle.png");
	eggTexture.loadFromFile("assets/egg.png");

	floor = sp_floor;

	//Creating Different Spawn Points
	spawnPoints.push_back({50.f, 170.f});
	spawnPoints.push_back({ 440.f, 160.f });
	spawnPoints.push_back({ 210.f, 60.f });
}

void game::spawnWave(int numEnemies)
{
	enemiesToSpawn = numEnemies;
}

void game::run()
{
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
			}
		}

		currentTime = timer.getElapsedTime();
		int deltaTime = (currentTime.asMilliseconds() - lastTime.asMilliseconds());
		if (deltaTime > timeStep)
		{
			lastTime = currentTime;
			if (gameState.getState() == GameState::Playing)
			{
				player.update(window, platforms);
				for (auto& enemy : enemies)
				{
					enemy.update(platforms);
				}

				for (auto& egg : eggs)
				{
					egg.handleCollision(platforms);
					egg.update();
				}

				if (enemies.empty() && enemiesToSpawn == 0)
				{
					spawnWave(5);
				}

				if (enemiesToSpawn > 0)
				{
					if (spawnClock.getElapsedTime().asSeconds() >= spawnSpeed)
					{
						spawnClock.restart();
						std::cout << "EnemySpawned" << std::endl;
						sf::Vector2f spawnPos = spawnPoints[rand() % spawnPoints.size()];
						enemies.emplace_back(enemyTexture, spawnPos);
						enemiesToSpawn--;
					}
				}

				if (player.invincible == false)
				{
					//Player and Enemy Collisions checks
					sf::Vector2f playerPos = player.getPosition();
					sf::Vector2f playerSize = player.getSize();
					for (auto i = enemies.begin(); i != enemies.end(); )
					{
						sf::Vector2f enemyPos = i->getPosition();
						sf::Vector2f enemySize = i->getSize();

						bool collisionX = playerPos.x + playerSize.x > enemyPos.x && playerPos.x < enemyPos.x + enemySize.x;
						bool collisionY = playerPos.y + playerSize.y > enemyPos.y && playerPos.y < enemyPos.y + enemySize.y;

						if (collisionX && collisionY)
						{
							if (playerPos.y < enemyPos.y)
							{
								std::cout << "Enemy Die " << std::endl;
								eggs.emplace_back(eggTexture, enemyPos);
								i = enemies.erase(i);
								continue;
							}
							else
							{
								std::cout << "Player Die " << std::endl;
								deathParticles.emplace_back(deathParticleTexture, playerPos);
								player.reset();
								player.loseLife();
								if (player.getLives() == 0)
								{
									gameState.setState(GameState::GameOver);
									enemies.clear();
									break;
								}
							}
						}
						++i;
					}
				}
			}
		/*	for (auto i = eggs.begin(); i != eggs.end(); )
			{
				if(player.getBounds().findIntersection(i->getBounds()))
				{
					i = eggs.erase(i);
				}
			}*/

			for (auto i = deathParticles.begin(); i != deathParticles.end(); )
			{
				if (!i->update())
				{
					i = deathParticles.erase(i);
				}
				else
				{
					++i;
				}
			}
			gameState.handleInput(window);
		}

		//Drawing depending on the state
		if (gameState.getState() == GameState::Playing)
		{
			window.clear();
			gameState.draw(window);
			//window.draw(sp_enemy);
			for (auto& enemy : enemies)
			{
				enemy.draw(window);
			}

			for (auto& platform : platforms)
			{
				window.draw(platform);
			}

			for (auto particle : deathParticles)
			{
				particle.draw(window);
			}

			for (auto& egg : eggs)
			{
				egg.draw(window);
			}

			for (int i = 0; i < player.getLives(); i++)
			{
				sf::Sprite lifeSprite(lifeTexture);
				lifeSprite.setPosition(sf::Vector2f(220.f + i * (lifeSprite.getGlobalBounds().size.x + 5.f), 352.f));
				window.draw(lifeSprite);
			}
			player.draw(window);
			window.display();
		}
		else if (gameState.getState() == GameState::Paused)
		{
			window.clear();
			for (auto& enemy : enemies)
			{
				enemy.draw(window);
			}

			for (auto& platform : platforms)
			{
				window.draw(platform);
			}
			player.draw(window);
			gameState.draw(window);
			window.display();
		}
		else
		{
			window.clear();
			window.draw(floor);
			gameState.draw(window);
			window.display();
		}
	}
}
