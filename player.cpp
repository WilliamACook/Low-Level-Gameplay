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

void player::update(sf::RenderWindow & window, const std::vector<sf::Sprite>& platforms)
{
	sf::Vector2f playerPos = sprite.getPosition();
	onPlatform = false;
	float moveSpeed = 1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		float x = sprite.getPosition().x - moveSpeed;
		float y = sprite.getPosition().y;
		if (x < .0f) { x = 400.0f; }
		sprite.setPosition(sf::Vector2f{ x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
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

	//Gravity
	if (onPlatform == false)
	{
	 sprite.move({ 0.f, 0.3f });
	}
}

bool player::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	//std::cout << "Drawing player at: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
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

	for (const auto& plat : platform) 
	{
		sf::Vector2f platformPos = plat.getPosition();
		sf::Vector2 platformSize = plat.getTexture().getSize();

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
		if (playerPos.y < platformPos.y + platformSize.y &&          
			playerPos.y + 5.f > platformPos.y + platformSize.y &&    
			playerPos.x + playerSize.x > platformPos.x &&            
			playerPos.x < platformPos.x + platformSize.x)            
		{
			sprite.setPosition(sf::Vector2(playerPos.x, platformPos.y + platformSize.y));
		}
		//Left Collision
		if (!onPlatform &&                                           
			playerPos.x + playerSize.x > platformPos.x &&            
			playerPos.x + playerSize.x - 5.f <= platformPos.x &&     
			playerPos.y + playerSize.y > platformPos.y &&            
			playerPos.y < platformPos.y + platformSize.y)            
		{
			sprite.setPosition(sf::Vector2(platformPos.x - playerSize.x, playerPos.y));
		}
		//Right Collision
		if (!onPlatform &&                                           
			playerPos.x < platformPos.x + platformSize.x &&          
			playerPos.x + 5.f > platformPos.x + platformSize.x &&    
			playerPos.y + playerSize.y > platformPos.y &&            
			playerPos.y < platformPos.y + platformSize.y)            
		{
			sprite.setPosition(sf::Vector2(platformPos.x + platformSize.x, playerPos.y));
		}
	}
}

sf::Vector2f player::getSize() const
{
	return sf::Vector2f(sprite.getTexture().getSize());
}

void player::reset()
{
	std::cout << "Resetting player position..." << std::endl;
	sprite.setPosition(sf::Vector2f(200.f, 180.f));
}
