// src/trap.cpp
#include "../headers/trap.hpp"
#include <iostream>

int Trap::getdamage() const { return damage_; }
Trap::Trap(int damage) : damage_(damage) {}

void Trap::trigger()
{
    if (active_ && armed_)
    {
        std::cout << "💥 Trap activated! (" << damage_ << " damage)\n";
    }
}

std::string Trap::getEventName() const
{
    return "Spiked Trap";
}

bool Trap::isActive() const
{
    return active_;
}

void Trap::setActive(bool active)
{
    active_ = active;
}

int Trap::getDamage() const
{
    return damage_;
}

void Trap::setDamage(int damage)
{
    damage_ = damage;
}

bool Trap::isArmed() const
{
    return armed_;
}

void Trap::disarm()
{
    armed_ = false;
} ////
