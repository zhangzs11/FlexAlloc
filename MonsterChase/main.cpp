//main.cpp

#include "Player.h"
#include "Monster.h"
#include <iostream>

const int MAP_WIDTH = 30;
const int MAP_HEIGHT = 10;
const int MAX_MONSTER = 10;
Monster monsters[MAX_MONSTER];
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

class LCG {
private:
    unsigned long long a = 1664525;
    unsigned long long c = 1013904223;
    unsigned long long m = 4294967296; // 2^32
    unsigned long long seed;
public:
    LCG(unsigned long long initialSeed) : seed(initialSeed) {}

    unsigned long long next() {
        seed = (a * seed + c) % m;
        return seed;
    }

    // 返回0到max-1之间的随机数
    unsigned long next(unsigned long max) {
        return next() % max;
    }
};

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

    if (numMonsters > MAX_MONSTER) {
        std::cout << "Too many monsters! Setting to maximum (" << MAX_MONSTER << ")." << std::endl;
        numMonsters = MAX_MONSTER;
    }

    char tempMonsterName[100];
    for (int i = 0; i < numMonsters; i++) {
        do {
            monsters[i].setAlive(true);
            monsters[i].setX(rng.next(MAP_WIDTH));
            monsters[i].setY(rng.next(MAP_HEIGHT));
            monsters[i].setLifespan(rng.next(20));
        } while ((isPositionOccupied(monsters[i].getX(), monsters[i].getY(), i))||monsters[i].getlifespan() <5); // 如果位置被占用，重新生成

        sprintf_s(tempMonsterName, sizeof(tempMonsterName), "Monster%d", i + 1);
        monsters[i].setName(tempMonsterName);
        std::cout << monsters[i].getName() <<"  " << monsters[i].getX() << "  " << monsters[i].getY() << std::endl;
    }

    char tempName[100];
    std::cout << "Please set players' name: ";
    std::cin.ignore();//clear the buffer
    std::cin.getline(tempName, sizeof(tempName));
    player.setName(tempName);

    // 为Player随机生成一个位置，确保这个位置没有被Monster占据
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

        // 如果用户输入Q或q，退出游戏
        if (input == 'Q' || input == 'q') {
            isGameAlive = false;
            continue; // 跳过当前循环的剩余部分
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

        // 每隔10步，如果还有空间，添加一个新的怪物
        if (step % 10 == 0 && numMonsters < MAX_MONSTER && step!=0) {
            // ... 这里加入创建新怪物的代码
            do {
                monsters[numMonsters].reset();
                monsters[numMonsters].setAlive(true);
                monsters[numMonsters].setX(rng.next(MAP_WIDTH));
                monsters[numMonsters].setY(rng.next(MAP_HEIGHT));
                monsters[numMonsters].setLifespan(rng.next(20));
                std::cout << "TEST:" << monsters[numMonsters].getlifespan()<<std::endl;
            } while ((isPositionOccupied(monsters[numMonsters].getX(), monsters[numMonsters].getY(), numMonsters)) || monsters[numMonsters].getlifespan() < 5); // 如果位置被占用，重新生成
            sprintf_s(tempMonsterName, sizeof(tempMonsterName), "Monster%d", numMonsters + 1);
            monsters[numMonsters].setName(tempMonsterName);
            numMonsters++;
        }

        // 整理monster数组，将活的怪物移到数组的左侧
        int aliveIndex = 0; // 用于指向下一个应该放置活怪物的位置
        for (int i = 0; i < numMonsters; i++) {
            if (monsters[i].getIsAlive()) {
                if (i != aliveIndex) { // 如果当前怪物不在它应该在的位置，就移动它
                    monsters[aliveIndex] = monsters[i];
                }
                aliveIndex++; // 移到下一个位置
            }
        }
        numMonsters = aliveIndex; // 更新怪物数量
        step++;
    }
    return 0;
}
