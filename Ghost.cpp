#include "Ghost.hpp"
#include "Heap.hpp"
#include <cmath>

Vector<sf::Vector2i> Ghost::AStar(const sf::Vector2i & start, const sf::Vector2i & goal)
{
    //scalar for heuristic component of cost fn
    float D = 1.05;
    // heap for sorting & processing
    Heap<Node> heap;
    sf::Vector2i size = maze.getSize();
    // arrays for quick membership testing
    Node **open = new Node *[size.x*size.y],
       **closed = new Node *[size.x*size.y];
    for (int i = 0; i < size.x*size.y; i++)
        open[i] = closed[i] = 0;

    //add starting position to open set
    int index = start.y*size.x+start.x,
        lastIndex = goal.y*size.x+goal.x;
    Node * currentNode = new Node(start.x,start.y,0,-1);

    //until the goal has been reached
    while(currentNode->x != goal.x || currentNode->y != goal.y)
    {
        index = currentNode->y*size.x + currentNode->x;
        //move the node to closed
        open[index] = 0;
        closed[index] = currentNode;

        //for each of the current index's neighbors
        for (int i = 0; i < 4; i++)
        {
            int x = currentNode->x;
            int y = currentNode->y;
            //if separated by a wall, skip
            if (maze.isWall(x,y,i))
                continue;
            switch(i)
            {
            case Maze::up:
                y--;
                break;
            case Maze::down:
                y++;
                break;
            case Maze::left:
                x--;
                break;
            case Maze::right:
                x++;
                break;
            }
            //if adjacent node does not exist
            //(i.e. is not a neighbor of current) do not process it
            if (x < 0 || x >= size.x || y < 0 || y >= size.y)
                continue;

            int nindex = y * size.x + x;
            //compute the distance from start to neighbor
            int g = currentNode->g + 1;
            //if neighbor is in OPEN and the new path is better
            if (open[nindex] && g < open[nindex]->g)
            {
                //remove the neighbor from OPEN
                heap.removeItem(open[nindex]);
                delete open[nindex];
                open[nindex] = 0;
            }

            //if neighbor was already processed, but new path is better
            if (closed[nindex] && g < closed[nindex]->g)
            {
                //remove the neighbor from CLOSED
                delete closed[nindex];
                closed[nindex] = 0;
            }

            //if the neighbor is not in either set
            if (!closed[nindex] && !open[nindex])
            {
                //add it to OPEN
                open[nindex] = new Node(x,y,g,index);
                heap.addItem(open[nindex], g + D*L1distance(x,y,goal.x,goal.y));
            }
        }
        if (heap.size() == 0)
        {
            Vector<sf::Vector2i> path;

            //free all memory used by algorithm
            for (int i = 0; i < size.x*size.y; i++)
            {
                if (open[i])
                    delete open[i];
                if (closed[i])
                    delete closed[i];
            }

            delete [] open;
            delete [] closed;
            return path;
        }
        else
            currentNode = heap.pop();
    }

    //reconstruct path found and form a list from the path
    Vector<Node *> reversePath;
    while (currentNode -> parent != -1)
    {
        reversePath.push_back(currentNode);
        if (open[currentNode -> parent])
            currentNode = open[currentNode -> parent];
        else
            currentNode = closed[currentNode -> parent];
    }
    reversePath.push_back(currentNode);
    Vector<sf::Vector2i> path;
    for (int i = reversePath.size() - 1; i >= 0; i--)
        path.push_back(sf::Vector2i(reversePath[i]->x,reversePath[i]->y));

    //free all memory used by algorithm
    for (int i = 0; i < size.x*size.y; i++)
    {
        if (open[i])
            delete open[i];
        if (closed[i])
            delete closed[i];
    }

    delete [] open;
    delete [] closed;

    return path;
}

int Ghost::L1distance(const sf::Vector2i & p1, const sf::Vector2i & p2)
{
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

int Ghost::L1distance(int x1, int y1, int x2, int y2)
{
    return abs(x1-x2) + abs(y1-y2);
}

void Ghost::handleEvent(sf::Event e)
{
    if (e.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i goal = maze.getGridPosition(sf::Vector2f(e.mouseButton.x,e.mouseButton.y));
        if (maze.contains(goal))
            path = AStar(sf::Vector2i(0,0),goal);
    }
}

void Ghost::draw(sf::RenderWindow & window)
{
//    sf::CircleShape shape(10.f);
//    shape.setOrigin(sf::Vector2f(10,10));
//    shape.setFillColor(sf::Color::Green);
    sf::Vertex * line = new sf::Vertex[path.size()];
    for (int i = 0; i < path.size(); i++)
    {
//        shape.setPosition(maze.getTruePosition(path[i]));
//        window.draw(shape);
        line[i] = sf::Vertex(maze.getTruePosition(path[i]));
    }
    window.draw(line, path.size(), sf::LinesStrip);
    delete [] line;
}
