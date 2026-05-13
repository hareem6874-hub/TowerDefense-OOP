#ifndef TOWER_H
#define TOWER_H

#include "Enemy.h"

class Tower : public Entity 
{
protected:
    int damage;
    float range;
    float fireRate;
    float timer;
    int level;
    int upgradeCost;
public:
    Tower() 
    {
        damage = 20;
        range = 150;
        fireRate = 1;
        timer = 0;
        level = 1;
        upgradeCost = 50;
    }

    virtual void attack(Enemy* enemies[], int totalEnemies) = 0;

    virtual void update(float timePerFrame) 
    {
        timer += timePerFrame;
    }

    virtual void upgrade() 
    {
        level++;
        damage += 10;
        range += 20;
        upgradeCost += 50;
    }

    int getLevel() 
    {
        return level;
    }

    FloatRect getGlobalBounds() 
    {
        return body.getGlobalBounds();
    }
    
    bool contains(float mx, float my) 
    {
        return body.getGlobalBounds().contains(mx, my);
    }

    int getUpgradeCost() 
    {
        return upgradeCost;
    }

    virtual void draw(RenderWindow& window) 
    {
        window.draw(body);
    }

    virtual ~Tower() {}
};

#endif