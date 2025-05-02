#include "Pickup.h"

Pickup::Pickup(const sf::Texture& texture, sf::Vector2f pos) : sprite(texture), onPlatform(false)
{
	sprite.setTexture(texture);
	sprite.setPosition(pos);
}

void Pickup::update()
{
	sprite.move({ 0.f, 0.6f });
}

void Pickup::handleCollision(const std::vector<sf::Sprite>& platforms)
{
	sf::Vector2f pickupPos = sprite.getPosition();
	sf::Vector2f pickupSize(12, 14);
	onPlatform = false;

	for (const auto& plat : platforms)
	{
		sf::Vector2f platformPos = plat.getPosition();
		sf::FloatRect platformSize = plat.getGlobalBounds();

		//Top Side 
		if (pickupPos.y + pickupSize.y > platformPos.y &&
			pickupPos.y + pickupSize.y - 1.f <= platformPos.y &&
			pickupPos.x + pickupSize.x > platformPos.x &&
			pickupPos.x < platformPos.x + platformSize.size.x)
		{
			onPlatform = true;
			sprite.setPosition(sf::Vector2(pickupPos.x, platformPos.y - pickupSize.y));
			pickupPos = sprite.getPosition();
		}
	}
}

void Pickup::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
