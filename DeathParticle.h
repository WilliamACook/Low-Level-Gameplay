#pragma once
#include <SFML/Graphics.hpp>

class DeathParticle
{
private:
	sf::Sprite sprite;
	sf::Clock clock;
	int frame = 0;
	float frameTime = 0.2f;
	float lifeTime = 0.4f;
public:
	DeathParticle(const sf::Texture& texture, sf::Vector2f pos);
	bool update();
	void draw(sf::RenderWindow& window);

};

