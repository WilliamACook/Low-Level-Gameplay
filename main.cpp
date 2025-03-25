#include <SFML/Graphics.hpp>
#include <iostream>
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
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);
    
    int distPerSecond = 100;
    int timeStep = 10;  // 10 ms, 100 updates per second
	float groundY = 350.f;
	bool onPlatform = false;

	//CharacterTexture
	const sf::Image character("assets/character.png");
	sf::Texture characterText;
	bool result = characterText.loadFromImage(character, false, sf::IntRect({ 0,0 }, { 24,24 }));
	sf::Sprite player(characterText);
	//spritey.scale(sf::Vector2(2.0f, 2.0f));
	sf::FloatRect boundingBox = player.getGlobalBounds();

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

	sf::Time lastTime = sf::Time();
	sf::Time currentTime = sf::Time();
	sf::Clock timer = sf::Clock();
	timer.start();
	lastTime = timer.getElapsedTime();

	bool transitioned = false;

	//int checkCollision(sf::Sprite & player, sf::Sprite & platform) {
	//	//float closestX = clamp(player.getPosition().x);
	//	return;
	//}
	

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

        }

        currentTime = timer.getElapsedTime();
        int timeDelta = (currentTime.asMilliseconds() - lastTime.asMilliseconds());
        if (timeDelta > timeStep)
        {
            lastTime = currentTime;
            float shiftDistance = distPerSecond * timeDelta / 1000;
			sf::FloatRect newposition = player.getGlobalBounds();

			// TODO:
			// Instead of moving the player straight away, manipulate newposition
			// check newposition instead of player, use it like a view into a future potential movement.
			// If it's colliding, discard the newposition and do not move
			// otherwise, overwrite the globalbounds of the rect with the newposition.

            //Controls
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				//spritey.move({ -shiftDistance, 0.f });

			{
				float x = player.getPosition().x - shiftDistance;
				float y = player.getPosition().y;
				if (x < .0f) { x = 400.0f; }
				player.setPosition(sf::Vector2f{ x,y });
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				//spritey.move({ shiftDistance, 0.f });
			{
				float x = player.getPosition().x + shiftDistance;
				float y = player.getPosition().y;
				if (x > 400.0f) { x = .0f; }
				player.setPosition(sf::Vector2f{ x,y });
			}
				
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				player.move({ 0.f, -shiftDistance });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !onPlatform)
				player.move({ 0.f, shiftDistance });

			//Gravity
			if (onPlatform == false)
			{
				player.move({ 0.f, 0.1f });
			}
			//Wrap screen
			if (sp_enemy.getPosition().x > 400.f)
			{			
				sp_enemy.setPosition({-10, 0});
			}
			else
			{
				sp_enemy.move({ 1.f, 0.f });
			}

			// Get positions and sizes
			sf::Vector2f platformPos = sp_platform.getPosition();
			sf::Vector2f platformSize(24.f, 24.f);  
			sf::Vector2f playerPos = player.getPosition();
			sf::Vector2f playerSize(24.f, 24.f);  

			// Debugging output
			std::cout << "Player Position: " << playerPos.x << ", " << playerPos.y << std::endl;
			std::cout << "Platform Position: " << platformPos.x << ", " << platformPos.y << std::endl;

			// Check if the player is falling and is above the platform
			if (playerPos.y + playerSize.y <= platformPos.y &&  
				playerPos.x + playerSize.x > platformPos.x &&   
				playerPos.x < platformPos.x + platformSize.x && 
				playerPos.y + playerSize.y + 0.1f >= platformPos.y) { 

				player.setPosition(sf::Vector2f(playerPos.x, platformPos.y - playerSize.y));  
				onPlatform = true;  
			}
			else {
				onPlatform = false;
			}

			// Handle horizontal collisions (left and right)
			if (playerPos.x + playerSize.x > platformPos.x && playerPos.x < platformPos.x + platformSize.x) {
				// Check for side collisions and prevent movement through platform
				if (playerPos.y + playerSize.y > platformPos.y && playerPos.y < platformPos.y + platformSize.y) {
					// Horizontal collision, stop moving in that direction
					// This ensures the player cannot move through the platform horizontally
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
						player.setPosition(sf::Vector2(playerPos.x + shiftDistance, playerPos.y));  // Prevent moving through platform from the left
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
						player.setPosition(sf::Vector2(playerPos.x - shiftDistance, playerPos.y));  // Prevent moving through platform from the right
					}
				}
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
			//	float distanceX = player.getPosition().x - sp_platform.getPosition().x;
			//	float distanceY = player.getPosition().y - sp_platform.getPosition().y;

			//	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
			//	//std::cout << "Collision with platform" << std::endl;;
			//	if (player.getPosition().x <= sp_platform.getPosition().x)
			//	{
			//		std::cout << "Left" << std::endl;
			//	}
			//	if (player.getPosition().x >= sp_platform.getPosition().y)
			//	{
			//		std::cout << "Right" << std::endl;
			//	}
			//	//onPlatform == true; // Does the onPlatform variable have the value true? result: no
			// 
			//	onPlatform = true; // onPlatform is set to the value of true;
			//}
			//else
			//{
			//	onPlatform = false;
			//}
			

			
			/*if (spritey.getPosition().x > 400.f && transitioned == false)
			{
				transitioned = true;
				spritey.setPosition({ -10,spritey.getPosition().y });
			}
			if (spritey.getPosition().x <  0.f && transitioned == false)
			{
				transitioned = true;
				spritey.setPosition({ 400.f,spritey.getPosition().y });
			}

			if (spritey.getPosition().x > 1 && spritey.getPosition().x < 399 && transitioned == true)
			{
				transitioned = false;
			}*/

				
        }

        window.clear();
        //window.draw(shape);
		window.draw(player);
		window.draw(sp_enemy);
		window.draw(sp_platform);
        window.display();

    }

    
}