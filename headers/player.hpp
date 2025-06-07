#pragma once
#include "treasure.hpp"
#include "character.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
class player : public character
{
private:
    bool alive = true;

    treasure **inventory;
    int inventorysize;
    int inventorycapacity;

    //
    std::unique_ptr<sf::Sprite> sprite; // no‐arg constructor removed in SFML 3
    sf::Texture *texturePtr = nullptr;
    float HUDbarWidth = 200.f;

public:
    void expandinventory();
    player(std::string name);
    void attack(character &target) override;
    void addtreasure(treasure *Treasure);
    void showinventory();
    ~player();
    void sortinventory();

    void setalive(bool status);
    bool isalive() const;
    friend std::ostream &operator<<(std::ostream &os, const player &p);

    /////
    // ───── SFML‐specific methods ─────
    void setTexture(const sf::Texture &tex)
    {
        // Construct the sprite with the texture
        sprite = std::make_unique<sf::Sprite>(tex);
        texturePtr = const_cast<sf::Texture *>(&tex);
        sprite->setScale({0.5f, 0.5f}); // adjust scale as needed
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

    void drawHUD(sf::RenderWindow &window)
    {
        if (!sprite)
            return;
        float healthPct = static_cast<float>(gethealth()) / getmaxhealth();
        sf::RectangleShape backBar({HUDbarWidth, 10.f});
        backBar.setFillColor({100, 100, 100});
        backBar.setPosition(sprite->getPosition() - sf::Vector2f(HUDbarWidth / 2, 20.f));

        sf::RectangleShape healthBar({HUDbarWidth * healthPct, 10.f});
        healthBar.setFillColor(sf::Color::Red);
        healthBar.setPosition(backBar.getPosition());

        window.draw(backBar);
        window.draw(healthBar);
    }
};