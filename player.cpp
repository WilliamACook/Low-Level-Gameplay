#include "player.hpp"

Player::Player() {}

Player::Player(sf::Texture & texture) : player(texture)
{
	player = std::make_unique<sf::Sprite>(texture, sf::IntRect(sf::Vector2i{ 0,0 }, { textureSize }));
}

sf::Vector2f Player::getPosition()
{
	return sf::Vector2f();
}

sf::Vector2f Player::setPosition(sf::Vector2f position)
{
	this->position = position;
	player->setPosition(this->position);

	return this->position;
}

void Player::Update()
{
	float shiftDistance = 0.1;
	//sf::FloatRect newposition = player.getGlobalBounds();

	//Controls
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		//spritey.move({ -shiftDistance, 0.f });

	{
		float x = player.getPosition().x - shiftDistance;
		float y = player.getPosition().y;
		if (x < .0f) { x = 400.0f; }
		player.setPosition(sf::Vector2f{ x,y });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		//spritey.move({ shiftDistance, 0.f });
	{
		float x = player.getPosition().x + shiftDistance;
		float y = player.getPosition().y;
		if (x > 400.0f) { x = .0f; }
		player.setPosition(sf::Vector2f{ x,y });
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		float x = player.getPosition().x;
		float y = player.getPosition().y - shiftDistance;
		player.setPosition(sf::Vector2f{ x,y });
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !onPlatform)
	{
		float x = player.getPosition().x;
		float y = player.getPosition().y + shiftDistance;
		player.setPosition(sf::Vector2f{ x,y });
	}

	//Gravity
	if (onPlatform == false)
	{
		player.move({ 0.f, 0.1f });
	}

	
}

void Player::Render()
{

}