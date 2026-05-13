#ifndef FASTENEMY_H
#define FASTENEMY_H

#include "Enemy.h"

class FastEnemy : public Enemy 
{
public:
    FastEnemy(float y) 
    {
        speed = 180;
        hp = 50;
        maxHp = 50;

        body.setSize(Vector2f(30, 30));
        body.setFillColor(Color::Yellow);
        body.setPosition(0, y);
    }

    void move(float timePerFrame)
    { 
        body.move(speed * timePerFrame, 0); 
    }
};

#endif