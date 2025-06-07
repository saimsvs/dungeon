

# Dungeon Escape 2D

A simple top-down 2D dungeon crawler built with SFML. Explore rooms, fight monsters, dodge traps, and collect treasure—all rendered with sprites and a basic HUD.

---

## Features

- **Six unique rooms** with randomized enemy health  
- **Combat system**: press **A** to attack, both you and the enemy lunge  
- **Traps** that deal damage on entry  
- **Treasures** to collect (Gold Coin, Silver Sword, Ruby Gem, Dragon’s Hoard)  
- **Health bar** HUD with numeric / graphical display  
- **Inventory** and **player stats** menus  
- **Victory** and **Game Over** screens  

---

## Controls

| Key     | Action                                            |
| ------- | ------------------------------------------------- |
| 1       | Move forward (enter next room)                    |
| 2       | Go back (return to previous room)                 |
| A       | Attack (when in combat)                           |
| E       | Escape combat (when in combat)                    |
| 3       | Show inventory                                    |
| 5       | Show player details & current room humidity       |
| 4       | Exit game                                         |

---

## Dependencies

- **SFML 3.0** (Graphics, Window, System)  
- A C++17-capable compiler (g++ / clang++ / MSVC)  

---

## Building

1. **Clone** the repo and navigate into it.  
2. **Make sure** SFML’s include and lib paths are set (e.g. in your IDE or Makefile).  
3. **Compile**:  
   ```bash
   g++ src/*.cpp -I/path/to/SFML/include \
      -L/path/to/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system \
      -std=c++17 -o DungeonEscape

4. Run the executable (./DungeonEscape or DungeonEscape.exe).


5. Resources: put your .png and .ttf files in resources/ next to the binary.




---

## Directory Structure

```text
/.
├── main/
│   ├── sfml.cpp
│   └──  resources/
│   
├── src/
│   ├── dungeon.cpp
│   └── … other .cpp
├── headers/
│   ├── dungeon.hpp
│   ├── player.hpp
│   ├── room.hpp
│   └── … others
└── README.md


