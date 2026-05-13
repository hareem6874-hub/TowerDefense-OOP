#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "Tower.h"

class SniperTower : public Tower 
{
public:
    SniperTower(float x, float y) 
    {
        damage = 100;
        range = 400;
        fireRate = 2;
        body.setSize(Vector2f(45, 45));
        body.setFillColor(Color::White);
        body.setPosition(x, y);
    }

    void attack(Enemy* enemies[], int totalEnemies) 
    {
        if (timer < fireRate) 
        {
            return;
        }

        for (int i = 0; i < totalEnemies; i++) 
        {
            float dx = enemies[i]->getPos().x - body.getPosition().x;
            float dy = enemies[i]->getPos().y - body.getPosition().y;

            if (dx * dx + dy * dy <= range * range) 
            {
                enemies[i]->takeDamage(damage);
                timer = 0;
                break;
            }
        }
    }

    void upgrade() 
    {
        level++;
        damage += 50;
        range += 30;
        upgradeCost += 80;
    }
};

#endif