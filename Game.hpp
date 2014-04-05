#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include "GameState.hpp"
#include "Ghost.hpp"

class Game
{
	friend class StartState;
	friend class ExitState;

	const static int START	 = 0,
                     EXIT    = 1;

private:
	static struct Machine
	{
		StartState start;
		ExitState exit;
	} StateMachine;
	GameState * currentState;
	void changeState(int state);
	Ghost ghost;
	Maze maze;

public:
    Game() : ghost(maze) { }
	const static int ScreenWidth = 800, ScreenHeight = 600;
	void init();
	void cleanup();

	void handleEvent(sf::Event e) { currentState -> handleEvent(*this, e); }
	void run(float time) { currentState -> run(*this, time); }
	void draw(sf::RenderWindow & window) { currentState -> draw(*this, window); }
	bool exited() { return currentState == &StateMachine.exit; }
};

#endif
