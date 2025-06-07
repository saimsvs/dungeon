#include "../headers/treasure.hpp"
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

treasure::treasure(std::string name, int value) : name(name), amount(value) {}

std::string treasure::getname()
{
    return name;
}

treasure::~treasure()
{
    std::cout << "treasure destruct\n";
}
int treasure::getvalue()
{
    return amount;
}

////
