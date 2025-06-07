#pragma once
#include <string>
class character
{
protected:
    int health;
    std::string name;
    int maxhealth;
    int attackpower;
    int defense;

public:
    character(const std::string &name, int health, int maxhealth, int attackpower, int defense);
    virtual ~character();
    virtual void attack(character &target) = 0;
    int getmaxhealth();
    void setmaxhealth(int h);
    void takedamage(int damage);

    void isalive();
    std::string getname();
    int gethealth();
    int getdefense();
    void sethealth(int x);
    int getattack();
};