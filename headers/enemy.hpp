
#pragma once
#include <SFML/Graphics.hpp>
#include "character.hpp"
#include <iostream>

class enemy : public character
{
    int aggression;
    std::unique_ptr<sf::Sprite> sprite;
    sf::Texture *texturePtr = nullptr;

public:
    enemy(const std::string &name, int health, int maxhealth, int attackpower, int defense, int aggression);
    ~enemy();
    void attack(character &target) override;
    int getaggression();
    int getattackpower();

    ///
    void setTexture(const sf::Texture &tex)
    {
        sprite = std::make_unique<sf::Sprite>(tex);
        texturePtr = const_cast<sf::Texture *>(&tex);
        sprite->setScale({0.5f, 0.5f});
    }

    void setPosition(const sf::Vector2f &pos)
    {
        if (sprite)
        {
            sprite->setPosition(pos);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        if (sprite)
        {
            window.draw(*sprite);
        }
    }
};