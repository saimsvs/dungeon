#pragma once
#include <iostream>
#include "treasure.hpp"
#include "enemy.hpp"
#include "trap.hpp"
#include "gamevalue.hpp"
#include <memory>
#include <SFML/Graphics.hpp>

class room
{
private:
    std::string description;
    enemy *Enemy;
    treasure *Treasure;
    room *next;
    // #include <memory>
    // Trap *trap;
    std::unique_ptr<Trap> trap;
    GameValue<float, 1> humidity{50.0f};
    // SFML members:
    std::unique_ptr<sf::Sprite> backgroundSprite;
    sf::Texture *backgroundTexturePtr = nullptr;
    sf::Vector2f position;

public:
    float checkHumidity();
    float getHumidity() const;
    room(std::string desc, enemy *e = nullptr, treasure *t = nullptr);
    std::string getdescription();
    enemy *getenemy();
    void clearEnemy();
    room *getnext();
    void setnext(room *r);
    treasure *gettreasure();
    void setenemy(enemy *Enemy);
    bool hasenemy();
    ~room();

    void settreasure(treasure *t);

    //
    // void settrap(Trap *t);
    void settrap(std::unique_ptr<Trap> t);
    Trap *gettrap() const;
    bool hastrap() const;

    ///
    // ───── SFML‐specific methods ─────
    void setBackgroundTexture(const sf::Texture &tex)
    {
        backgroundSprite = std::make_unique<sf::Sprite>(tex);
        backgroundTexturePtr = const_cast<sf::Texture *>(&tex);
        backgroundSprite->setScale({800.f / float(tex.getSize().x),
                                    600.f / float(tex.getSize().y)});
    }

    void setPosition(const sf::Vector2f &pos)
    {
        position = pos;
        if (backgroundSprite)
        {
            backgroundSprite->setPosition(position);
        }
    }

    const sf::Vector2f &getPosition() const
    {
        return position;
    }

    void draw(sf::RenderWindow &window)
    {
        if (backgroundSprite)
        {
            window.draw(*backgroundSprite);
        }
    }
};
