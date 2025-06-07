#include "../headers/player.hpp"
#include <vector>
#include "../headers/character.hpp"
#include <iostream>
#include "../headers/treasure.hpp"
#include "../headers/achievement.hpp"
#include <SFML/Graphics.hpp>
void player::setalive(bool status) { alive = status; }
bool player::isalive() const { return alive; }

player::player(const std::string name) : character(name, 1915, 1915, 20, 5)
{
    inventorycapacity = 2;
    inventorysize = 0;
    inventory = new treasure *[inventorycapacity];

    for (int i = 0; i < inventorycapacity; i++)
    {
        inventory[i] = nullptr;
    }
}

void player::attack(character &target)
{

    target.takedamage(attackpower);
}

void player::expandinventory()
{
    int newcapacity = inventorycapacity * 2;
    treasure **newinventory = new treasure *[newcapacity];
    for (int i = 0; i < inventorycapacity; i++)
    {
        newinventory[i] = inventory[i];
    }
    delete[] inventory;
    inventory = newinventory;
    inventorycapacity = newcapacity;
}

void player::addtreasure(treasure *Treasure)
{
    if (inventorysize >= inventorycapacity)
    {
        expandinventory();
    }
    inventory[inventorysize++] = Treasure;
    std::cout << "You have collected: " << Treasure->getname() << "\n";
    showinventory();
    static AchievementTracker achievements;
    achievements.unlockTiered("Treasure Collector", 5);
}

void player::showinventory()
{
    sortinventory();
    std::cout << name << "'s inventory:";
    for (int i = 0; i < inventorysize; i++)
    {
        std::cout << inventory[i]->getname() << "-" << inventory[i]->getvalue() << "\n";
    }
}
player::~player()
{
    for (int i = 0; i < inventorysize; ++i)
    {
        delete inventory[i]; // Delete each treasure
        std::cout << "Treasure deleted from Player Inventory\n";
    }
    delete[] inventory;
}

std::ostream &operator<<(std::ostream &os, const player &p)
{
    os << p.name << "'s Stats:\n";
    os << "Health: " << p.health << "\n";
    os << "Inventory:\n";

    for (int i = 0; i < p.inventorysize; ++i)
    {
        if (p.inventory[i] != nullptr)
        {
            os << "- " << p.inventory[i]->getname()
               << " (Value: " << p.inventory[i]->getvalue() << ")\n";
        }
    }
    return os;
}

void player::sortinventory()
{
    for (int i = 0; i < inventorysize - 1; ++i)
    {
        int minIndex = i;
        for (int j = i + 1; j < inventorysize; ++j)
        {
            if (inventory[j]->getvalue() < inventory[minIndex]->getvalue())
            {
                minIndex = j;
            }
        }
        // Swap the treasures based on the minimum value
        if (minIndex != i)
        {
            treasure *temp = inventory[i];
            inventory[i] = inventory[minIndex];
            inventory[minIndex] = temp;
        }
    }
}
