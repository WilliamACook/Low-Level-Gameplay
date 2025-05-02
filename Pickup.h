#pragma once
#include <SFML/Graphics.hpp>

class Pickup
{
private:
	sf::Sprite sprite;
	sf::Vector2f velocity;
	bool onPlatform;
public:
	Pickup(const sf::Texture& texture, sf::Vector2f pos);
	void update();
	void handleCollision(const std::vector<sf::Sprite>& platforms);
	void draw(sf::RenderWindow& window);
	sf::FloatRect getBounds() { return sprite.getGlobalBounds(); }
};

