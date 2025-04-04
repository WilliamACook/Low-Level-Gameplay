#pragma once
#include <SFML/Graphics.hpp>

enum class GameState { MainMenu, Playing, Paused, GameOver};

class GameStateManager
{
private:
	GameState currentState;
	sf::Font font;
	sf::Font titleFont;
	sf::Text title, playButton, optionsButton, exitButton, paused;
	sf::RectangleShape playButtonBox;
	sf::RectangleShape exitButtonBox;

public:
	GameStateManager();

	void handleInput(sf::RenderWindow& window);
	void update();
	void draw(sf::RenderWindow& window);

	GameState getState() const { return currentState; }
	void seteState(GameState state) { currentState = state; }
};

