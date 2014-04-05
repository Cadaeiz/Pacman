#include "Maze.hpp"
#include "Heap.hpp"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>

Maze::Maze() : walls(0), size(0,0), squareSize(0,0)
{
    srand(time(0));
    bgTexture.setRepeated(true);
}

Maze::~Maze()
{
    if (walls)
        delete [] walls;
}

void Maze::setTextures(const string & name)
{
    bgTexture.loadFromFile("images/mazebg" + name + ".png");
    wallTexture.loadFromFile("images/mazewall" + name + ".png");

    //find the dimensions of a single square
    squareSize.x = wallTexture.getSize().x / 4;
    squareSize.y = wallTexture.getSize().y / 4;
}

bool Maze::isWall(int x, int y, int direction)
{
    //if x is just out of frame, wrap it around maze
    //assume that any call is never too far out of frame
    if (x < 0)
        x += size.x;
    else if (x >= size.x)
        x -= size.x;

    if (y < 0)
        y += size.y;
    else if (y >= size.y)
        y -= size.y;

    //find location of square in walls, and look up proper direction
    int index = x*size.y + y;
    switch (direction)
    {
    case up:
        return walls[index].up;
    case down:
        return walls[index].down;
    case left:
        return walls[index].left;
    case right:
        return walls[index].right;
    default:
        return false;
    }
}

int Maze::degree(int x, int y)
{
    int count = 4;
    //each wall removes an edge from the position
    for (int i = 0; i < 4; i++)
        if (isWall(x, y, i))
            count--;

    return count;
}

sf::Vector2f Maze::getTruePosition(const sf::Vector2i & gridPos)
{
    //start in top left corner
    sf::Vector2f position = sprite.getPosition();

    //move to center of given square
    position.x += (gridPos.x + .5) * squareSize.x;
    position.y += (gridPos.y + .5) * squareSize.y;

    return position;
}

sf::Vector2i Maze::getGridPosition(const sf::Vector2f & truePosition)
{
    //find position relative to the maze position
    sf::Vector2f position = truePosition - sprite.getPosition();

    //divide by the squareSize and round down to get which square it is in
    return sf::Vector2i(position.x / squareSize.x,
                        position.y / squareSize.y);
}

bool Maze::contains(const sf::Vector2i & pos)
{
    return (pos.x >= 0 && pos.x < size.x &&
            pos.y >= 0 && pos.y < size.y);
}

void Maze::generate(int x, int y)
{
    size.x = x;
    size.y = y;

    sf::Vector2i textureSize(x*squareSize.x,y*squareSize.y);

    if (walls)
        delete [] walls;

    walls = new node[x*y];
    for (int i = 0; i < x*y; i++)
    {
        walls[i].up = 0;
        walls[i].down = 0;
        walls[i].left = 0;
        walls[i].right = 0;
    }

    //start by adding the outer walls
    for (int i = 0; i < size.x; i++)
    {

        addWall(i,0,up);
        std::cout << "+";
    }
    for (int j = 0; j < size.y; j++)
    {
        addWall(0,j,left);
        std::cout << "-";
    }

    //then add inner walls
    //(make sure the degree of every square is at least 2)
    //(make sure there are no length 4 cycles: 4 squares are divided)
    //(make sure the graph remains connected)
    for (int i = 0; i < size.x; i++)
    {
        for (int j = 0; j < size.y; j++)
        {
            //if the coordinate is in a 4-cycle, add a wall
            if (!(isWall(i,j,up) || isWall(i,j,left) ||
                  isWall(i-1,j,up) || isWall(i,j-1,left)))
            {
                std::cout << ".";
                //check which walls can be added
                if (degree(i-1,j) == 2)
                    addWall(i,j,up);
                else if (degree(i,j-1) == 2)
                    addWall(i,j,left);
                //if both can be added, choose one at random
                else
                    addWall(i,j,rand() % 2);
            }

            //if no more walls can be added, skip it
            if (degree(i,j) == 2)
                continue;

            //count the number of  walls that can be placed
            int W = 0;
            for (int d = 0; d < 4; d++)
                if (isValid(i,j,d))
                    W++;

            //repeat degree-2 times, or until no more can be added
            for (int k = degree(i,j)-2; (k > 0) && (W > 0); k--)
            {
                int r = rand() % (3*W);
                //do not add a wall with probability 2/3
                if (r >= W)
                    continue;

                r = r % W;
                //find the corresponding wall to add
                int w = 0;
                for (int d = 0; d < 4; d++)
                    if (isValid(i,j,d) && w++ == r)
                    {
                        std::cout << ",";
                        addWall(i,j,d);
                        //when a wall is added, update #valid
                        W--;
                    }
            }
        }
    }
    std::cout << "\n";

    //create maze texture from walls
    sf::Sprite peg(bgTexture);
    peg.setTextureRect(sf::IntRect(0,0,textureSize.x,textureSize.y));
    sf::RenderTexture render;

    //start by drawing the background of the maze
    render.create(textureSize.x,textureSize.y);

    render.clear();
    render.draw(peg);

    //next draw the walls onto the texture
    peg.setTexture(wallTexture);

    for (int j = 0; j < size.y; j++)
    {
        for (int i = 0; i < size.x; i++)
        {
            int index = i*size.y + j;
            //find the proper wall pattern
            //squares are organized by which walls are present
            peg.setTextureRect(sf::IntRect((2*walls[index].left + walls[index].up) * squareSize.x,
                                           (2*walls[index].right + walls[index].down) * squareSize.y,
                                           squareSize.x, squareSize.y));
            //move the sprite to the current square and draw
            peg.setPosition(i*squareSize.x, j*squareSize.y);
            render.draw(peg);
            std::cout << degree(i,j) << ",(" <<
                         2*walls[index].left + walls[index].up << "," <<
                         2*walls[index].right + walls[index].down << ")  ";
        }
        std::cout << '\n';
    }
    //finally, save the finished texture

    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * DO NOT FORGET THIS LINE, OR THE
     * TEXTURE WILL BE DISPLAYED UPSIDE DOWN
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    render.display();


    mazeTexture = render.getTexture();
    sprite.setTexture(mazeTexture);
}

