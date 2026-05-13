#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"

class CannonTower : public Tower 
{
public:
    CannonTower(float x, float y) 
    {
        damage = 50;
        range = 170;
        fireRate = 1.5f;

        body.setSize(Vector2f(50, 50));
        body.setFillColor(Color::Blue);
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
    
        damage += 20;
    
        range += 25;
    
        if (fireRate > 0.4f)
            fireRate -= 0.1f;
    
        upgradeCost += 60;
    }
};
#endif