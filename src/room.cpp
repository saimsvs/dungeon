#include "../headers/room.hpp"
#include <iostream>
#include "../headers/treasure.hpp"
#include "../headers/enemy.hpp"
#include "../headers/gamevalue.hpp"
#include <cstdlib>
#include <SFML/Graphics.hpp>
room::room(std::string desc, enemy *e, treasure *t)
    : description(desc), Enemy(e), Treasure(t), next(nullptr), trap(nullptr) {}
float room::getHumidity() const { return humidity.get(); }
float room::checkHumidity()
{ // srand(time(0));

    float current = (humidity.get() - (rand() % 20));
    humidity.set(current - 7.0f); // Decrease by 7%
    return current;               // Return value BEFORE decrease
}
std::string room::getdescription()
{
    return description;
}

enemy *room::getenemy()
{
    return Enemy;
}

treasure *room::gettreasure()
{
    return Treasure;
}

room *room::getnext()
{
    return next;
}

void room::setnext(room *r)
{
    next = r;
}

void room::clearEnemy()
{
    Enemy = nullptr;
}

void room::setenemy(enemy *Enemy)
{
    this->Enemy = Enemy;
}

bool room::hasenemy()
{
    return this->Enemy != nullptr;
}

void room::settreasure(treasure *t)
{
    Treasure = t;
}
/*
room::~room()
{
    if (Treasure != nullptr)
    {
        delete Treasure;
        std::cout << "Unclaimed treasure destroyed by Room\n";
    }
    if (trap != nullptr)
    {
        delete trap;
        std::cout << "Trap deleted by Room\n";
    }
    std::cout << "Treasure deleted by player\n";
}*/
room::~room()
{
    // No need to delete trap manually; unique_ptr does it automatically.
    if (Treasure != nullptr)
    {
        delete Treasure;
        Treasure = nullptr;
    }
    //  if (Enemy != nullptr) {
    //    delete Enemy;
    //  Enemy = nullptr;
    //}
    // (And you still null‐out next if needed, etc.)
}

//
void room::settrap(std::unique_ptr<Trap> t)
{
    trap = std::move(t);
}
Trap *room::gettrap() const
{
    return trap.get(); // unique_ptr→raw
}
bool room::hastrap() const
{
    return (trap != nullptr) && trap->isArmed();
}
///////
