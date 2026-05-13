#ifndef FIRETOWER_H
#define FIRETOWER_H

#include "Tower.h"

class FireTower : public Tower 
{
public:
    FireTower(float x, float y) 
    {
        damage = 25;
        range = 160;
        fireRate = 0.7f;

        body.setSize(Vector2f(50, 50));
        body.setFillColor(Color::Red);
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
            }
        }

        timer = 0;
    }
};
#endif