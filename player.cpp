#include "player.h"
#include <iostream>

player::player(float x, float y, const sf::Image texturesrc) : texture(), sprite(texture)
{
	bool result = texture.loadFromImage(texturesrc, false, sf::IntRect({ 0,0 }, { 24,24 }));

	sf::Sprite player(texture);
	sprite = player;
	sprite.setPosition(sf::Vector2f(x, y));
	
	onPlatform = false;
}

void player::update()
{
	sf::Vector2f playerPos = sprite.getPosition();
	onPlatform = false;
	float moveSpeed = 1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		//spritey.move({ -shiftDistance, 0.f });

	{
		float x = sprite.getPosition().x - moveSpeed;
		float y = sprite.getPosition().y;
		if (x < .0f) { x = 400.0f; }
		sprite.setPosition(sf::Vector2f{ x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		//spritey.move({ shiftDistance, 0.f });
	{
		float x = sprite.getPosition().x + moveSpeed;
		float y = sprite.getPosition().y;
		if (x > 400.0f) { x = .0f; }
		sprite.setPosition(sf::Vector2f{ x,y });
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
}

bool player::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	std::cout << "Drawing player at: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
	return true;
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
	sf::Vector2f playerSize(24.f, 24.f);

	for (const auto& plat : platform) {
		sf::Vector2f platformPos = plat.getPosition();
		sf::Vector2f platformSize(24.f, 24.f);
		//top collision
		if (playerPos.y + playerSize.y > platformPos.y &&            // Player's bottom is below platform's top
			playerPos.y + playerSize.y - 5.f <= platformPos.y &&     // Player is just about to land on the platform
			playerPos.x + playerSize.x > platformPos.x &&            // Player horizontally overlaps platform
			playerPos.x < platformPos.x + platformSize.x)            // Player horizontally overlaps platform
		{
			// Snap the player to the top of the platform
			sprite.setPosition(sf::Vector2(playerPos.x, platformPos.y - playerSize.y));
			onPlatform = true;
		}
		//Bottom Collision
		if (playerPos.y < platformPos.y + platformSize.y &&          // Player's top is above platform's bottom
			playerPos.y + 5.f > platformPos.y + platformSize.y &&    // Player is just about to hit the bottom of the platform
			playerPos.x + playerSize.x > platformPos.x &&            // Player horizontally overlaps platform
			playerPos.x < platformPos.x + platformSize.x)            // Player horizontally overlaps platform
		{
			// Snap the player to the bottom of the platform
			sprite.setPosition(sf::Vector2(playerPos.x, platformPos.y + platformSize.y));
		}
		//Left Collision
		if (!onPlatform &&                                           // Only check if the player is not standing on the platform
			playerPos.x + playerSize.x > platformPos.x &&            // Player's right side is beyond platform's left side
			playerPos.x + playerSize.x - 5.f <= platformPos.x &&     // Player is just about to hit the left side
			playerPos.y + playerSize.y > platformPos.y &&            // Player vertically overlaps platform
			playerPos.y < platformPos.y + platformSize.y)            // Player vertically overlaps platform
		{
			// Snap the player to the left of the platform
			sprite.setPosition(sf::Vector2(platformPos.x - playerSize.x, playerPos.y));
		}
		//Right Collision
		if (!onPlatform &&                                           // Only check if the player is not standing on the platform
			playerPos.x < platformPos.x + platformSize.x &&          // Player's left side is beyond platform's right side
			playerPos.x + 5.f > platformPos.x + platformSize.x &&    // Player is just about to hit the right side
			playerPos.y + playerSize.y > platformPos.y &&            // Player vertically overlaps platform
			playerPos.y < platformPos.y + platformSize.y)            // Player vertically overlaps platform
		{
			// Snap the player to the right of the platform
			sprite.setPosition(sf::Vector2(platformPos.x + platformSize.x, playerPos.y));
		}
	}
}
