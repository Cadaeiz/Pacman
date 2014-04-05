#include "Game.hpp"


struct Game::Machine Game::StateMachine;

void Game::init()
{
    currentState = 0;
    changeState(START);
}


void Game::cleanup()
{
}


void Game::changeState(int state)
{
    if (currentState)
        currentState -> cleanup(*this);

    switch(state)
    {
    case START:
        currentState = &StateMachine.start;
        break;
    case EXIT:
        currentState = &StateMachine.exit;
        break;
    }

    currentState -> init(*this);
}

