#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Player {
protected:
    string name;
    int maxHealth;
    int health;
    int meleeDamage;
    int rangedDamage;
    int defence;
    int level;
    int specialAbilityLevel;
    bool hasSword;
    bool hasShield;
    bool hasArmour;
    bool hasBow;

public:
    Player(string &playerName)
        : name(playerName), maxHealth(100), health(100), meleeDamage(10),
        rangedDamage(5), defence(5), level(1), specialAbilityLevel(1),
        hasSword(false), hasShield(false), hasArmour(false), hasBow(false) {}

    virtual ~Player() {}

    virtual void takeDamage(int damage) {
        int actualDamage = damage - defence;
        if (actualDamage < 0) {
            actualDamage = 0;
        }
        health -= actualDamage;
        cout << name << " took " << actualDamage << " damage.\n";
        if (health <= 0) {
            cout << name << " has been defeated!\n";
        }
    }

    virtual int attack() {
        int totalDamage = meleeDamage;
        if (hasSword && (rand() % 100) < 10) {
            totalDamage *= 2;
            cout << name << " performs a critical hit!\n";
        }
        cout << name << " performs a melee attack for " << totalDamage << " damage.\n";
        return totalDamage;
    }

    virtual void heal() {
        if (health == maxHealth) {
            cout << name << " is already at full health.\n";
        } else {
            health += 20; // Fixed amount of healing for simplicity
            if (health > maxHealth) {
                health = maxHealth;
            }
            cout << name << " healed for 20 health.\n";
        }
    }

    int getHealth() {
        return health;
    }

    bool isDefeated() { return health <= 0; }

    string getName() { return name; }

    void increaseLevel() {
        level++;
        maxHealth += 20;
        health = maxHealth;
        meleeDamage += 5;
        defence += 3;

        switch (level) {
            case 2:
                specialAbilityLevel = 2;
                hasSword = true;
                break;
            case 3:
                specialAbilityLevel = 3;
                hasShield = true;
                break;
            case 4:
                specialAbilityLevel = 4;
                hasArmour = true;
                break;
            case 5:
                specialAbilityLevel = 5;
                hasBow = true;
                break;
            case 6:
                specialAbilityLevel = 6;
                // Boss fight
                break;
        }
    }
};

class Enemy {
public:
    int health;
    int meleeDamage;
    int rangedDamage;
    int defence;

    Enemy(int enemyLevel)
        : health(50 + 25 * enemyLevel), meleeDamage(5 + 5 * enemyLevel),
        rangedDamage(3 + 2 * enemyLevel), defence(3 + 2 * enemyLevel) {}

    void takeDamage(int damage) {
        int actualDamage = damage - defence;
        if (actualDamage < 0) {
            actualDamage = 0;
        }
        health -= actualDamage;
    }

    int attack() {
        int attackType = rand() % 2;
        if (attackType == 0) {
            cout << "Enemy performs a melee attack for " << meleeDamage << " damage.\n";
            return meleeDamage;
        } else {
            cout << "Enemy performs a ranged attack for " << rangedDamage << " damage.\n";
            return rangedDamage;
        }
    }

    bool isDefeated() { return health <= 0; }
};

int main() {
    srand(time(NULL));
    string playerName;

    cout << "Enter your hero's name: ";
    cin >> playerName;

    Player hero(playerName);
    vector<Enemy> enemies;
    int numEnemies = 1;

    for (int level = 1; level <= 6; level++) {
        cout << "Level " << level << " - " << hero.getName() << " vs. " << numEnemies << " enemies:\n";
        for (int i = 0; i < numEnemies; i++) {
            enemies.push_back(Enemy(level));
        }

        while (!hero.isDefeated() && !enemies.empty()) {
            cout << hero.getName() << "'s turn:\n";
            cout << "Choose an action (1 - Attack, 2 - Heal): ";
            int action;
            cin >> action;

            switch (action) {
                case 1: {
                    for (int i = 0; i < enemies.size(); i++) {
                        if (!enemies[i].isDefeated()) {
                            int damage = hero.attack();
                            enemies[i].takeDamage(damage);

                            if (enemies[i].isDefeated()) {
                                cout << "You defeated enemy " << (i + 1) << "!\n";
                                enemies.erase(enemies.begin() + i);
                                i--;
                            } else {
                                cout << "You hit enemy " << (i + 1) << " for " << damage << " damage. Enemy HP: " << enemies[i].health << "\n";
                            }
                        }
                    }
                    break;
                }
                case 2:
                    hero.heal();
                    break;
                default:
                    cout << "Invalid action! Turn skipped.\n";
            }

            for (int i = 0; i < enemies.size(); i++) {
                if (!enemies[i].isDefeated()) {
                    int damage = enemies[i].attack();
                    hero.takeDamage(damage);
                }
            }
            cout << hero.getName() << " has " << hero.getHealth() << " health remaining.\n";
        }

        if (hero.isDefeated()) {
            cout << "Game over! " << hero.getName() << " was defeated on level " << level << ".\n";
            break;
        } else {
            cout << "Level " << level << " completed! " << hero.getName() << " advances to the next level.\n";
            hero.increaseLevel();
            numEnemies++;
        }
    }

    if (hero.isDefeated()) {
        cout << "You couldn't defeat Murlocs. The village remains captured.\n";
    } else {
        cout << "Congratulations! " << hero.getName() << " defeated Murlocs and saved the village!\n";
    }

    return 0;
}