#include "room.hpp"
#include <iostream>

class Node{
public:
room* Room;
Node* next;
Node(room* r) : Room(r), next(nullptr) {}

};
class Stack{

    private:
   Node* top;
   public:

   Stack(); 
    ~Stack(); 
    void push(room* Room);
    room* pop(); 
room* gettop();

    bool isempty(); 
};



