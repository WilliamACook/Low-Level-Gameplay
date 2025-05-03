#pragma once
#include <SFML/Graphics.hpp>

enum class flapDirection {None, Up, Down};

class Enemy
{
private:
	sf::Sprite sprite;
	bool onPlatform;
	sf::Vector2f velocity;

	sf::Clock flapClock;
	float flapCooldown = 0.f;
	float strength = 0.2f;
	float bottomThreshhold = 320.f;
	float middleThreshhold = 200.f;
	float topThreshhold = 80.f;
	flapDirection currentState = flapDirection::None;

	sf::Texture flyingTexture;
	std::vector<sf::IntRect> flyingFrames;
	int currentFlyingFrame = 0;
	sf::Clock animationClock;

public:
	Enemy(const sf::Texture& texture,sf::Vector2f position);

	void update(const std::vector<sf::Sprite>& platforms);
	void draw(sf::RenderWindow& window);
	void bounce();
	void loadAnimation();
	void updateAnimation();
	void handleCollision(const std::vector<sf::Sprite>& platforms);
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);
	sf::Vector2f getSize() const;
};
