
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <climits>
#include <optional>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "../headers/dungeon.hpp"
#include "../headers/player.hpp"
#include "../headers/room.hpp"
#include "../headers/queue.hpp"
#include "../headers/treasure.hpp"
#include "../headers/trap.hpp"
#include "../headers/gamevalue.hpp"
#include "../headers/achievement.hpp"
#include <stdexcept>
#include <iostream>

struct TeeBuf : std::streambuf
{
    std::streambuf *orig;       // the original std::cout buffer
    std::ostringstream capture; // we accumulate everything here

    TeeBuf() : orig(nullptr) {}

    void setOrig(std::streambuf *o)
    {
        orig = o;
    }

    virtual int overflow(int c) override
    {
        if (c == EOF)
            return EOF;
        if (orig)
            orig->sputc(static_cast<char>(c)); // still print to real console
        capture.put(static_cast<char>(c));     // also store in capture
        return c;
    }

    virtual std::streamsize xsputn(const char *s, std::streamsize size) override
    {
        if (orig)
            orig->sputn(s, size);
        capture.write(s, size);
        return size;
    }

    std::string str() const
    {
        return capture.str();
    }

    void clear()
    {
        capture.str("");
        capture.clear();
    }
};

// Global TeeBuf
static TeeBuf tee;

void initializeDungeon(
    dungeon &Dungeon,
    player &Player,
    const sf::Texture &roomTexture,
    const sf::Texture &enemyTexture,
    const sf::Texture &trapTexture,
    const sf::Texture &treasureTexture)
{
    int goblinHealth = 20 + std::rand() % 21;
    int orcHealth = 140 + std::rand() % 21;
    int darkKnightHealth = 570 + std::rand() % 31;
    int dragonHealth = 595 + std::rand() % 41;

    // ROOM 1: Goblin + Gold Coin
    room *r1 = new room("A dark and eerie room", nullptr, new treasure("Gold Coin", 10));
    r1->settrap(nullptr);

    enemy *goblin = new enemy("Goblin", goblinHealth, 30, 5, 2, 1);
    goblin->setTexture(enemyTexture);
    goblin->setPosition({650.f, 250.f});

    treasure *gcoin = dynamic_cast<treasure *>(r1->gettreasure());
    if (gcoin)
    {
        gcoin->setTexture(treasureTexture);
        gcoin->setPosition({200.f, 350.f});
    }
    r1->setBackgroundTexture(roomTexture);
    r1->setPosition({0.f, 0.f});
    Dungeon.addnew(r1, goblin);

    // ROOM 2: Silver Sword, no enemy
    room *r2 = new room("A chamber with shining silver lights", nullptr, new treasure("Silver Sword", 500));
    r2->settrap(nullptr);
    treasure *ssword = dynamic_cast<treasure *>(r2->gettreasure());
    if (ssword)
    {
        ssword->setTexture(treasureTexture);
        ssword->setPosition({200.f, 350.f});
    }
    r2->setBackgroundTexture(roomTexture);
    r2->setPosition({0.f, 0.f});
    Dungeon.addnew(r2, nullptr);

    // ROOM 3: Orc, no treasure
    room *r3 = new room("A battle-scarred room filled with weapons", nullptr, nullptr);
    r3->settrap(nullptr);

    enemy *orcEntity = new enemy("Orc", orcHealth, 50, 8, 4, 2);
    orcEntity->setTexture(enemyTexture);
    orcEntity->setPosition({550.f, 170.f});
    r3->setBackgroundTexture(roomTexture);
    r3->setPosition({0.f, 0.f});
    Dungeon.addnew(r3, orcEntity);

    room *r4 = new room("A hidden treasure vault", nullptr, new treasure("Ruby Gem", 100));
    r4->settrap(std::make_unique<Trap>(25));
    treasure *rgem = dynamic_cast<treasure *>(r4->gettreasure());
    if (rgem)
    {
        rgem->setTexture(treasureTexture);
        rgem->setPosition({200.f, 350.f});
    }
    Trap *tptr = r4->gettrap();
    if (tptr)
    {
        tptr->setTexture(trapTexture);
        tptr->setPosition({500.f, 450.f});
    }
    r4->setBackgroundTexture(roomTexture);
    r4->setPosition({0.f, 0.f});
    Dungeon.addnew(r4, nullptr);

    room *r5 = new room("The Knight's Dungeon", nullptr, nullptr);
    r5->settrap(nullptr);

    enemy *darkKnight = new enemy("Dark Knight", darkKnightHealth, 80, 12, 4, 3);
    darkKnight->setTexture(enemyTexture);
    darkKnight->setPosition({650.f, 250.f});
    r5->setBackgroundTexture(roomTexture);
    r5->setPosition({0.f, 0.f});
    Dungeon.addnew(r5, darkKnight);

    room *r6 = new room(
        "A massive cave with an ancient dragon guarding treasure!",
        nullptr,
        new treasure("Dragon's Hoard", 300));
    r6->settrap(nullptr);

    enemy *ancientDragon = new enemy("Ancient Dragon", dragonHealth, dragonHealth, 15, 8, 4);
    ancientDragon->setTexture(enemyTexture);
    ancientDragon->setPosition({650.f, 250.f});
    treasure *dhoard = dynamic_cast<treasure *>(r6->gettreasure());
    if (dhoard)
    {
        dhoard->setTexture(treasureTexture);
        dhoard->setPosition({200.f, 350.f});
    }
    r6->setBackgroundTexture(roomTexture);
    r6->setPosition({0.f, 0.f});
    Dungeon.addnew(r6, ancientDragon);

    // Finally, place Player at base “fight stance” (350, 200)
    Player.setPosition({550.f, 250.f});
}

