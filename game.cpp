#include "game.h"
#include <iostream>

game::game() : window(sf::VideoMode({400, 400}), "Joust"), timeStep(6), player(200.f, 160.f, sf::Image("assets/idle.png")), sp_platform(platformText), sp_platform1(platformText1), sp_platform2(platformText2), sp_floor(floorText)
{
	lastTime = timer.getElapsedTime();
	loadAssets();
	const sf::Image enemyTexture("assets/enemy.png");
	enemies.emplace_back(enemyTexture, sf::Vector2f(0, 0));
	gameState.setPlayerReference(&player);
}

void game::loadAssets()
{
	const sf::Image platform("assets/platform.png");
	bool platformresult = platformText.loadFromImage(platform, false, sf::IntRect({ 0, 0 }, { 24, 24 }));
	sf::Sprite sp_platform(platformText);
	sp_platform.setPosition({ 200.f, 200.f });
	platforms.push_back(sp_platform);

	const sf::Image platform1("assets/platform1.png");
	bool platformresult1 = platformText1.loadFromImage(platform1, false, sf::IntRect({ 0, 0 }, { 176, 18 }));
	sf::Sprite sp_platform1(platformText1);
	sp_platform1.setPosition({ 120.f, 60.f });
	platforms.push_back(sp_platform1);

	const sf::Image platform2("assets/platform2.png");
	bool platformresult2 = platformText2.loadFromImage(platform2, false, sf::IntRect({ 0, 0 }, { 128, 16 }));
	sf::Sprite sp_platform2(platformText2);	
	sp_platform2.setPosition({ 350.f, 130.f });
	platforms.push_back(sp_platform2);

	const sf::Image floor("assets/floor.png");
	bool floorresult = floorText.loadFromImage(floor, false, sf::IntRect({ 0, 0 }, { 372, 4 }));
	sf::Sprite sp_floor(floorText);
	sp_floor.setPosition({ 16.f,380.f });
	platforms.push_back(sp_floor);
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
		int timeDelta = (currentTime.asMilliseconds() - lastTime.asMilliseconds());
		if (timeDelta > timeStep)
		{
			lastTime = currentTime;
			if (gameState.getState() == GameState::Playing)
			{
				player.update(window, platforms);
				player.handleCollision(platforms);
				for (auto& enemy : enemies)
				{
					enemy.update();
				}

				sf::Vector2f playerPos = player.getPosition();
				sf::Vector2f playerSize = player.getSize();

				for (auto it = enemies.begin(); it != enemies.end(); )
				{
					sf::Vector2f enemyPos = it->getPosition();
					sf::Vector2f enemySize = it->getSize();
					float enemyMiddleY = enemyPos.y + enemySize.y / 2.f;


					bool collisionX = playerPos.x + playerSize.x > enemyPos.x && playerPos.x < enemyPos.x + enemySize.x;
					bool collisionY = playerPos.y + playerSize.y > enemyPos.y && playerPos.y < enemyPos.y + enemySize.y;

					if (collisionX && collisionY)
					{
						if (playerPos.y + playerSize.y >= enemyMiddleY - 10.f && playerPos.y + playerSize.y <= enemyPos.y + enemySize.y)
						{
							std::cout << "Enemy Die " << std::endl;
							it = enemies.erase(it);
							continue;
						}
						else
						{
							std::cout << "Player Die " << std::endl;
							player.reset();
							player.loseLife();
							if (player.getLives() == 0)
							{
								gameState.setState(GameState::GameOver);
							}
						}
					}
					++it;
				}
			}

			gameState.handleInput(window);

			// TODO:
			// Instead of moving the player straight away, manipulate newposition
			// check newposition instead of player, use it like a view into a future potential movement.
			// If it's colliding, discard the newposition and do not move
			// otherwise, overwrite the globalbounds of the rect with the newposition.		
		}

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
			player.draw(window);
			window.display();
		}
		else
		{
			window.clear();
			gameState.draw(window);
			window.display();
		}
	}
}
