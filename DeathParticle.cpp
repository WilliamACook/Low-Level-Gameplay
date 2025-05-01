#include "DeathParticle.h"

DeathParticle::DeathParticle(const sf::Texture& texture, sf::Vector2f pos) : sprite(texture)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect({ 0, 0 }, { 22, 24 }));
	sprite.setPosition(pos);

}

bool DeathParticle::update()
{
	if (clock.getElapsedTime().asSeconds() > frameTime && frame == 0)
	{
		sprite.setTextureRect(sf::IntRect({ 22, 0 }, { 22, 24 }));
		frame++;
		clock.restart();
	}
	if (frame == 1 && clock.getElapsedTime().asSeconds() > frameTime)
	{
		return false;
	}
	return true;
}

void DeathParticle::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
