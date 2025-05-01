#include "GameStateManager.h"
#include <iostream>
#include "player.h"

GameStateManager::GameStateManager() : title(font), playButton(font), optionsButton(font), exitButton(font), paused(font), lives(font), sp_Logo(logoText), sp_PlayButton(buttonOffText), sp_QuitButton(buttonOffText)
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
	playButton.setOutlineColor(sf::Color::Black);
	playButton.setOutlineThickness(2.f);
	playButton.setPosition(sf::Vector2f(236.f, 155.f));
	

	exitButton.setFont(font);
	exitButton.setString("Quit");
	exitButton.setCharacterSize(30);
	exitButton.setOutlineColor(sf::Color::Black);
	exitButton.setOutlineThickness(2.f);
	exitButton.setPosition(sf::Vector2f(236.f, 250.f));

	paused.setFont(titleFont);
	paused.setString("Paused");
	paused.setCharacterSize(50);
	paused.setPosition(sf::Vector2f(166.f, 50.f));

	const sf::Image joustLogo("assets/joustLogo.png");
	bool logoResult = logoText.loadFromImage(joustLogo, false, sf::IntRect({ 0, 0 }, { 170, 40 }));
	sf::Sprite logoTemp(logoText);
	sp_Logo = logoTemp;
	sp_Logo.setScale(sf::Vector2f(2.f, 2.f));
	sp_Logo.setPosition({ 99.f, 30.f });

	const sf::Image buttonOff("assets/button_Off.png");
	bool buttonResult = buttonOffText.loadFromImage(buttonOff, false, sf::IntRect({ 0, 0 }, { 512, 256 }));
	sf::Sprite buttonTemp(buttonOffText);
	sp_PlayButton = buttonTemp;
	sp_PlayButton.setScale(sf::Vector2f(0.3f, 0.2f));
	sp_PlayButton.setPosition(sf::Vector2f(189.f, 150.f));

	sp_QuitButton = buttonTemp;
	sp_QuitButton.setScale(sf::Vector2f(0.3f, 0.2f));
	sp_QuitButton.setPosition(sf::Vector2f(189.f, 245.f));
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

	sp_PlayButton.setScale(sf::Vector2f(0.3f, 0.2f));
	sp_PlayButton.setPosition(sf::Vector2f(189.f, 150.f));
	sp_QuitButton.setScale(sf::Vector2f(0.3f, 0.2f));
	sp_QuitButton.setPosition(sf::Vector2f(189.f, 245.f));
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (currentState != GameState::Playing)
	{
		if (sp_PlayButton.getGlobalBounds().contains(mousePos))
		{
			sp_PlayButton.setScale(sf::Vector2f(0.33f, 0.22f));
			sp_PlayButton.setPosition(sf::Vector2f(181.5f, 148.f));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				restartGame();
				playerRef->setLifes(3);
				currentState = GameState::Playing;
			}
		}

		if (sp_QuitButton.getGlobalBounds().contains(mousePos))
		{
			sp_QuitButton.setScale(sf::Vector2f(0.33f, 0.22f));
			sp_QuitButton.setPosition(sf::Vector2f(181.5f, 243.f));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				window.close();
			}
		}
	}
}

void GameStateManager::setPlayerReference(player* p)
{
	playerRef = p;
}

void GameStateManager::draw(sf::RenderWindow& window)
{
	if (currentState == GameState::MainMenu)
	{
		window.draw(sp_Logo);
		window.draw(sp_PlayButton);
		window.draw(playButton);
		window.draw(sp_QuitButton);
		window.draw(exitButton);
	}

	if (currentState == GameState::Paused)
	{
		playButton.setString("Resume");
		playButton.setPosition(sf::Vector2f(211.f, 155.f));
		window.draw(paused);
		window.draw(sp_PlayButton);
		window.draw(sp_QuitButton);
		window.draw(playButton);
		window.draw(exitButton);
	}

	if (currentState == GameState::GameOver)
	{
		window.clear();
		playButton.setString("Restart");
		playButton.setPosition(sf::Vector2f(216.f, 155.f));
		title.setString("Game Over");
		title.setPosition(sf::Vector2f(126.f, 50.f));
		window.draw(title);
		window.draw(sp_PlayButton);
		window.draw(playButton);
		window.draw(sp_QuitButton);
		window.draw(exitButton);
	}

	//Moved this to game cpp with new sprites instead of text
	//if (currentState == GameState::Playing)
	//{
	//	window.clear();
	//	lives.setFont(font);
	//	lives.setString("Lives: " + std::to_string(playerRef->getLives()));
	//	lives.setPosition(sf::Vector2f(20.f, 20.f));
	//	window.draw(lives);
	//}
	
}

void GameStateManager::restartGame()
{
	//std::cout << "Player position after reset: " << player.getPosition().x << std::endl;
}
