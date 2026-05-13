#ifndef TANKENEMY_H
#define TANKENEMY_H

#include "Enemy.h"

class TankEnemy : public Enemy 
{
public:
    TankEnemy(float y) 
    {
        speed = 50;
        hp = 300;
        maxHp = 300;

        body.setSize(Vector2f(60, 60));
        body.setFillColor(Color::Magenta);
        body.setPosition(0, y);
    }

    void move(float timePerFrame) 
    { 
        body.move(speed * timePerFrame, 0); 
    }
};

#endif