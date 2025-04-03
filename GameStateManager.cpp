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
	playButton.setString("Play");
	playButton.setCharacterSize(30);
	playButton.setPosition(sf::Vector2f(100.f, 150.f));

	exitButton.setFont(font);
	exitButton.setString("Press Escape to Quit");
	exitButton.setCharacterSize(30);
	exitButton.setPosition(sf::Vector2f(100.f, 200.f));

	playButtonBox.setSize(sf::Vector2f(150.f, 40.f));
	playButtonBox.setPosition(sf::Vector2f(100.f, 150.f));
	playButtonBox.setFillColor(sf::Color::Blue);
	playButtonBox.setOutlineThickness(2.f);
	playButtonBox.setOutlineColor(sf::Color::White);

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

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (playButtonBox.getGlobalBounds().contains(mousePos))
		{
			currentState = GameState::Playing;
		}
	}
}

void GameStateManager::draw(sf::RenderWindow& window)
{
	if (currentState == GameState::MainMenu)
	{
		window.draw(title);
		window.draw(playButtonBox);
		window.draw(playButton);
		window.draw(exitButton);
		
	}
}
