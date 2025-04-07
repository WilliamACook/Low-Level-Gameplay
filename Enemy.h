#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Enemy(sf::Image texturesrc, sf::Vector2f position);

	void update();
	void draw(sf::RenderWindow& window);
	void bounce();
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getSize() const;
};

