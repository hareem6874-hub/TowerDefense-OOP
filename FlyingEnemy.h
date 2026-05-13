#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H

#include "Enemy.h"

class FlyingEnemy : public Enemy 
{
public:
    FlyingEnemy(float y) 
    {
        speed = 120;
        hp = 80;
        maxHp = 80;

        body.setSize(Vector2f(35, 35));
        body.setFillColor(Color::Cyan);
        body.setPosition(0, y - 80);
    }

    void move(float timePerFrame) 
    { 
        body.move(speed * timePerFrame, 0); 
    }
};

#endif