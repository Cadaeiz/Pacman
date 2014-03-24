#include "Game.hpp"
#include "GameState.hpp"

void Game::init()
{

}


void Game::cleanup()
{
}


void Game::changeState(int state)
{
}


void Game::handleEvent(sf::Event e)
{
	states[currentState] -> handleEvent(*this, e);
}

void Game::run(float time)
{
	states[currentState] -> run(*this, time);
}


void Game::draw(sf::RenderWindow & window)
{
	states[currentState] -> draw(*this, window);
}
