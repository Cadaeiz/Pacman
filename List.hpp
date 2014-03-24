#ifndef LIST_H_
#define LIST_H_

template <class Item>
class List
{
private:
	unsigned int num_elts;
	struct Node
	{
		Item * item;
		Node * next;
		Node * prev;
	};
	Node * first, * last;
	void removeNode(Node * n);
	void reset();
public:
	class Iterator
	{
	private:
		Node * nextNode, * prevNode;
		List<Item> * l;
	public:
		Iterator(List<Item> * l, Node * start = 0) : nextNode(start), prevNode(0), l(l) { }
		bool hasNext() { return (nextNode != 0); }
		Item * removePreviousItem();
		Item * next();
	};

	List() : first(0), last(0), num_elts(0) { }
    List(List<Item> & l);
    List & operator=(const List<Item> & l);
	~List();
	void addItem(Item * item);
	bool removeItem(Item * item);
	unsigned int getSize() { return num_elts; }
	Iterator getIterator();
};

template <class Item>
void List<Item>::List(List<Item> & l)
{
    /* delete current list */
    reset();

    /* add all nodes in l to this list */
    Node * n = l.first;
    while(n)
    {
        addItem(n -> item)
        n = n -> next;
    }
}

template <class Item>
typename List<Item> & operator=(const List<Item> & l)
{
    reset();

    Node * n = l.first;
    while(n)
    {
        addItem(n -> item)
        n = n -> next;
    }

    return *this;
}
void List<Item>::reset()
{
	/* delete all remaining nodes */
	Node * n = first;

	while (first != 0)
	{
		n = first;
		first = first -> next;
		delete n;
	}

	num_elts = 0;
}


template <class Item>
void List<Item>::addItem(Item * item)
{
	Node * n = new Node;
	n -> item = item;
	/* add node to the end of the list */
	n -> prev = last;
	n -> next = 0;
	if (last == 0)
		first = n;
	else
		last -> next = n;
	last = n;
	num_elts++;
}

template <class Item>
bool List<Item>::removeItem(Item * item)
{
	bool removed = false;
	Node * match = 0;

	for (Node * n = first; n != 0; n = n -> next, delete match)
	{
		match = 0;
		if (n -> item == item)
		{
			removeNode(n);
			/* mark node for deletion */
			match = n;
			removed = true;
		}
	}
	return removed;
}

template <class Item>
void List<Item>::removeNode(Node * n)
{
	if (n -> prev != 0)
		(n -> prev) -> next = n -> next;
	else
		first = n -> next;

	if (n -> next != 0)
		(n -> next) -> prev = n -> prev;
	else
		last = n -> prev;
	num_elts--;
}

template <class Item>
List<Item>::~List()
{
    reset();
}

template <class Item>
typename List<Item>::Iterator List<Item>::getIterator()
{
	return Iterator(this, first);
}

template <class Item>
Item * List<Item>::Iterator::next()
{
	if(hasNext())
	{
		prevNode = nextNode;
		nextNode = nextNode -> next;
		return prevNode -> item;
	}
	else
		return 0;
}

template <class Item>
Item * List<Item>::Iterator::removePreviousItem()
{
	Item * item = 0;
	if (prevNode != 0)
	{
		l -> removeNode(prevNode);
		item = prevNode -> item;
		delete prevNode;
		prevNode = 0;
	}
	return item;
}

#endif
