#include "../headers/queue.hpp"
#include "../headers/enemy.hpp"
#include <iostream>

queue::queue()
{
    front = nullptr;
    rear = nullptr;
}

queue::~queue()
{
    while (!isempty())
    {
        enemy *e = dequeue(); // Removes node and returns enemy*
        delete e;             // Delete enemy object
        std::cout << "Enemy deleted by Queue\n";
    }
}

void queue::enqueue(enemy *newEnemy)
{
    Node *newNode = new Node{newEnemy, nullptr};
    if (isempty())
    {
        front = rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }
}

enemy *queue::dequeue()
{
    if (isempty())
        return nullptr;

    Node *temp = front;
    enemy *e = front->data;
    front = front->next;
    delete temp; // Delete node
    return e;    // Caller deletes enemy*
}

bool queue::isempty() const
{
    return front == nullptr;
}

enemy *queue::peek() const
{
    return isempty() ? nullptr : front->data;
}
