#include "Enemy.h"

Enemy::Enemy(sf::Image texturesrc, sf::Vector2f position) : texture(), sprite(texture)
{ 
	bool result = texture.loadFromImage(texturesrc, false, sf::IntRect({ 0,0 }, { 24,24 }));
	
	sf::Sprite enemy(texture);
	sprite = enemy;
	sprite.setPosition(position);
}

void Enemy::update()
{
	float speed = 1.f;
	sprite.move(sf::Vector2f(speed, 0.f));

	//Wrap screen
	if (sprite.getPosition().x > 400.f)
	{
		sprite.setPosition({ -10, 0 });
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Enemy::bounce()
{
}

sf::Vector2f Enemy::getPosition() const
{
	return sprite.getPosition();
}

void Enemy::setPosition(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

sf::Vector2f Enemy::getSize() const
{
	return sf::Vector2f(sprite.getTexture().getSize());
}
