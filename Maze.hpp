#ifndef MAZE_H_
#define MAZE_H_

#include <SFML/Graphics.hpp>
using std::string;

class Maze
{
private:
    //bgTexture: image appearing below the walls
    //wallTexture: contains all possible wall sprites
    //mazeTexture: contains image of walls within the maze
    sf::Texture bgTexture, wallTexture, mazeTexture;
    sf::Sprite sprite;
    //array of wall bits; every 2 bits represents the walls
    //above and to the left of a square
    struct node
    {
        unsigned char up : 1;
        unsigned char left : 1;
        unsigned char down : 1;
        unsigned char right : 1;
    } * walls;
    sf::Vector2i size;
    sf::Vector2f squareSize;

    void addWall(int x, int y, int direction);
    void removeWall(int x, int y, int direction);
    bool isValid(int x, int y, int direction);
    float L1(sf::Vector2i * p1, sf::Vector2i * p2);

public:
    const static int up     = 0,
                     left   = 1,
                     down   = 2,
                     right  = 3;

    ~Maze();
    Maze();
    void setTextures(const string & name = "");
    bool isWall(int x, int y, int direction);
    bool isWall(const sf::Vector2i & pos, int direction) { return isWall(pos.x,pos.y,direction); }
    int degree(int x, int y);
    int degree(const sf::Vector2i & pos) { return degree(pos.x,pos.y); }
    sf::Vector2f getTruePosition(const sf::Vector2i & gridPosition);
    sf::Vector2i getGridPosition(const sf::Vector2f & truePosition);
    bool contains(const sf::Vector2i & pos);
    void generate(int x, int y);
    sf::Vector2i getSize() { return size; }

    void draw(sf::RenderWindow & window);
};

#endif // MAZE_H_
