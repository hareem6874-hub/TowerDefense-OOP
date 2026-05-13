#ifndef MACHINEGUNTOWER_H
#define MACHINEGUNTOWER_H

#include "Tower.h"

class MachineGunTower : public Tower 
{
public:
    MachineGunTower(float x, float y) 
    {
        damage = 10;
        range = 130;
        fireRate = 0.2;

        body.setSize(Vector2f(40, 40));
        body.setFillColor(Color::Green);
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
    
        damage += 5;
    
        if (fireRate > 0.05)
            fireRate -= 0.02;
    
        range += 10;
    
        upgradeCost += 40;
    }
};

#endif