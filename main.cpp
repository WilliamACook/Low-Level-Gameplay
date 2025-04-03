#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "GameStateManager.h"
//#include <chrono>

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/

#ifdef _CONSOLE
int main()
#else
int WinMain()
#endif
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");
    int timeStep = 6;  // 10 ms, 100 updates per second
	GameStateManager gameState;

	//Player
	const sf::Image playerTexture("assets/character.png");
	player player(0, 0, playerTexture);

	//EnemyTexture
	const sf::Image enemy("assets/enemy.png");
	sf::Texture enemyText;
	bool enemyresult = enemyText.loadFromImage(enemy, false, sf::IntRect({ 0,0 }, { 24,24 }));
	sf::Sprite sp_enemy(enemyText);

	//Platform
	const sf::Image platform("assets/platform.png");
	sf::Texture platformText;
	bool platformresult = platformText.loadFromImage(platform, false, sf::IntRect({ 0, 0 }, { 24, 24 }));
	sf::Sprite sp_platform(platformText);
	sp_platform.setPosition({ 200.f, 200.f });
	std::vector<sf::Sprite> platforms;
	platforms.push_back(sp_platform);

	const sf::Image platform1("assets/platform1.png");
	sf::Texture platformText1;
	bool platformresult1 = platformText1.loadFromImage(platform1, false, sf::IntRect({ 0, 0 }, { 176, 18 }));
	sf::Sprite sp_platform1(platformText1);
	sp_platform1.setPosition({ 120.f, 60.f });
	platforms.push_back(sp_platform1);

	const sf::Image platform2("assets/platform2.png");
	sf::Texture platformText2;
	bool platformresult2 = platformText2.loadFromImage(platform2, false, sf::IntRect({ 0, 0 }, { 128, 16 }));
	sf::Sprite sp_platform2(platformText2);
	sp_platform2.setPosition({ 350.f, 130.f });
	platforms.push_back(sp_platform2);

	const sf::Image floor("assets/floor.png");
	sf::Texture floorText;
	bool floorresult = floorText.loadFromImage(floor, false, sf::IntRect({ 0, 0 }, { 372, 4 }));
	sf::Sprite sp_floor(floorText);
	sp_floor.setPosition({ 16.f,380.f });
	platforms.push_back(sp_floor);

	//Time
	sf::Time lastTime = sf::Time();
	sf::Time currentTime = sf::Time();
	sf::Clock timer = sf::Clock();
	timer.start();
	lastTime = timer.getElapsedTime();

	

    while (window.isOpen())
    {
        // Event polling section of code - this must be done in the thread which created the window
        // we will talk about threading later, but essentially this must be done here
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
			//std::cout << "Drawing player at: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;
			

        }

        currentTime = timer.getElapsedTime();
        int timeDelta = (currentTime.asMilliseconds() - lastTime.asMilliseconds());
        if (timeDelta > timeStep)
        {
            lastTime = currentTime;
			player.update();
			gameState.handleInput(window);
			
			player.handleCollision(platforms);
			// TODO:
			// Instead of moving the player straight away, manipulate newposition
			// check newposition instead of player, use it like a view into a future potential movement.
			// If it's colliding, discard the newposition and do not move
			// otherwise, overwrite the globalbounds of the rect with the newposition.

			//Wrap screen
			if (sp_enemy.getPosition().x > 400.f)
			{			
				sp_enemy.setPosition({-10, 0});
			}
			else
			{
				sp_enemy.move({ 1.f, 0.f });
			}

			////Collision
			//if (newposition.findIntersection(sp_enemy.getGlobalBounds()))
			//{
			//	std::cout << "Collision " << sp_enemy.getPosition().x<<":"<<sp_enemy.getPosition().y << std::endl;;
			//	
			//	
			//}
			//if (player.getGlobalBounds().findIntersection(sp_platform.getGlobalBounds()))
			//{
			//	/*float distanceX = player.getPosition().x - sp_platform.getPosition().x;
			//	float distanceY = player.getPosition().y - sp_platform.getPosition().y;

			//	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);*/
			//	std::cout << "Collision with platform" << std::endl;;
			//	/*if (player.getPosition().x <= sp_platform.getPosition().x)
			//	{
			//		std::cout << "Left" << std::endl;
			//	}
			//	if (player.getPosition().x >= sp_platform.getPosition().y)
			//	{
			//		std::cout << "Right" << std::endl;
			//	}*/
			//	onPlatform == true; // Does the onPlatform variable have the value true? result: no
			// 
			//	onPlatform = true; // onPlatform is set to the value of true;
			//}
			//else
			//{
			//	onPlatform = false;
			//}			
        }
        
		if (gameState.getState() == GameState::Playing)
		{
			window.clear();
			window.draw(sp_enemy);
			window.draw(sp_platform);
			window.draw(sp_platform1);
			window.draw(sp_platform2);
			window.draw(sp_floor);
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