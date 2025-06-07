#include <iostream>
#include "../headers/stack.hpp"
#include "../headers/room.hpp"

Stack::Stack() : top(nullptr) {}

Stack::~Stack()
{
    while (!isempty())
    {
        Node *temp = top;
        top = top->next;
        delete temp; // Only delete Node
    }
}

void Stack::push(room *Room)
{
    Node *newnode = new Node(Room);
    newnode->next = top;
    top = newnode;
}

room *Stack::pop()
{
    if (isempty())
    {
        return nullptr;
    }
    Node *temp = top;
    room *R = top->Room;
    top = top->next;
    delete temp;
    return R;
}
room *Stack::gettop()
{
    if (isempty())
    {

        return nullptr;
    }
    else
    {
        return top->Room;
    }
}

bool Stack::isempty()
{
    return top == nullptr;
}