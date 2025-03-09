#include <SFML/Graphics.hpp>

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/
int WinMain()
{
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");
    sf::CircleShape shape(10.f);
    shape.setFillColor(sf::Color::Green);

    sf::Time lastTime = sf::Time();
    sf::Time currentTime = sf::Time();
    int distPerSecond = 100;
    int timeStep = 10;  // 10 ms, 100 updates per second
	float gravity = 10.f;
	float groundY = 350.f;
	sf::Vector2f velocity(0.f, 0.f);

    sf::Clock timer = sf::Clock ();
    timer.start();
    lastTime = timer.getElapsedTime();

    sf::Clock clock;

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
		float deltaTime = clock.restart().asSeconds();
		velocity.y += gravity;
		shape.move(velocity * deltaTime);
		if(velocity.y > 5.f)
			velocity.y = 5.f;

        currentTime = timer.getElapsedTime();
        int timeDelta = (currentTime.asMilliseconds() - lastTime.asMilliseconds());
        if (timeDelta > timeStep)
        {
            lastTime = currentTime;
            float shiftDistance = distPerSecond * timeDelta / 1000;
            //Controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
				shape.move({ -shiftDistance, 0.f });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
				shape.move({ shiftDistance, 0.f });
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
				shape.move({ 0.f, -shiftDistance });

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				shape.move({ 0.f, shiftDistance });
        }

        window.clear();
        window.draw(shape);
        window.display();

    }

    
}