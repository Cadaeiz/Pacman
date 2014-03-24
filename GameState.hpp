#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <SFML/Graphics.hpp>

class Game;

class GameState
{

public:
	GameState();
	virtual ~GameState();

	virtual void handleEvent(Game & game, sf::Event e) = 0;
	virtual void run(Game & game, float time) = 0;
	virtual void draw(Game & game, sf::RenderWindow & window) = 0;

	virtual void init(Game & game) = 0;
	virtual void cleanup(Game & game) = 0;

	void changeState(GameState * state);
};

class StartState : public GameState 
{
public:
	StartState();
	virtual ~StartState();

	virtual void handleEvent(Game & game, sf::Event e);
	virtual void runf(Game & game, float time);
	virtual void draw(Game & game, sf::RenderWindow & window);

	virtual void init(Game & game);
	virtual void cleanup(Game & game);
};

#endif
