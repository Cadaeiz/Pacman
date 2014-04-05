#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <sstream>


/* fixed fps (both virtual and actual) */
#define FPS 60
#define PERIOD (1.0/FPS)

#define VFPS 120
#define VPER (1.0/VFPS)

int main()
{
	sf::RenderWindow window(sf::VideoMode(Game::ScreenWidth, Game::ScreenHeight), "Pacman");
	sf::Clock clock1, clock2;
	sf::Time elapsed1, elapsed2;

	/* for writing fps on screen while debugging */
	sf::Font font;
	font.loadFromFile("text/arial.ttf");
	sf::Text fps;
	fps.setFont(font);
	fps.setCharacterSize(15);
	fps.setColor(sf::Color::Green);

	sf::Text vfps = sf::Text(fps);
	vfps.setPosition(0,15);

	std::stringstream ss;

	/* start up game */
	Game game;
	game.init();


	while (window.isOpen())
	{
		elapsed1 = clock1.getElapsedTime();
		if (elapsed1.asSeconds() >= VPER)
		{
			ss.str("");
			ss << "VFPS:\t\t" << (1 / elapsed1.asSeconds());
			vfps.setString(ss.str());

			game.run(elapsed1.asSeconds());
			clock1.restart();
		}

		elapsed2 = clock2.getElapsedTime();
		/* wait 1/FPS seconds to rewrite to screen */
		if (elapsed2.asSeconds() >= PERIOD)
		{
			ss.str("");
			ss << "FPS:\t\t  " << (1 / elapsed2.asSeconds());
			fps.setString(ss.str());

			clock2.restart();
			window.clear();
			game.draw(window);
			window.draw(fps);
			window.draw(vfps);
			window.display();
		}


		sf::Event e;
		while (window.pollEvent(e))
		{
			switch(e.type)
			{
			case sf::Event::Closed:
				game.cleanup();
				window.close();
				break;
			default:
				game.handleEvent(e);
			}
		}

		if (game.exited())
		{
			game.cleanup();
			window.close();
		}

	}

	return 0;
}
