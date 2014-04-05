#include "GameState.hpp"
#include "Game.hpp"

void StartState::init(Game & game)
{
    game.maze.setTextures();
    game.maze.generate(20,15);
}

void StartState::run(Game & game, float time)
{

}

void StartState::handleEvent(Game & game, sf::Event e)
{
    game.ghost.handleEvent(e);
}

void StartState::draw(Game & game, sf::RenderWindow & window)
{
    game.maze.draw(window);
    game.ghost.draw(window);
}

void StartState::cleanup(Game & game)
{

}

void ExitState::init(Game & game)
{

}
