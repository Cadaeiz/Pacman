#ifndef VECTOR_H_
#define VECTOR_H_

template <class Item>
class Vector
{
private:
    Item * items;
    unsigned int num_elts, capacity;

public:
    Vector(unsigned int capacity);
    Vector(Vector<Item> & vect);
    ~Vector() { delete [] items; }

    Vector<Item> & operator=(const Vector<Item> & vect);
    Item & operator[](unsigned int i) { return items[i]; }
    const Item & operator[](unsigned int i) const { return items[i]; }
    void push_back(const Item & item);
    void pop_back();
    void swap(unsigned int i, unsigned int j);
    unsigned int size() { return num_elts; }
};

template <class Item>
Vector<Item>::Vector(unsigned int capacity = 10) : capacity(capacity), num_elts(0)
{
    items = new Item[capacity];
}

template <class Item>
Vector<Item>::Vector(Vector<Item> & vect) : capacity(vect.capacity), num_elts(vect.num_elts)
{
    items = new Item[capacity];
    for (int i = 0; i < capacity; i++)
        items[i] = vect.items[i];
}

template <class Item>
Vector<Item> & Vector<Item>::operator=(const Vector<Item> & vect)
{
    delete [] items;
    capacity = vect.capacity;
    num_elts = vect.num_elts;

    items = new Item[capacity];
    for (int i = 0; i < capacity; i++)
        items[i] = vect.items[i];
}

template <class Item>
void Vector<Item>::push_back(const Item & item)
{
    /* expand capacity if necessary */
    if (num_elts > capacity - 1)
    {
        Item * newItems = new Item[capacity * 2];
        for (int i = 0; i < num_elts; i++)
            newItems[i] = items[i];

        delete [] items;
        items = newItems;
        capacity *= 2;
    }
    items[num_elts++] = item;
}

template <class Item>
void Vector<Item>::pop_back()
{
    num_elts--;
    /* decrease capacity if necessary */
    if (num_elts < capacity / 4 && num_elts > 10)
    {
        Item * newItems = new Item[capacity / 2];
        for (int i = 0; i < num_elts; i++)
            newItems[i] = items[i];

        delete [] items;
        items = newItems;
        capacity /= 2;
    }
}

template <class Item>
void Vector<Item>::swap(unsigned int i, unsigned int j)
{
    Item temp = items[i];
    items[i] = items[j];
    items[j] = temp;
}

#endif // VECTOR_H_
