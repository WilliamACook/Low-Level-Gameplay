#pragma once
#include <SFML/Graphics.hpp>

namespace LLGP
{
	class collision
	{
	private:
		sf::Vector2f platformPos;
		sf::Vector2f platformSize;
		sf::Vector2f playerPos;
		sf::Vector2f playerSize;

	public:
		collision();
		collision(const sf::Vector2f platformPos, sf::Vector2f platformSize, sf::Vector2f playerPos, sf::Vector2f playerSize);
		bool checkCollision();
	};
}


