/*
// src/dungeon.cpp

#include "../headers/dungeon.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// ───────────────────────────────────────────────────────────────────────────────
//  Constructor: stores pointer to the room‐background texture
// ───────────────────────────────────────────────────────────────────────────────
dungeon::dungeon(const sf::Texture &roomTex)
    : head(nullptr), current(nullptr)
{
    roomTexturePtr = const_cast<sf::Texture *>(&roomTex);
}

// ───────────────────────────────────────────────────────────────────────────────
//  Destructor: deletes every room in the linked list. Each room’s destructor
//  will free its own enemy/treasure/trap.
// ───────────────────────────────────────────────────────────────────────────────
dungeon::~dungeon()
{
    room *ptr = head;
    while (ptr)
    {
        room *nextRoom = ptr->getnext();
        delete ptr;
        ptr = nextRoom;
    }
    head = current = nullptr;
}

// ───────────────────────────────────────────────────────────────────────────────
//  addnew: append a new room to the list, assign its background, enqueue enemy
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::addnew(room *Room, enemy *Enemy)
{
    if (!head)
    {
        head = Room;
        current = head;
    }
    else
    {
        room *temp = head;
        while (temp->getnext() != nullptr)
            temp = temp->getnext();
        temp->setnext(Room);
    }

    // Give the new room its SFML background sprite & position:
    if (roomTexturePtr)
    {
        Room->setBackgroundTexture(*roomTexturePtr);
        Room->setPosition({0.f, 0.f});
    }

    // If there’s an enemy, link it in and enqueue it:
    if (Enemy)
    {
        Room->setenemy(Enemy);
        enemyQueue.enqueue(Enemy);
    }
    else
    {
        Room->setenemy(nullptr);
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  moveroom: move from the current room to the next.
//    - If no “next,” the player has escaped (return true).
//    - Otherwise, push current→stack, advance→current, then trigger check.
// ───────────────────────────────────────────────────────────────────────────────
bool dungeon::moveroom(player &P)
{
    if (!current)
        return false;

    if (current->getnext() == nullptr)
    {
        state = DungeonState::Escaped;
        return true;
    }
    else
    {
        stack.push(current);
        current = current->getnext();
        checkcurrentroom(P);
        return false;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  checkenemy: if current room has a queued enemy still alive, switch to InCombat.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::checkenemy(player &P)
{
    if (!enemyQueue.isempty() && current && current->hasenemy())
    {
        state = DungeonState::InCombat;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  battle: one round of melee. Player attacks first. If enemy survives,
//  it attacks back. Adjust state if someone died.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::battle(player &P)
{
    if (!current)
        return;
    enemy *E = current->getenemy();
    if (!E)
        return;

    // Player’s attack:
    P.attack(*E);

    // If enemy still alive, it strikes back:
    if (E->gethealth() > 0)
    {
        E->attack(P);
    }

    // If player died, GameOver:
    if (P.gethealth() <= 0)
    {
        state = DungeonState::GameOver;
        return;
    }

    // If enemy died, clear it and go back to Exploring:
    if (E->gethealth() <= 0)
    {
        current->setenemy(nullptr);
        state = DungeonState::Exploring;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  checkcurrentroom: handle trap damage, pick up treasure, then check for enemy.
//  (Restored full trap‐and‐treasure printing logic.)
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::checkcurrentroom(player &P)
{
    if (!current)
        return;

    // 1) If there’s a trap and it’s still armed, trigger it:
    if (current->hastrap())
    {
        Trap *trap = current->gettrap();
        if (trap && trap->isArmed())
        {
            trap->trigger();
            int dmg = trap->getDamage();
            P.sethealth(P.gethealth() - dmg);
            std::cout << "☠️ Trap dealt " << dmg << " damage! Current HP: " << P.gethealth() << "\n";

            if (P.gethealth() <= 0)
            {
                std::cout << "💀 The trap killed you!\n";
                P.setalive(false);
                return;
            }

            trap->disarm();
        }
    }

    // 2) If there’s a treasure, give it to the player:
    if (current->gettreasure())
    {
        treasure *T = current->gettreasure();
        if (T)
        {
            P.addtreasure(T);
            current->settreasure(nullptr);

            std::cout << "\n💎 You found a treasure!\n";
            P.showinventory();
        }
    }

    // 3) Now check for an enemy to go InCombat:
    checkenemy(P);
}

// ───────────────────────────────────────────────────────────────────────────────
//  goback: pop the last visited room from the stack and make it current again.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::goback()
{
    if (stack.isempty())
        return;
    room *prevRoom = stack.pop();
    current = prevRoom;
    state = DungeonState::Exploring;
}

// ───────────────────────────────────────────────────────────────────────────────
//  displaycurrentroom: (legacy console) prints room description.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::displaycurrentroom()
{
    if (current)
    {
        std::cout << current->getdescription() << "\n";
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  SFML 3 handleInput: uses event.is<>() and event.getIf<>() for key codes.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::handleInput(sf::Event &event, player &P)
{
    // If InCombat: Space to attack, Esc to flee
    if (state == DungeonState::InCombat && event.is<sf::Event::KeyPressed>())
    {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->code == sf::Keyboard::Key::Space)
        {
            battle(P);
        }
        else if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape)
        {
            goback();
            state = DungeonState::Exploring;
        }
    }
    // If Exploring: Right arrow to move forward, Left to go back
    else if (state == DungeonState::Exploring && event.is<sf::Event::KeyPressed>())
    {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->code == sf::Keyboard::Key::Right)
        {
            if (moveroom(P))
                state = DungeonState::Escaped;
        }
        else if (keyEvent && keyEvent->code == sf::Keyboard::Key::Left)
        {
            goback();
        }
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  drawCurrentRoom: tell the current room to draw its background sprite.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::drawCurrentRoom(sf::RenderWindow &window)
{
    if (current)
    {
        current->draw(window);
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  getCurrentRoom: returns a pointer to the current room.
// ───────────────────────────────────────────────────────────────────────────────
room *dungeon::getCurrentRoom() const
{
    return current;
}
*/

