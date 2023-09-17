//main.cpp

#include "Player.h"
#include "Monster.h"

#include "EngineMath.h"
#include <iostream>

const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 10;
const int MAX_MONSTER = 10;
Monster* monsters = nullptr;
int numMonsters = 0;

void drawMap(const Player& player) {
    //system("cls");
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == player.getX() && y == player.getY()) {
                std::cout << 'P';
            } 
            else
            {
                bool flag = false;
                for (int i = 0; i < numMonsters; i++) {
                    if (x == monsters[i].getX() && y == monsters[i].getY() && monsters[i].getIsAlive() == true) {
                        std::cout << 'M';
                        flag = true;
                    }
                }
                if (!flag) { std::cout << '.'; }
            }
        }
        std::cout << std::endl;
    }
}

bool isPositionOccupied(int x, int y, int currentMonsterIndex) {
    for (int i = 0; i < currentMonsterIndex; i++) {
        if (monsters[i].getX() == x && monsters[i].getY() == y) {
            return true;
        }
    }
    return false;
}

int main() {
    int step = 0;
    Player player("Player", 0, 0);
    LCG rng(static_cast<unsigned long>(time(nullptr)));

    std::cout << "How many monsters do you want at the beginning: " ;
    std::cin >> numMonsters;

    monsters = new Monster[numMonsters];  // Dynamically allocate memory

    char tempMonsterName[100];
    for (int i = 0; i < numMonsters; i++) {
        do {
            monsters[i].setAlive(true);
            monsters[i].setX(rng.next(MAP_WIDTH));
            monsters[i].setY(rng.next(MAP_HEIGHT));
            monsters[i].setLifespan(rng.next(20));
        } while ((isPositionOccupied(monsters[i].getX(), monsters[i].getY(), i))||monsters[i].getlifespan() <5); // If the position is occupied, regenerate

        sprintf_s(tempMonsterName, sizeof(tempMonsterName), "Monster%d", i + 1);
        monsters[i].setName(tempMonsterName);
        std::cout << monsters[i].getName() <<"  " << monsters[i].getX() << "  " << monsters[i].getY() << std::endl;
    }

    char tempName[100];
    std::cout << "Please set players' name: ";
    std::cin.ignore();//clear the buffer
    std::cin.getline(tempName, sizeof(tempName));
    player.setName(tempName);

    // Randomly generate a position for Player, ensuring the position is not occupied by a Monster
    int playerX, playerY;
    do {
        playerX = rng.next(MAP_WIDTH);
        playerY = rng.next(MAP_HEIGHT);
    } while (isPositionOccupied(playerX, playerY, numMonsters));

    player.setX(playerX);
    player.setY(playerY);
    std::cout << "X of player: " << player.getX() << "  Y of player: " << player.getY() << std::endl;

    char input;
    bool isGameAlive = true;

    while (isGameAlive) {
        system("cls");
        std::cout << "Score: " << step * 50 << std::endl;
        std::cout << "Player: " << player.getName() << "  X: " << player.getX() << "  Y: " << player.getY() << std::endl;
        for (int i = 0; i < numMonsters; i++) {
            std::cout<< monsters[i].getName() << ":  X:" << monsters[i].getX() << "  Y:" << monsters[i].getY() << "  leftSpan:"<<monsters[i].getleftSpan()<<std::endl;
        }
        drawMap(player);
        std::cout << "input direction of player(W/A/S/D)  or Q to quit: ";
        std::cin >> input;

        // Exit the game if the user inputs 'Q' or 'q'
        if (input == 'Q' || input == 'q') {
            isGameAlive = false;
            continue;
        }

        player.move(input);
        for (int i = 0; i < numMonsters; i++) {
            if (monsters[i].getIsAlive()) { monsters[i].chaseAI(player); }
            if (player.getX() == monsters[i].getX() && player.getY() == monsters[i].getY()) {
                std::cout << "Player touch the Monster!" << std::endl;
                isGameAlive = false;
                break;
            }
        }

        // Every 10 stpes, if there is space, add a new monster
        if (step % 10 == 0 && numMonsters < MAX_MONSTER && step!=0) {
            Monster* newMonsters = new Monster[numMonsters + 1];
            for (int i = 0; i < numMonsters; i++) {
                newMonsters[i] = monsters[i];
            }
            delete[] monsters;
            monsters = newMonsters;  // Point the monsters pointer to the new memory

            do {
                monsters[numMonsters].reset();
                monsters[numMonsters].setAlive(true);
                monsters[numMonsters].setX(rng.next(MAP_WIDTH));
                monsters[numMonsters].setY(rng.next(MAP_HEIGHT));
                monsters[numMonsters].setLifespan(rng.next(20));
                std::cout << "TEST:" << monsters[numMonsters].getlifespan()<<std::endl;
            } while ((isPositionOccupied(monsters[numMonsters].getX(), monsters[numMonsters].getY(), numMonsters)) || monsters[numMonsters].getlifespan() < 5); // If the position is occupied, regenerate
            sprintf_s(tempMonsterName, sizeof(tempMonsterName), "Monster%d", numMonsters + 1);
            monsters[numMonsters].setName(tempMonsterName);
            numMonsters++;
        }

        // Organize the monsters array, moving the living monsters to the left side of the array
        int aliveIndex = 0;

        for (int i = 0; i < numMonsters; i++) {
            if (monsters[i].getIsAlive()) {
                if (i != aliveIndex) {
                    monsters[aliveIndex] = monsters[i];
                }
                aliveIndex++;
            }
        }
        numMonsters = aliveIndex;

        // If the current number of living monsters is less than the previous count, memory size needs to be adjusted
        if (aliveIndex < MAX_MONSTER) {
            Monster* newMonsters = new Monster[aliveIndex];
            for (int i = 0; i < aliveIndex; i++) {
                newMonsters[i] = monsters[i];
            }
            delete[] monsters;
            monsters = newMonsters;
        }

        numMonsters = aliveIndex;
        step++;

    }
    delete[] monsters;
    return 0;
}
