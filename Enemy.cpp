#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const sf::Texture& texture,sf::Vector2f position) : sprite(texture), onPlatform(false)
{ 
	sf::Sprite enemy(texture);
	sprite = enemy;
	sprite.setTextureRect(sf::IntRect({ 32,0 }, { 32, 28 }));
	sprite.setPosition(position);
	velocity.x = 1.f;
	loadAnimation();
}

void Enemy::update(const std::vector<sf::Sprite>& platforms)
{
	onPlatform = false;
	sf::Vector2f enemyPos = getPosition();

	//Wrap screen
	if (enemyPos.x > 533.f) { enemyPos.x = -sprite.getGlobalBounds().size.length(); }
	else if (enemyPos.x + sprite.getGlobalBounds().size.length() < 0.f) {enemyPos.x = 533.f ; }

	setPosition(enemyPos);
	handleCollision(platforms);

	if (flapClock.getElapsedTime().asSeconds() >= flapCooldown)
	{
		int roll = rand() % 100;

		if (getPosition().y > bottomThreshhold)
		{
			currentState = flapDirection::Up;
		}
		else if (getPosition().y < topThreshhold)
		{
			currentState = flapDirection::Down;
		}
		else
		{
			if (roll < 50)
			{
				currentState = flapDirection::Up;
			}
			else if (roll > 50)
			{
				currentState = flapDirection::Down;
			}
			else
			{
				currentState = flapDirection::None;
			}

		}
		flapCooldown = static_cast<float>(rand() % 1000) / 1000.f + 0.4f;
		flapClock.restart();
		//std::cout << "Flap triggered! Next flap in: " << flapCooldown << " seconds." << std::endl;

		switch (currentState)
		{
		case flapDirection::Up:
			//sprite.setTexture(flyingTexture);
			velocity.y = -strength;
			break;
		case flapDirection::Down:
			velocity.y = strength;
			break;
		case flapDirection::None:
			velocity.y = 0.f;
			break;
		}	
		//if (getPosition().y > bottomThreshhold)
		//{
		//	velocity.y = -1.0f;
		//	flapCooldown = static_cast<float>(rand() % 400) / 1000.f + 0.3f;
		//}
		//else if (getPosition().y > middleThreshhold)
		//{
		//	velocity.y = -1.3f;
		//	flapCooldown = static_cast<float>(rand() % 800) / 1000.f + 0.5f;
		//}
		//else
		//{
		//	velocity.y = -0.6f;
		//	flapCooldown = static_cast<float>(rand() % 1500) / 1000.f + 1.0f;
		//}
		
	}
	sprite.move(velocity);
	/*velocity.y += 0.015f;
	if (velocity.y > 0.3f) {velocity.y = 0.3f;}*/

	if (getPosition().y < 0.f)
	{
		setPosition(sf::Vector2f(getPosition().x, 0.f));
	}
	updateAnimation();
}

void Enemy::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Enemy::bounce()
{
}

void Enemy::loadAnimation()
{
	//flyingTexture.loadFromFile("assets/EnemyflyingSpritesheet.png");

	flyingFrames.push_back(sf::IntRect({ 0,0 }, { 32, 28 }));
	flyingFrames.push_back(sf::IntRect({ 32,0 }, { 32, 28 }));
}

void Enemy::updateAnimation()
{
	float frameTime = 0.2f;
	if (animationClock.getElapsedTime().asSeconds() > frameTime)
	{
		currentFlyingFrame = (currentFlyingFrame + 1) % flyingFrames.size();
		sprite.setTextureRect(flyingFrames[currentFlyingFrame]);
		animationClock.restart();
	}
}

void Enemy::handleCollision(const std::vector<sf::Sprite>& platforms)
{
	sf::Vector2f enemyPos = getPosition();
	sf::Vector2f enemySize(32, 28);

	for (const auto& plat : platforms)
	{
		sf::Vector2f platformPos = plat.getPosition();
		sf::FloatRect platformSize = plat.getGlobalBounds();

		//Top Side 
		if (enemyPos.y + enemySize.y > platformPos.y &&
			enemyPos.y + enemySize.y - 5.f <= platformPos.y &&
			enemyPos.x + enemySize.x > platformPos.x &&
			enemyPos.x < platformPos.x + platformSize.size.x)
		{	
			onPlatform = true;
			sprite.setPosition(sf::Vector2(enemyPos.x, platformPos.y - enemySize.y));
			enemyPos = getPosition();
		}

		//Bottom Side
		if (enemyPos.y < platformPos.y + platformSize.size.y &&
			enemyPos.y + 5.f > platformPos.y + platformSize.size.y &&
			enemyPos.x + enemySize.x > platformPos.x &&
			enemyPos.x < platformPos.x + platformSize.size.x)
		{
			sprite.setPosition(sf::Vector2(enemyPos.x, platformPos.y + platformSize.size.y));
			enemyPos = getPosition();
		}

		//Left Side
		if (enemyPos.x + enemySize.x > platformPos.x &&
			enemyPos.x + enemySize.x - 5.f <= platformPos.x &&
			enemyPos.y + enemySize.y > platformPos.y &&
			enemyPos.y < platformPos.y + platformSize.size.y)
		{
			velocity.x = -velocity.x;
			sprite.setOrigin(sf::Vector2(sprite.getLocalBounds().size.x, 0.f));
			sprite.setScale(sf::Vector2f(-1.f, 1.f));
			enemyPos = getPosition();
		}

		//Right Side
		if (enemyPos.x < platformPos.x + platformSize.size.x &&
			enemyPos.x + 5.f > platformPos.x + platformSize.size.x &&
			enemyPos.y + enemySize.y > platformPos.y &&
			enemyPos.y < platformPos.y + platformSize.size.y)
		{
			velocity.x = -velocity.x;
			sprite.setOrigin(sf::Vector2(0.f, 0.f));
			sprite.setScale(sf::Vector2f(1.f, 1.f));
			enemyPos = getPosition();
		}
	}
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