// src/dungeon.cpp

#include "../headers/dungeon.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// ───────────────────────────────────────────────────────────────────────────────
//  Constructor: stores pointer to the room‐background texture
// ───────────────────────────────────────────────────────────────────────────────
dungeon::dungeon(const sf::Texture &roomTex)
    : head(nullptr), current(nullptr)
{
    roomTexturePtr = const_cast<sf::Texture *>(&roomTex);
}

// ───────────────────────────────────────────────────────────────────────────────
//  Destructor: deletes every room in the linked list. Each room’s destructor
//  will free its own enemy/treasure/trap.
// ───────────────────────────────────────────────────────────────────────────────
dungeon::~dungeon()
{
    room *ptr = head;
    while (ptr)
    {
        room *nextRoom = ptr->getnext();
        delete ptr;
        ptr = nextRoom;
    }
    head = current = nullptr;
}

// ───────────────────────────────────────────────────────────────────────────────
//  addnew: append a new room to the list, assign its background, enqueue enemy
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::addnew(room *Room, enemy *Enemy)
{
    if (!head)
    {
        head = Room;
        current = head;
    }
    else
    {
        room *temp = head;
        while (temp->getnext() != nullptr)
            temp = temp->getnext();
        temp->setnext(Room);
    }

    // Give the new room its SFML background sprite & position:
    if (roomTexturePtr)
    {
        Room->setBackgroundTexture(*roomTexturePtr);
        Room->setPosition({0.f, 0.f});
    }

    // If there’s an enemy, link it in and enqueue it:
    if (Enemy)
    {
        Room->setenemy(Enemy);
        enemyQueue.enqueue(Enemy);
    }
    else
    {
        Room->setenemy(nullptr);
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  moveroom: move from the current room to the next.
//    - If no “next,” the player has escaped (return true).
//    - Otherwise, push current→stack, advance→current, then trigger check.
// ───────────────────────────────────────────────────────────────────────────────
bool dungeon::moveroom(player &P)
{
    if (!current)
        return false;

    if (current->getnext() == nullptr)
    {
        state = DungeonState::Escaped;
        return true;
    }
    else
    {
        stack.push(current);
        current = current->getnext();
        checkcurrentroom(P);
        return false;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  checkenemy: if current room has a queued enemy still alive, switch to InCombat.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::checkenemy(player &P)
{
    if (!enemyQueue.isempty() && current && current->hasenemy())
    {
        state = DungeonState::InCombat;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  battle: one round of melee. Player attacks first. If enemy survives,
//  it attacks back. Adjust state if someone died.
// ───────────────────────────────────────────────────────────────────────────────
// ───────────────────────────────────────────────────────────────────────────────
//  battle: one round of melee. Player attacks first. If enemy survives,
//  it attacks back. Adjust state if someone died.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::battle(player &P)
{
    if (!current)
        return;

    enemy *E = current->getenemy();
    if (!E)
        return;

    // 1) Player attacks first
    P.attack(*E);

    // 2) If enemy still alive, it strikes back
    if (E->gethealth() > 0)
    {
        E->attack(P);
    }

    // 3) If player died in that counter‐attack, mark them dead
    if (P.gethealth() <= 0)
    {
        state = DungeonState::GameOver;
        P.setalive(false);          // ← mark player as dead
        std::cout << "You lost!\n"; // ← print loss message
        return;
    }

    // 4) If enemy died, remove it and go back to Exploring
    if (E->gethealth() <= 0)
    {
        current->setenemy(nullptr);
        state = DungeonState::Exploring;
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  checkcurrentroom: handle trap damage, pick up treasure, then check for enemy.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::checkcurrentroom(player &P)
{
    if (!current)
        return;

    // 1) If there’s a trap and it’s still armed, trigger it:
    if (current->hastrap())
    {
        Trap *trap = current->gettrap();
        if (trap && trap->isArmed())
        {
            // 1a) Trigger the trap (prints its activation via trap->trigger())
            trap->trigger();

            // 1b) Deal damage and print to console (so TeeBuf captures it)
            int dmg = trap->getDamage();
            P.sethealth(P.gethealth() - dmg);
            std::cout << "☠️ Trap dealt " << dmg << " damage! Current HP: " << P.gethealth() << "\n";

            // 1c) If the trap killed the player, print death message and stop
            if (P.gethealth() <= 0)
            {
                std::cout << "💀 The trap killed you!\n";
                P.setalive(false);
                return;
            }

            // 1d) Disarm the trap so it won't trigger again
            trap->disarm();
        }
    }

    // 2) If there’s a treasure, give it to the player:
    if (current->gettreasure())
    {
        treasure *T = current->gettreasure();
        if (T)
        {
            P.addtreasure(T);
            current->settreasure(nullptr);
        }
    }

    // 3) Now check for an enemy to go InCombat:
    checkenemy(P);
}

// ───────────────────────────────────────────────────────────────────────────────
//  goback: pop the last visited room from the stack and make it current again.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::goback()
{
    if (stack.isempty())
        return;
    room *prevRoom = stack.pop();
    current = prevRoom;
    state = DungeonState::Exploring;
}

// ───────────────────────────────────────────────────────────────────────────────
//  displaycurrentroom: (legacy console) prints room description.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::displaycurrentroom()
{
    if (current)
    {
        std::cout << current->getdescription() << "\n";
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  SFML 3 handleInput: uses event.is<>() and event.getIf<>() for key codes.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::handleInput(sf::Event &event, player &P)
{
    // If InCombat: Space to attack, Esc to flee
    if (state == DungeonState::InCombat && event.is<sf::Event::KeyPressed>())
    {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->code == sf::Keyboard::Key::Space)
        {
            battle(P);
        }
        else if (keyEvent && keyEvent->code == sf::Keyboard::Key::Escape)
        {
            goback();
            state = DungeonState::Exploring;
        }
    }
    // If Exploring: Right arrow to move forward, Left to go back
    else if (state == DungeonState::Exploring && event.is<sf::Event::KeyPressed>())
    {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent && keyEvent->code == sf::Keyboard::Key::Right)
        {
            if (moveroom(P))
                state = DungeonState::Escaped;
        }
        else if (keyEvent && keyEvent->code == sf::Keyboard::Key::Left)
        {
            goback();
        }
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  drawCurrentRoom: tell the current room to draw its background sprite.
// ───────────────────────────────────────────────────────────────────────────────
void dungeon::drawCurrentRoom(sf::RenderWindow &window)
{
    if (current)
    {
        current->draw(window);
    }
}

// ───────────────────────────────────────────────────────────────────────────────
//  getCurrentRoom: returns a pointer to the current room.
// ───────────────────────────────────────────────────────────────────────────────
room *dungeon::getCurrentRoom() const
{
    return current;
}
