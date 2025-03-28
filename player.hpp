#pragma once

#include "SFML/Graphics.hpp"

class Player
{
	int distPerSecond = 100;
	float groundY = 350.f;
	bool onPlatform = false;
	//sf::Sprite player;
	sf::Vector2f position;
	std::unique_ptr<sf::Sprite> player;

public:
	Player();
	Player(sf::Texture & texture);
	sf::Vector2f getPosition();
	sf::Vector2f setPosition(sf::Vector2f position);

	void Update();

	void Render();
};