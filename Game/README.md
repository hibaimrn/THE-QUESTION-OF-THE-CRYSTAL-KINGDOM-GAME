
---

# Crystal Kingdom Quest

Crystal Kingdom Quest is a text-based adventure game where the player navigates through a map filled with obstacles, rewards, enemies, and challenges. The goal of the game is to reach the crystal located within the map while collecting rewards and avoiding obstacles and enemies.

## Features

- **Map Navigation**: Move the character through the map using row and column coordinates.
- **Inventory Management**: Collect jewels, potions, and weapons to increase your score and inventory count.
- **Challenges**: Encounter enemies such as goblins, dragons, and werewolves that can diminish your inventory.
- **Obstacles**: Navigate through obstacles represented by '#' symbols on the map.
- **Death Points**: Avoid death points marked by '%' symbols, which end the game.
- **Crystal Goal**: Reach the crystal '*' to complete the game and achieve victory.

## Entities on the Map

- **'.'**: Clear path where the character can move.
- **'J'**: Jewel, collectible item that adds to the player's score.
- **'P'**: Potion, collectible item that increases the player's inventory of potions.
- **'W'**: Weapon, collectible item that increases the player's inventory of weapons.
- **'$'**: Goblin, enemy that reduces the player's potion inventory upon encounter.
- **'&'**: Dragon, enemy that reduces the player's jewel inventory upon encounter.
- **'@'**: Werewolf, enemy that reduces the player's weapon inventory upon encounter.
- **'#'**: Obstacle, impassable terrain.
- **'%'**: Death Point, area that ends the game upon entry.
- **'*'**: Crystal, the goal point to win the game.

## Instructions

1. Use row and column coordinates to move the character through the map.
2. Collect jewels, potions, and weapons to increase your score and inventory.
3. Beware of enemies, as encounters with goblins, dragons, and werewolves can reduce your inventory.
4. Navigate around obstacles and avoid death points to stay alive.
5. Reach the crystal '*' to complete the game and achieve victory.

## Compilation and Execution

Compile the program using a C++ compiler and execute the compiled binary to start the game.

Example (using g++ compiler):
```bash
g++ main.cpp -o crystal_kingdom_quest
./crystal_kingdom_quest
```

## Credits

This game is created by  Hiba Imran.

---
