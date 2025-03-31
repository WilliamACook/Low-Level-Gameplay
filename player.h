#include <SFML/Graphics.hpp>

#pragma once
class player
{
private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	bool onPlatform;
public:
	player(float x, float y, const sf::Image texturesrc);

	void update();

	bool draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;

	void setPosition(float x, float y);

	void handleCollision(const std::vector<sf::Sprite>& platform);
};

