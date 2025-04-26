#include "player.h"
#include <iostream>

player::player(float x, float y, const sf::Image texturesrc) : texture(), sprite(texture)
{
	bool result = texture.loadFromImage(texturesrc, false, sf::IntRect({ 0,0 }, { 31,41 }));

	sf::Sprite player(texture);
	sprite = player;
	sprite.setPosition(sf::Vector2f(x, y));
	
	onPlatform = false;

	loadAnimation();
}

void player::update(sf::RenderWindow & window, const std::vector<sf::Sprite>& platforms)
{
	sf::Vector2f playerPos = sprite.getPosition();
	onPlatform = false;
	isMoving = false;
	float moveSpeed = 1.f;

	if (invincible)
	{
		if (invincibleClock.getElapsedTime().asSeconds() > flashSpeed)
		{
			visible = !visible;
			sprite.setColor(visible ? sf::Color::White : sf::Color(255, 255, 255, 300));
			invincibleClock.restart();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			invincible = false;
			sprite.setColor(sf::Color::White);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		sprite.setOrigin(sf::Vector2(sprite.getLocalBounds().size.x, 0.f));
		sprite.setScale(sf::Vector2f( - 1.f, 1.f));
		isMoving = true;
		velocity.x -= acceleration;
		if (velocity.x < -maxSpeed) velocity.x = -maxSpeed; 
		
		//float x = sprite.getPosition().x - moveSpeed;
		float y = sprite.getPosition().y;
		if (playerPos.x < 0.f) playerPos.x = 533.f;
			sprite.setPosition(sf::Vector2f{ playerPos.x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		sprite.setOrigin(sf::Vector2(0.f, 0.f));
		sprite.setScale(sf::Vector2f(1.f, 1.f));
		isMoving = true;
		velocity.x += acceleration;
		if (velocity.x > maxSpeed) velocity.x = maxSpeed;
		//float x = sprite.getPosition().x + moveSpeed;
		float y = sprite.getPosition().y;
		if (playerPos.x > 533.0f) playerPos.x = .0f; 
		sprite.setPosition(sf::Vector2f{ playerPos.x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		float x = sprite.getPosition().x;
		float y = sprite.getPosition().y - moveSpeed;
		sprite.setPosition(sf::Vector2f{ x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !onPlatform)
	{
		float x = sprite.getPosition().x;
		float y = sprite.getPosition().y + moveSpeed;
		sprite.setPosition(sf::Vector2f{ x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if (!just_jumped)
		{
			sf::Vector2f playerPos = sprite.getPosition();
			sf::Vector2f playerSize(24.f, 24.f);
			float projected_height = sprite.getPosition().y - 20.f;
			bool collidesWithPlatform = false;
			for (const auto& platform : platforms)
			{
				sf::Vector2f platformPos = platform.getPosition();
				sf::Vector2 platformSize = platform.getTexture().getSize();

				bool collisionX = playerPos.x + playerSize.x > platformPos.x && playerPos.x < platformPos.x + platformSize.x;
				bool collisionY = projected_height + playerSize.y > platformPos.y && projected_height < platformPos.y + platformSize.y;

				if (collisionX && collisionY)
				{
					collidesWithPlatform = true;
					break;
				}
			}
			if (!collidesWithPlatform)
			{
				if (projected_height < 0)
				{
					projected_height = 0;
				}
				sprite.setPosition(sf::Vector2f(sprite.getPosition().x, projected_height));
			}
			just_jumped = true;
		}
	}
	else
	{
		just_jumped = false;
	}
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)))
	{
		if (velocity.x > 0)
		{
			velocity.x -= deceleration;
			if (velocity.x < 0) velocity.x = 0;
		}
		else if (velocity.x < 0)
		{
			velocity.x += deceleration;
			if (velocity.x > 0) velocity.x = 0;
		}
	}
	sprite.move(sf::Vector2(velocity.x, 0.f));
	//Gravity
	if (onPlatform == false)
	{
	 sprite.move({ 0.f, 0.3f });
	}

	handleCollision(platforms);

	//Animation States
	PlayerState newState;
	if (!onPlatform)
	{
		newState = PlayerState::Flying;
	}
	else if (!isMoving && std::abs(velocity.x) > 0.1f)
	{
		newState = PlayerState::Slowing;
	}
	else if (isMoving)
	{
		newState = PlayerState::Walking;
	}
	else
	{
		newState = PlayerState::Idle;
	}

	//Sets current state once instead of setting it every update, fixes issue sprite being invisible
	if (newState != currentState)
	{
		currentState = newState;
		
		switch (currentState)
		{
		case PlayerState::Idle:
			sprite.setTexture(idleTexture);
			sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 31, 41 }));
			break;
		case PlayerState::Slowing:
			sprite.setTexture(slowTexture);
			sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 31, 38 }));
			break;

		case PlayerState::Walking:
			sprite.setTexture(walkingTexture);
			sprite.setTextureRect(walkingFrames[currentWalkingFrame]);
			break;

		case PlayerState::Flying:
			sprite.setTexture(flyingTexture);
			sprite.setTextureRect(flyingFrames[currentFlyingFrame]);
			break;
		}
	}
	updateAnimation();
}

