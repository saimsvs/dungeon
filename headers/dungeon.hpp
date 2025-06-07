/*#pragma once
#include "room.hpp"
#include "stack.hpp"
#include "player.hpp"
#include "queue.hpp"
#include "achievement.hpp"

#include <memory>
#include <vector>
#include "../headers/game_event.hpp"
#include <SFML/Graphics.hpp>

class dungeon
{
private:
    room *head;
    room *current;
    Stack stack;
    queue enemyQueue;

public:

    dungeon();
    ~dungeon();

    void addnew(room *Room, enemy *Enemy);
    bool moveroom(player &Player);
    room *getcurrentroom();
    void backtrack();
    void checkcurrentroom(player &Player);
    void battle(player &Player);
    void goback();
    void displaycurrentroom();
    bool haswon();
    void exitdungeon();
    void checkenemy(player &Player);


};*/

// headers/dungeon.hpp

// headers/dungeon.hpp

// headers/dungeon.hpp

// headers/dungeon.hpp

// headers/dungeon.hpp

// headers/dungeon.hpp

// headers/dungeon.hpp

#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <SFML/Graphics.hpp>
#include "room.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "treasure.hpp"
#include "trap.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include "achievement.hpp"

class dungeon
{
public:
    enum class DungeonState
    {
        Exploring,
        InCombat,
        InTrap,
        GameOver,
        Escaped
    };
    DungeonState state = DungeonState::Exploring;

private:
    sf::Texture *roomTexturePtr = nullptr;
    room *head;
    room *current;
    Stack stack;      // back‐stack of rooms
    queue enemyQueue; // queue of enemies

public:
    explicit dungeon(const sf::Texture &roomTex);
    ~dungeon();

    void addnew(room *Room, enemy *Enemy);
    bool moveroom(player &Player);
    void checkenemy(player &Player);
    void battle(player &Player);
    void checkcurrentroom(player &Player);
    void goback();
    void displaycurrentroom();

    // SFML 3 style: use event.kind and event.asKeyEvent().code
    void handleInput(sf::Event &event, player &P);

    void drawCurrentRoom(sf::RenderWindow &window);
    room *getCurrentRoom() const;
};

#endif // DUNGEON_HPP
