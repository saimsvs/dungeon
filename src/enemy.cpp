#include "../headers/enemy.hpp"
#include <iostream>
#include "../headers/character.hpp"
#include <SFML/Graphics.hpp>

enemy::enemy(const std::string &name, int health, int maxhealth, int attackpower, int defense, int agg) : character(name, health, maxhealth, attackpower, defense), aggression(agg) {}

void enemy::attack(character &target)
{
    int damage = attackpower + aggression - target.getdefense();
    if (damage > 0)
    {
        target.takedamage(damage);
        target.sethealth(target.gethealth() - damage);
    }
    else
    {
        std::cout << "attack by " << name << " was too weak to harm " << target.getname() << " \n";
    }
}
int enemy::getattackpower()
{
    return attackpower;
}
enemy::~enemy()
{
    std::cout << "\n";
}
int enemy::getaggression()
{
    return aggression;
}

//
//
