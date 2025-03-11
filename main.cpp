#include <SFML/Graphics.hpp>
#include <chrono>

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/
int WinMain()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);
    
    int distPerSecond = 100;
    int timeStep = 10;  // 10 ms, 100 updates per second
	float groundY = 350.f;

	//CharacterTexture
	const sf::Image character("assets/character.png");
	sf::Texture characterText;
	bool result = characterText.loadFromImage(character, false, sf::IntRect({ 0,0 }, { 24,24 }));
	sf::Sprite spritey(characterText);
	//spritey.scale(sf::Vector2(2.0f, 2.0f));

	//EnemyTexture
	const sf::Image enemy("assets/enemy.png");
	sf::Texture enemyText;
	bool enemyresult = enemyText.loadFromImage(enemy, false, sf::IntRect({ 0,0 }, { 24,24 }));
	sf::Sprite sp_enemy(enemyText);

	sf::Time lastTime = sf::Time();
	sf::Time currentTime = sf::Time();
	sf::Clock timer = sf::Clock();
	timer.start();
	lastTime = timer.getElapsedTime();

	bool transitioned = false;

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
            //Controls
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				//spritey.move({ -shiftDistance, 0.f });

			{
				float x = spritey.getPosition().x - shiftDistance;
				float y = spritey.getPosition().y;
				if (x <  .0f) { x = 400.0f + x; }
				spritey.setPosition(sf::Vector2f{ x,y });
			}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				spritey.move({ shiftDistance, 0.f });
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				spritey.move({ 0.f, -shiftDistance });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				spritey.move({ 0.f, shiftDistance });

			//Gravity
			spritey.move({ 0.f, 0.1f });
			
			//Wrap screen
			if (sp_enemy.getPosition().x > 400.f)
			{			
				sp_enemy.setPosition({-10, 0});
			}
			else
			{
				sp_enemy.move({ 1.f, 0.f });
			}

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
		window.draw(spritey);
		window.draw(sp_enemy);
        window.display();

    }

    
}