bool player::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	//std::cout << "Drawing player at: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
	return true;
}

void player::loadAnimation()
{
	const sf::Image walkingImage("assets/walkingSpritesheet.png");
	bool walkingResult = walkingTexture.loadFromImage(walkingImage, false, sf::IntRect({ 0, 0 }, { 93, 41 }));

	const sf::Image slowingImage("assets/slowing.png");
	bool slowingResult = slowTexture.loadFromImage(slowingImage, false, sf::IntRect({ 0, 0 }, { 31, 38 }));

	const sf::Image idleImage("assets/idle.png");
	bool idleResult = idleTexture.loadFromImage(idleImage, false, sf::IntRect({ 0, 0 }, { 31, 41 }));

	walkingFrames.push_back(sf::IntRect({ 0,0 }, { 31, 41 }));
	walkingFrames.push_back(sf::IntRect({ 31,0 }, { 31, 41 }));
	walkingFrames.push_back(sf::IntRect({ 62,0 }, { 31, 41 }));

	const sf::Image flyingImage("assets/flyingSpritesheet.png");
	bool flyingResult = flyingTexture.loadFromImage(flyingImage, false, sf::IntRect({ 0, 0 }, { 64, 28 }));

	flyingFrames.push_back(sf::IntRect({ 0,0 }, { 32, 28 }));
	flyingFrames.push_back(sf::IntRect({ 32,0 }, { 32, 28 }));
}

void player::updateAnimation()
{
	float frameTime = 0.1f;

	switch (currentState)
	{
	case PlayerState::Walking:
		if (animationClock.getElapsedTime().asSeconds() > frameTime)
		{
			currentWalkingFrame = (currentWalkingFrame + 1) % walkingFrames.size();
			sprite.setTextureRect(walkingFrames[currentWalkingFrame]);
			animationClock.restart();
		}
		break;
	case PlayerState::Flying:
		if (animationClock.getElapsedTime().asSeconds() > frameTime)
		{
			currentFlyingFrame = (currentFlyingFrame + 1) % flyingFrames.size();
			sprite.setTextureRect(flyingFrames[currentFlyingFrame]);
			animationClock.restart();
		}
		break;
	}
}

sf::Vector2f player::getPosition() const
{
	return sprite.getPosition();
}

void player::setPosition(float x, float y)
{
	sprite.setPosition(sf::Vector2(x, y));
}

void player::handleCollision(const std::vector<sf::Sprite>& platform)
{
	sf::Vector2f playerPos = sprite.getPosition();
	sf::Vector2f playerSize(31.f, 41.f);

	for (const auto& plat : platform) 
	{
		sf::Vector2f platformPos = plat.getPosition();
		sf::FloatRect platformSize = plat.getGlobalBounds();

		//top collision
		if (playerPos.y + playerSize.y > platformPos.y &&            // Player's bottom is below platform's top
			playerPos.y + playerSize.y - 5.f <= platformPos.y &&     // Player is just about to land on the platform
			playerPos.x + playerSize.x > platformPos.x &&            // Player horizontally overlaps platform
			playerPos.x < platformPos.x + platformSize.size.x)       // Player horizontally overlaps platform
		{
			// Snap the player to the top of the platform
			sprite.setPosition(sf::Vector2(playerPos.x, platformPos.y - playerSize.y));
			onPlatform = true;
			playerPos = sprite.getPosition();
			//std::cout << "Onplatform " << std::endl;
		}
		//Bottom Collision
		if (playerPos.y < platformPos.y + platformSize.size.y &&
			playerPos.y + 5.f > platformPos.y + platformSize.size.y &&
			playerPos.x + playerSize.x > platformPos.x &&
			playerPos.x < platformPos.x + platformSize.size.x)
		{
			sprite.setPosition(sf::Vector2(playerPos.x, platformPos.y + platformSize.size.y));
			playerPos = sprite.getPosition();
		}
		//Left Collision
		if (playerPos.x + playerSize.x > platformPos.x &&
			playerPos.x + playerSize.x - 5.f <= platformPos.x &&
			playerPos.y + playerSize.y > platformPos.y &&
			playerPos.y < platformPos.y + platformSize.size.y)
		{
			sprite.setPosition(sf::Vector2(platformPos.x - playerSize.x, playerPos.y));
			playerPos = sprite.getPosition();
		}
		//Right Collision
		if (playerPos.x < platformPos.x + platformSize.size.x &&
			playerPos.x + 5.f > platformPos.x + platformSize.size.x &&
			playerPos.y + playerSize.y > platformPos.y &&
			playerPos.y < platformPos.y + platformSize.size.y)
		{
			sprite.setPosition(sf::Vector2(platformPos.x + platformSize.size.x, playerPos.y));
			playerPos = sprite.getPosition();
		}
	}
}

sf::Vector2f player::getSize() const
{
	return sf::Vector2f(sprite.getTexture().getSize());
}

void player::reset()
{
	std::cout << "Reset player position" << std::endl;
	invincible = true;
	invincibleClock.restart();
	visible = true;
	sprite.setColor(sf::Color::White);
	sprite.setPosition(sf::Vector2f(235.f, 300.f));
}
