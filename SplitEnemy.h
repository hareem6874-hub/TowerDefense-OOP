#ifndef SPLITENEMY_H
#define SPLITENEMY_H

#include "Enemy.h"

class SplitEnemy : public Enemy 
{
public:
    SplitEnemy(float y) 
    {
        speed = 90;
        hp = 150;
        maxHp = 150;

        body.setSize(Vector2f(45, 45));
        body.setFillColor(sf::Color::Black);
        body.setPosition(0, y);
    }

    void move(float timePerFrame) 
    {
        body.move(speed * timePerFrame, 0);
    }
};

#endif