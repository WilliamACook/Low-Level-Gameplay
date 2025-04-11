#include <SFML/Graphics.hpp>

#pragma once
class player
{
private:
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	int lives = 3;
	bool onPlatform;
	bool just_jumped = true;

public:
	player(float x, float y, const sf::Image texturesrc);

	void update(sf::RenderWindow& window, const std::vector<sf::Sprite>& platforms);

	bool draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;

	void setPosition(float x, float y);

	int getLives() const { return lives; }
	void loseLife() { lives--; }
	void setLifes(int numberOfLives) {lives = numberOfLives;}

	void handleCollision(const std::vector<sf::Sprite>& platform);
	sf::Vector2f getSize() const;
	void reset();
};

