#include <SFML/Graphics.hpp>
#include "game.h"

#ifdef _CONSOLE
int main()
#else
int WinMain()
#endif
{
	game game;
	game.run();
}