// ───────────────────────────────────────────────────────────────────────────────
// main(): loads textures, shows on-screen name‐entry GUI, then runs SFML loop.
// ───────────────────────────────────────────────────────────────────────────────
int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // (0) Install TeeBuf so that std::cout → both real console & capture
    tee.setOrig(std::cout.rdbuf());
    std::cout.rdbuf(&tee);

    // (1) Load SFML textures; exit if any fail
    sf::Texture roomTexture;
    if (!roomTexture.loadFromFile("resources/room_background.png"))
    {
        std::cerr << "Error: Failed to load resources/room_background.png\n";
        return -1;
    }

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("resources/player.png"))
    {
        std::cerr << "Error: Failed to load resources/player.png\n";
        return -1;
    }

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("resources/enemy.png"))
    {
        std::cerr << "Error: Failed to load resources/enemy.png\n";
        return -1;
    }

    sf::Texture trapTexture;
    if (!trapTexture.loadFromFile("resources/trap.png"))
    {
        std::cerr << "Error: Failed to load resources/trap.png\n";
        return -1;
    }

    sf::Texture treasureTexture;
    if (!treasureTexture.loadFromFile("resources/treasure.png"))
    {
        std::cerr << "Error: Failed to load resources/treasure.png\n";
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Dungeon Escape 2D");
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = false;
    if (!font.openFromFile("resources/arial.ttf"))
    {
        std::cerr << "Warning: Could not load font arial.ttf. On‐screen text will be skipped.\n";
    }
    else
    {
        fontLoaded = true;
    }

    std::string playerName;
    bool enteringName = true;

    sf::Text promptText(font, "🏰 Enter your name, brave adventurer: 🏰", 28u);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition({50.f, 200.f});

    sf::Text nameText(font, "", 28u);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition({50.f, 260.f});

    while (window.isOpen() && enteringName)
    {
        std::optional<sf::Event> evtOpt;
        while ((evtOpt = window.pollEvent()))
        {
            sf::Event event = *evtOpt;

            if (event.is<sf::Event::Closed>())
            {
                return 0;
            }
            if (event.is<sf::Event::KeyPressed>())
            {
                auto *kp = event.getIf<sf::Event::KeyPressed>();
                if (!kp)
                    continue;
                sf::Keyboard::Key code = kp->code;

                if (code == sf::Keyboard::Key::Enter)
                {
                    if (!playerName.empty())
                        enteringName = false;
                }
                else if (code == sf::Keyboard::Key::Backspace)
                {
                    if (!playerName.empty())
                        playerName.pop_back();
                }
                else
                {
                    if (code >= sf::Keyboard::Key::A && code <= sf::Keyboard::Key::Z)
                    {
                        bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
                                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
                        int offset = int(code) - int(sf::Keyboard::Key::A);
                        char c = static_cast<char>('a' + offset);
                        if (shift)
                            c = static_cast<char>('A' + offset);
                        playerName.push_back(c);
                    }
                    else if (code >= sf::Keyboard::Key::Num0 && code <= sf::Keyboard::Key::Num9)
                    {
                        int offset = int(code) - int(sf::Keyboard::Key::Num0);
                        char digit = static_cast<char>('0' + offset);
                        playerName.push_back(digit);
                    }
                    else if (code == sf::Keyboard::Key::Space)
                    {
                        playerName.push_back(' ');
                    }
                }
            }
        }

        nameText.setString(playerName);
        window.clear(sf::Color::Black);
        if (fontLoaded)
        {
            window.draw(promptText);
            window.draw(nameText);
        }
        window.display();
    }

    // (5) Create Player now that we have a name
    player Player(playerName);
    Player.setTexture(playerTexture);

    // (6) Create the Dungeon instance
    dungeon Dungeon(roomTexture);

    // (7) Build all rooms/enemies/treasures/traps
    initializeDungeon(
        Dungeon,
        Player,
        roomTexture,
        enemyTexture,
        trapTexture,
        treasureTexture);

    // ───────────────────────────────────────────────────────────────────────────────
    // Timing & state variables
    bool inCombat = false;
    room *combatRoom = nullptr;
    float displayedHumidity = 0.0f;
    bool deathTimerStarted = false;
    sf::Clock deathClock;
    bool victoryTimerStarted = false;
    sf::Clock victoryClock;
    bool playerLungeActive = false;
    bool enemyLungeActive = false;
    sf::Clock lungeClock;
    bool walkForwardActive = false;
    sf::Clock walkClock;

    // Base positions for “fight stance”
    const sf::Vector2f playerBasePos = {450.f, 250.f};
    const sf::Vector2f enemyBasePos = {550.f, 250.f};

    std::string overlayText;
    tee.clear();

    // Instantiate a single AchievementTracker (we’ll call displayAll() when needed)
    AchievementTracker tracker;

    while (window.isOpen())
    {
        // If player died, move them down 20px once, then show “You lost!” for 4 s
        if (!Player.isalive())
        {
            static bool didLieDown = false;
            if (!didLieDown)
            {
                Player.setPosition({playerBasePos.x, playerBasePos.y + 20.f});
                didLieDown = true;
            }
            if (!deathTimerStarted)
            {
                std::cout << "You lost!\n";
                overlayText = tee.str();
                tee.clear();
                deathClock.restart();
                deathTimerStarted = true;
            }
            else if (deathClock.getElapsedTime().asSeconds() >= 4.0f)
            {
                window.close();
            }
        }

        // Poll events
        std::optional<sf::Event> evtOpt;
        while ((evtOpt = window.pollEvent()))
        {
            sf::Event event = *evtOpt;

            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (!Player.isalive() || victoryTimerStarted)
            {
                // Do nothing if player is dead or victory screen is active
            }
            // ───── Combat‐mode inputs ─────────────────────────────────────────
            else if (inCombat && event.is<sf::Event::KeyPressed>())
            {
                try
                {
                    auto *kp = event.getIf<sf::Event::KeyPressed>();
                    if (!kp)
                        throw std::runtime_error("Invalid event");

                    sf::Keyboard::Key code = kp->code;
                    switch (code)
                    {
                    case sf::Keyboard::Key::A:
                    {
                        // 1) Lunge both forward 10px
                        if (!playerLungeActive && !enemyLungeActive)
                        {
                            Player.setPosition({playerBasePos.x + 10.f, playerBasePos.y});
                            if (combatRoom && combatRoom->hasenemy())
                                combatRoom->getenemy()->setPosition({enemyBasePos.x - 10.f, enemyBasePos.y});
                            lungeClock.restart();
                            playerLungeActive = enemyLungeActive = true;
                        }

                        // 2) Run battle logic
                        Dungeon.battle(Player);
                        {
                            std::string temp = tee.str();
                            overlayText = temp.empty()
                                              ? "(Battle round had no console output.)"
                                              : temp;
                        }
                        tee.clear();

                        // Append actual post-attack health so overlay matches health bar
                        overlayText += "\nCurrent HP: " + std::to_string(Player.gethealth());

                        // 3) If enemy died, it’s removed from the room; you can’t “lie it down” after removal.
                        if (combatRoom && !combatRoom->hasenemy())
                        {
                            inCombat = false;
                            combatRoom = nullptr;
                            // Snap player back
                            Player.setPosition(playerBasePos);
                        }

                        // 4) If player died as a result of battle, set alive=false
                        if (Player.gethealth() <= 0)
                        {
                            Player.setalive(false);
                        }
                        break;
                    }

                    case sf::Keyboard::Key::E:
                    {
                        Dungeon.goback();
                        inCombat = false;
                        combatRoom = nullptr;
                        overlayText.clear();
                        tee.clear();
                        if (Dungeon.getCurrentRoom())
                            displayedHumidity = Dungeon.getCurrentRoom()->checkHumidity();
                        break;
                    }

                    default:
                        throw std::runtime_error("Invalid key in combat mode");
                    }
                }
                catch (const std::exception &ex)
                {
                    overlayText = "Error: Unrecognized key pressed!";
                    tee.clear();
                }
            }
            // ───── Menu‐mode inputs (not in combat) ─────────────────────────
            else if (!inCombat && event.is<sf::Event::KeyPressed>())
            {
                try
                {
                    auto *kp = event.getIf<sf::Event::KeyPressed>();
                    if (!kp)
                        throw std::runtime_error("Invalid event");

                    sf::Keyboard::Key code = kp->code;
                    switch (code)
                    {
                    case sf::Keyboard::Key::Num1:
                    {
                        overlayText.clear();

                        // Walk‐forward animation (10px) for 0.1s
                        if (!walkForwardActive)
                        {
                            Player.setPosition({playerBasePos.x + 10.f, playerBasePos.y});
                            walkClock.restart();
                            walkForwardActive = true;
                        }

                        if (Dungeon.moveroom(Player))
                        {
                            // If this returns true, we’ve exited the final room → “Victory!”
                            if (!victoryTimerStarted)
                            {
                                std::cout << "Victory!\n";
                                overlayText = tee.str();
                                tee.clear();
                                victoryClock.restart();
                                victoryTimerStarted = true;
                            }
                            break;
                        }

                        room *justArrived = Dungeon.getCurrentRoom();
                        if (justArrived)
                            displayedHumidity = justArrived->checkHumidity();

                        // If room has trap, handle it...
                        std::string trapMsg;
                        if (justArrived && justArrived->hastrap())
                        {
                            Trap *T = justArrived->gettrap();
                            if (T && T->isArmed())
                            {
                                T->trigger();
                                int dmg = T->getDamage();
                                Player.sethealth(Player.gethealth() - dmg);
                                std::cout << "☠️ Trap dealt " << dmg
                                          << " damage! Current HP: " << Player.gethealth() << "\n";
                                trapMsg = tee.str();
                                tee.clear();
                                T->disarm();
                                if (Player.gethealth() <= 0)
                                {
                                    std::cout << "💀 The trap killed you!\n";
                                    Player.setalive(false);
                                    Player.setPosition({playerBasePos.x, playerBasePos.y + 20.f});
                                    trapMsg += "\n" + tee.str();
                                    tee.clear();
                                }
                            }
                        }

                        if (justArrived && justArrived->hasenemy())
                        {
                            inCombat = true;
                            combatRoom = justArrived;
                            if (!trapMsg.empty())
                                overlayText = trapMsg + "\nAn enemy appears! Press 'A' to Attack or 'E' to Escape.";
                            else
                                overlayText = "An enemy appears! Press 'A' to Attack or 'E' to Escape.";
                        }
                        else
                        {
                            overlayText = trapMsg;
                        }
                        break;
                    }

                    case sf::Keyboard::Key::Num2:
                        overlayText.clear();
                        Dungeon.goback();
                        tee.clear();
                        if (Dungeon.getCurrentRoom())
                            displayedHumidity = Dungeon.getCurrentRoom()->checkHumidity();
                        break;

                    case sf::Keyboard::Key::Num3:
                    {
                        Player.showinventory();
                        overlayText = tee.str();
                        if (overlayText.empty())
                            overlayText = "(Your inventory is empty)";
                        tee.clear();
                        break;
                    }

                    case sf::Keyboard::Key::Num4:
                        window.close();
                        break;

                    case sf::Keyboard::Key::Num5:
                    {
                        std::cout << Player;
                        std::cout << "Room Humidity: " << displayedHumidity << "%\n";
                        overlayText = tee.str();
                        if (overlayText.empty())
                            overlayText = "(No player details or achievements available)";
                        tee.clear();
                        break;
                    }

                    default:
                        throw std::runtime_error("Invalid key in menu mode");
                    }
                }
                catch (const std::exception &ex)
                {
                    overlayText = "Error: Unrecognized key pressed!";
                    tee.clear();
                }
            }
            else if (!inCombat)
            {
                Dungeon.handleInput(event, Player);
            }
        }

        // Complete “lunge” after 0.1s
        if (playerLungeActive && enemyLungeActive && lungeClock.getElapsedTime().asSeconds() >= 0.1f)
        {
            Player.setPosition(playerBasePos);
            if (combatRoom && combatRoom->hasenemy())
                combatRoom->getenemy()->setPosition(enemyBasePos);
            playerLungeActive = enemyLungeActive = false;
        }

        // Complete “walk forward” after 0.1s
        if (walkForwardActive && walkClock.getElapsedTime().asSeconds() >= 0.1f)
        {
            Player.setPosition(playerBasePos);
            walkForwardActive = false;
        }

        // After 4 s of “Victory!”, close the window
        if (victoryTimerStarted && victoryClock.getElapsedTime().asSeconds() >= 4.0f)
        {
            window.close();
        }

        // (8b) Clear previous frame
        window.clear(sf::Color::Black);

        // (8c) Draw current room’s background
        Dungeon.drawCurrentRoom(window);

        // (8d) Draw room description and humidity
        room *currentRoom = Dungeon.getCurrentRoom();
        std::string desc;
        if (currentRoom && fontLoaded)
        {
            desc = currentRoom->getdescription();
            if (!desc.empty())
            {
                sf::Text roomText(font, desc, 20u);
                roomText.setFillColor(sf::Color::Black);
                roomText.setPosition({50.f, 50.f});
                window.draw(roomText);

                std::ostringstream humSS;
                humSS << "Humidity: " << displayedHumidity << "%";
                sf::Text humText(font, humSS.str(), 18u);
                humText.setFillColor(sf::Color::Black);
                humText.setPosition({50.f, 80.f});
                window.draw(humText);
            }
        }

        // (8e) Draw the player + HUD (health bar at far right)
        Player.draw(window);
        if (fontLoaded)
        {
            float hp = static_cast<float>(Player.gethealth());
            float maxHp = static_cast<float>(Player.getmaxhealth());
            float ratio = std::max(0.f, hp / maxHp);

            // Gray back‐bar
            sf::RectangleShape backBar({100.f, 20.f});
            backBar.setFillColor(sf::Color(50, 50, 50));
            backBar.setPosition({800.f - 110.f, 10.f});
            window.draw(backBar);

            // Red HP bar
            sf::RectangleShape hpBar({100.f * ratio, 20.f});
            hpBar.setFillColor(sf::Color(200, 0, 0));
            hpBar.setPosition({800.f - 110.f, 10.f});
            window.draw(hpBar);

            // HP text
            std::ostringstream hpSS;
            hpSS << static_cast<int>(hp) << "/" << static_cast<int>(maxHp);
            sf::Text hpText(font, hpSS.str(), 14u);
            hpText.setFillColor(sf::Color::Black);
            hpText.setPosition({800.f - 108.f, 12.f});
            window.draw(hpText);

            if (currentRoom)
            {
                if (desc == "A dark and eerie room" || desc == "A chamber with shining silver lights" || desc == "A hidden treasure vault" || desc == "A massive cave with an ancient dragon guarding treasure!")
                {
                    sf::Sprite treasureIcon(treasureTexture);
                    treasureIcon.setPosition({800.f - 110.f - 50.f - 50.f, 35.f + 400.f}); // below HP bar/////
                    window.draw(treasureIcon);
                }
            }
        }

        if (currentRoom && currentRoom->hasenemy())
        {
            enemy *E = currentRoom->getenemy();
            if (E)
                E->draw(window);
        }

        if (currentRoom && currentRoom->hastrap())
        {
            Trap *T = currentRoom->gettrap();
            if (T)
                T->draw(window);
        }

        // (8i) Draw menu options (1–5) if not in combat or victory
        if (!inCombat && !victoryTimerStarted && fontLoaded)
        {
            const float menuY = 480.f;
            const float x0 = 50.f;
            const float lineSpace = 22.f;

            sf::Text option1(font, "1: Move Forward", 18u);
            option1.setFillColor(sf::Color::Black);
            option1.setPosition({x0, menuY});
            window.draw(option1);

            sf::Text option2(font, "2: Go Back", 18u);
            option2.setFillColor(sf::Color::Black);
            option2.setPosition({x0, menuY + lineSpace});
            window.draw(option2);

            sf::Text option3(font, "3: Show Inventory", 18u);
            option3.setFillColor(sf::Color::Black);
            option3.setPosition({x0, menuY + 2.f * lineSpace});
            window.draw(option3);

            sf::Text option4(font, "4: Exit Game", 18u);
            option4.setFillColor(sf::Color::Black);
            option4.setPosition({x0, menuY + 3.f * lineSpace});
            window.draw(option4);

            sf::Text option5(font, "5: Show Details", 18u);
            option5.setFillColor(sf::Color::Black);
            option5.setPosition({x0, menuY + 4.f * lineSpace});
            window.draw(option5);
        }

        if (!overlayText.empty() && fontLoaded)
        {
            std::istringstream iss(overlayText);
            std::string line;
            float y = 100.f; // beneath humidity
            while (std::getline(iss, line))
            {
                unsigned int charSize = 18u;
                sf::Color textColor = sf::Color::Black;

                if (line == "You lost!")
                {
                    charSize = 36u;
                    textColor = sf::Color::Black;
                }
                else if (line == "Victory!")
                {
                    charSize = 36u;
                    textColor = sf::Color::Black;
                }

                sf::Text lineText(font, line, charSize);
                lineText.setFillColor(textColor);
                lineText.setPosition({50.f, y});
                window.draw(lineText);

                y += (charSize == 36u ? 40.f : 20.f);
                if (y > 450.f)
                    break;
            }
        }

        window.display();
    }
}