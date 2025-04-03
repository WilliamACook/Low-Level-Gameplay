#pragma once
#include <SFML/Graphics.hpp>

enum class GameState { MainMenu, Playing, Paused, GameOver};

class GameStateManager
{
private:
	GameState currentState;
	sf::Font font;
	sf::Text title, playButton, optionsButton, exitButton;

public:
	GameStateManager();

	void handleInput(sf::RenderWindow& window);
	void update();
	void draw(sf::RenderWindow& window);

	GameState getState() const { return currentState; }
	void seteState(GameState state) { currentState = state; }
};

