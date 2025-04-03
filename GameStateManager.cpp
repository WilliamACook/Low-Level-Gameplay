#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager() : title(font), playButton(font), optionsButton(font), exitButton(font)
{
	if (!font.openFromFile("assets/Roboto.ttf"))
		std::cout << "Failed to load font!" << std::endl;

	title.setFont(font);
	title.setString("Joust");
	title.setCharacterSize(50);
	title.setPosition(sf::Vector2f(150.f, 50.f));

	playButton.setFont(font);
	playButton.setString("Press Enter to Play");
	playButton.setCharacterSize(30);
	playButton.setPosition(sf::Vector2f(100.f, 150.f));

	exitButton.setFont(font);
	exitButton.setString("Press Escape to Quit");
	exitButton.setCharacterSize(30);
	exitButton.setPosition(sf::Vector2f(100.f, 200.f));

	currentState = GameState::MainMenu;
}

void GameStateManager::handleInput(sf::RenderWindow& window)
{
	if (currentState == GameState::MainMenu)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			currentState = GameState::Playing;
		}
	}
	if (currentState == GameState::Playing)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		{
			currentState = GameState::Paused;
		}
	}

}

void GameStateManager::draw(sf::RenderWindow& window)
{
	if (currentState == GameState::MainMenu)
	{
		window.draw(title);
		window.draw(playButton);
		window.draw(exitButton);
	}
}