bool Maze::isValid(int x, int y, int direction)
{
    if (isWall(x,y,direction))
        return false;

    //find the neighbor in the given direction
    sf::Vector2i start(x,y), neighbor;
    switch(direction)
    {
    case up:
        neighbor = sf::Vector2i(x,y-1);
        break;
    case down:
        neighbor = sf::Vector2i(x,y+1);
        break;
    case left:
        neighbor = sf::Vector2i(x-1,y);
        break;
    case right:
        neighbor = sf::Vector2i(x+1,y);
        break;
    }

    //adding a wall would create a dead end
    if (degree(neighbor) == 2)
        return false;

    //add the wall, then check that the graph is still connected
    addWall(x,y,direction);
    bool * visited = new bool[size.x*size.y];
    for (int i = 0; i < size.x*size.y; i++)
        visited[i] = false;
    sf::Vector2i * current = new sf::Vector2i(neighbor);
    Heap<sf::Vector2i> heap;
    heap.addItem(current,L1(current,&start));
    //while the heap is nonempty
    while (heap.size())
    {
        current = heap.pop();
        //if the start node has been reached
        if (current->x != start.x ||
            current->y != start.y)
        {
            //cleanup and return true (graph is connected)
            delete current;
            while (heap.size())
                delete heap.pop();
            delete [] visited;
            removeWall(x,y,direction);
            return true;
        }
        //mark the node as visited
        visited[current->x*size.y+current->y] = true;

        //add all neighbors to the heap
        sf::Vector2i * next;
        if (!isWall(current->x,current->y,up) &&
            !visited[current->x*size.y+current->y-1])
        {
            next = new sf::Vector2i(current->x,current->y-1);
            heap.addItem(next,L1(next,&start));
        }

        if (!isWall(current->x,current->y,down) &&
            !visited[current->x*size.y+current->y+1])
        {
            next = new sf::Vector2i(current->x,current->y+1);
            heap.addItem(next,L1(next,&start));
        }

        if (!isWall(current->x,current->y,left) &&
            !visited[(current->x-1)*size.y+current->y])
        {
            next = new sf::Vector2i(current->x-1,current->y);
            heap.addItem(next,L1(next,&start));
        }

        if (!isWall(current->x,current->y,right) &&
            !visited[(current->x+1)*size.y+current->y])
        {
            next = new sf::Vector2i(current->x+1,current->y);
            heap.addItem(next,L1(next,&start));
        }

        delete current;
    }

    //if the graph is not connected
    delete [] visited;
    removeWall(x,y,direction);
    return false;
}

void Maze::draw(sf::RenderWindow & window)
{
    window.draw(sprite);
}

void Maze::addWall(int x, int y, int direction)
{
    if (x < 0 || x >= size.x ||
        y < 0 || y >= size.y)
        return;

    int index = x*size.y + y;

    switch(direction)
    {
    case up:
        walls[index].up = 1;
        //update the adjacent wall (with wraparound)
        if (--y < 0)
            y += size.y;
        walls[x*size.y + y].down = 1;
        break;

    case down:
        walls[index].down = 1;
        if (++y >= size.y)
            y = 0;
        walls[x*size.y+y].up = 1;
        break;

    case left:
        walls[index].left = 1;
        if (--x < 0)
            x += size.x;
        walls[x*size.y+y].right = 1;
        break;

    case right:
        walls[index].right = 1;
        if (++x >= size.x)
            x = 0;
        walls[x*size.y+y].left = 1;
        break;
    }
}

void Maze::removeWall(int x, int y, int direction)
{
    if (x < 0 || x >= size.x ||
        y < 0 || y >= size.y)
        return;

    int index = x*size.y + y;

    switch(direction)
    {
    case up:
        walls[index].up = 0;
        //update the adjacent wall (with wraparound)
        if (--y < 0)
            y += size.y;
        walls[x*size.y + y].down = 0;
        break;

    case down:
        walls[index].down = 0;
        if (++y >= size.y)
            y = 0;
        walls[x*size.y+y].up = 0;
        break;

    case left:
        walls[index].left = 0;
        if (--x < 0)
            x += size.x;
        walls[x*size.y+y].right = 0;
        break;

    case right:
        walls[index].right = 0;
        if (++x >= size.x)
            x = 0;
        walls[x*size.y+y].left = 0;
        break;
    }
}

float Maze::L1(sf::Vector2i * p1, sf::Vector2i * p2)
{
    return float(abs(p1->x - p2->x) +
                 abs(p1->y - p2->y));
}
