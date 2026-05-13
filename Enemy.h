#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity 
{
protected:
    float speed;
    float originalSpeed;

    int hp;
    int maxHp;

    bool dead;

    float slowTimer;

public:
    Enemy() 
    {
        speed = 100;
        originalSpeed = speed;

        hp = 100;
        maxHp = 100;

        dead = false;

        slowTimer = 0;
    }

    virtual void move(float timePerFrame) = 0;

    virtual void update(float timePerFrame) 
    {

        if (slowTimer > 0) 
        {
            slowTimer -= timePerFrame;
        }
        else 
        {
            speed = originalSpeed;
        }

        move(timePerFrame);
    }

    void applySlow(float factor) 
    {
        speed = originalSpeed * factor;
        slowTimer = 1.0f;
    }

    void takeDamage(int dmg) 
    {
        hp -= dmg;

        if (hp <= 0)
         {
            dead = true;
        }
    }

    bool isDead() 
    { 
        return dead;
    }

    Vector2f getPos() 
    { 
        return body.getPosition(); 
    }

    void setPosition(float x, float y) 
    {
        body.setPosition(x, y);
    }

    virtual void draw(RenderWindow& window) 
    {
        window.draw(body);

        RectangleShape hpBar;
        hpBar.setSize(Vector2f((float)hp / maxHp * 40, 5));
        hpBar.setFillColor(Color::Green);
        hpBar.setPosition(body.getPosition().x, body.getPosition().y - 10);

        window.draw(hpBar);
    }

    virtual ~Enemy() {}
};

#endif