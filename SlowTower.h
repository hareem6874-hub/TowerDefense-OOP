#ifndef SLOWTOWER_H
#define SLOWTOWER_H

#include "Tower.h"

class SlowTower : public Tower 
{
public:
    SlowTower(float x, float y) 
    {
        damage = 0;
        range = 120;
        fireRate = 1;
        body.setSize(Vector2f(45, 45));
        body.setFillColor(Color(255, 100, 0));
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
                enemies[i]->applySlow(0.5);
            }
        }

        timer = 0;
    }

    void upgrade()
    {
        level++;
        range += 20;
        if (fireRate > 0.3)
            fireRate -= 0.1;
        upgradeCost += 40;
    }
};

#endif