#include "../headers/character.hpp"

#include <iostream>

character::character(const std::string &name, int health, int maxhealth, int attackpower, int defense) : name(name), health(health), maxhealth(maxhealth), attackpower(attackpower), defense(defense) {}

character::~character() { std::cout << name << " is destroyed\n"; }
void character::takedamage(int damage)
{
    int save = damage - defense;
    health = health - save;
    if (save < 0)
        save = 0;
    if (health < 0)
    {
        health = 0;
    }
    std::cout << name << " has taken damage and health was " << health << "\n";
}

void character::isalive()
{
    if (health > 0)
    {
        std::cout << name << " is alive";
    }
    else
    {
        std::cout << " DEAD!\n";
    }
}

std::string character ::getname() { return name; }

int character::gethealth() { return health; }
int character::getdefense() { return defense; }
void character::sethealth(int x)
{
    health = x;
}

int character::getattack()
{
    return attackpower;
}

int character::getmaxhealth()
{
    return maxhealth;
}
void character::setmaxhealth(int h)
{
    maxhealth = h;
}