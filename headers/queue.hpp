
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../headers/enemy.hpp"

class queue {
private:
    struct Node {
        enemy* data;
        Node* next;
    };
    Node* front;
    Node* rear;

public:
    queue();
    ~queue();
    
    void enqueue(enemy* newEnemy);
    enemy* dequeue();
    bool isempty() const;
    enemy* peek() const;  
};

#endif
