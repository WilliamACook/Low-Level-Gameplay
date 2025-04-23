#pragma once
#include <SFML/Graphics.hpp>

enum class GameState { MainMenu, Playing, Paused, GameOver};

class player;

class GameStateManager
{
private:
	GameState currentState;
	sf::Font font;
	sf::Font titleFont;
	sf::Text title, playButton, optionsButton, exitButton, paused, lives;
	player* playerRef = nullptr;
	sf::Texture logoText;
	sf::Sprite sp_Logo;
	sf::Texture buttonOffText;
	sf::Texture buttonOnText;
	sf::Sprite sp_PlayButton;
	sf::Sprite sp_QuitButton;

public:
	GameStateManager();

	void handleInput(sf::RenderWindow& window);
	void setPlayerReference(player* p);
	void draw(sf::RenderWindow& window);
	void restartGame();

	GameState getState() const { return currentState; }
	void setState(GameState state) { currentState = state; }
};

