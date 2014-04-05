#ifndef HEAP_H_
#define HEAP_H_

#include "Vector.hpp"

template <class Item>
class Heap
{
private:
    struct Node
    {
        Node(Item * item, float cost) : item(item), cost(cost) {}
        Node(Node & node) : item(node.item), cost(node.cost) {}
        Node() : item(0), cost(0) { }
        Item * item;
        float cost;
    };
    bool minHeap;
    Vector<Node> nodes;
    int num_elts;

public:
    Heap(bool minHeap = true) : num_elts(0), minHeap(minHeap) { }
    void addItem(Item * item, float cost);
    void removeItem(Item * item);
    Item * pop();
    int size() { return num_elts; }
};

template <class Item>
void Heap<Item>::addItem(Item * item, float cost)
{
    int i = num_elts++;
    nodes.push_back(Node(item,cost));
    /* if parent is larger in a min heap or smaller in a max heap */
    while (i > 0 && (nodes[(i+1)/2-1].cost > nodes[i].cost == minHeap))
    {
        /* move item up the list and recurse */
        nodes.swap(i,(i+1)/2-1);
        i = (i+1)/2 - 1;
    }
}

template <class Item>
void Heap<Item>::removeItem(Item * item)
{
    int i = 0;
    while (i < num_elts && nodes[i].item != item)
        i++;
    if (i == num_elts)
        return;

    /* move last item to this position, then swap it to a valid location */
    nodes[i] = nodes[--num_elts];
    nodes.pop_back();

    int larger;
    /* if i has no children, then item is in valid position */
    while ((larger = 2*i+1) < num_elts)
    {
        /* find the higher priority of the 2 children (if both exist) */
        if (larger+1 < num_elts && (nodes[larger+1].cost < nodes[larger].cost == minHeap))
            larger++;

        /* if the current node has lower priority than its children, swap with the larger */
        if (nodes[i].cost > nodes[larger].cost == minHeap)
        {
            nodes.swap(i,larger);
            i = larger;
        }
        /* if the current node has higher priority, then it is in the right place */
        else
            break;
    }
}

template <class Item>
Item * Heap<Item>::pop()
{
    Item * item = nodes[0].item;
    removeItem(item);
    return item;
}

#endif // HEAP_H_
