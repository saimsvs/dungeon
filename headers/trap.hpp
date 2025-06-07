

#pragma once
#include "game_event.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Trap : public GameEvent
{
public:
    Trap(int damage = 10);

    void trigger() override;
    std::string getEventName() const override;
    bool isActive() const override;
    void setActive(bool active) override;

    int getDamage() const;
    void setDamage(int damage);

    bool isArmed() const;
    void disarm();
    int getdamage() const;
    ////
    // Call once from main
    void setTexture(const sf::Texture &tex)
    {
        sprite = std::make_unique<sf::Sprite>(tex);
        texturePtr = const_cast<sf::Texture *>(&tex);
        sprite->setScale({0.5f, 0.5f});
    }

    void setPosition(const sf::Vector2f &pos)
    {
        position = pos;
        if (sprite)
        {
            sprite->setPosition(position);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        if (armed_ && sprite)
        {
            window.draw(*sprite);
        }
    }

private:
    int damage_;
    bool armed_ = true;
    bool active_ = true;
    /////////
    std::unique_ptr<sf::Sprite> sprite;
    sf::Texture *texturePtr = nullptr;
    sf::Vector2f position;
};