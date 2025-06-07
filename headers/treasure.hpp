#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class treasure
{
    std::string name;
    int amount;
    ///
    std::unique_ptr<sf::Sprite> sprite;
    sf::Texture *texturePtr = nullptr;
    sf::Vector2f position;

public:
    treasure();
    treasure(std::string name, int value);
    std::string getname();
    int getvalue();

    virtual ~treasure();
    ////
    // ───── SFML‐specific ─────
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
        if (sprite)
        {
            window.draw(*sprite);
        }
    }
};