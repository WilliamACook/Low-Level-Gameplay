#include "GameStateManager.h"
#include <iostream>

GameStateManager::GameStateManager() : title(font), playButton(font), optionsButton(font), exitButton(font), paused(font)
{
	if (!font.openFromFile("assets/Roboto.ttf"))
		std::cout << "Failed to load font!" << std::endl;

	if (!titleFont.openFromFile("assets/titleFont.ttf"))
		std::cout << "Failed to load font!" << std::endl;

	title.setFont(titleFont);
	title.setString("Joust");
	title.setCharacterSize(50);
	title.setPosition(sf::Vector2f(125.f, 50.f));

	playButton.setFont(font);
	playButton.setString("Play");
	playButton.setCharacterSize(30);
	playButton.setPosition(sf::Vector2f(175.f, 150.f));

	exitButton.setFont(font);
	exitButton.setString("Quit");
	exitButton.setCharacterSize(30);
	exitButton.setPosition(sf::Vector2f(175.f, 250.f));

	paused.setFont(titleFont);
	paused.setString("Paused");
	paused.setCharacterSize(50);
	paused.setPosition(sf::Vector2f(125.f, 50.f));

	playButtonBox.setSize(sf::Vector2f(150.f, 40.f));
	playButtonBox.setPosition(sf::Vector2f(125.f, 150.f));
	playButtonBox.setFillColor(sf::Color::Blue);
	playButtonBox.setOutlineThickness(2.f);
	playButtonBox.setOutlineColor(sf::Color::White);

	exitButtonBox.setSize(sf::Vector2f(150.f, 40.f));

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
	

	playButton.setFillColor(sf::Color::White);
	playButtonBox.setSize(sf::Vector2f(150.f, 40.f));
	playButtonBox.setPosition(sf::Vector2f(125.f, 150.f));
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (playButtonBox.getGlobalBounds().contains(mousePos))
	{
		//playButtonBox.setFillColor(sf::Color::Cyan);
		playButton.setFillColor(sf::Color::Red);
		playButtonBox.setSize(sf::Vector2f(154.f, 44.f));
		playButtonBox.setPosition(sf::Vector2f(123.f, 148.f));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
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

	if (currentState == GameState::Paused)
	{
		playButton.setString("Resume");
		playButton.setPosition(sf::Vector2f(150.f, 150.f));
		window.draw(paused);
		window.draw(playButtonBox);
		window.draw(playButton);
		window.draw(exitButton);
	}
}
