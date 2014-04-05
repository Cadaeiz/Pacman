#ifndef GHOST_H_
#define GHOST_H_

#include <SFML/Graphics.hpp>
#include "Maze.hpp"
#include "Vector.hpp"

class Ghost
{
private:
    Vector<sf::Vector2i> AStar(const sf::Vector2i & start, const sf::Vector2i & goal);
    int L1distance(const sf::Vector2i & p1, const sf::Vector2i & p2);
    int L1distance(int x1, int y1, int x2, int y2);
    struct Node
    {
        int x, y, g;
        int parent;
        Node(int x, int y, int g, int parent) : x(x),y(y),g(g),parent(parent) { }
    };
    Maze & maze;
public:
    Ghost(Maze & maze) : maze(maze) { }
    Vector<sf::Vector2i> path;

    void handleEvent(sf::Event e);
    void draw(sf::RenderWindow & window);
};
#endif // GHOST_H_
