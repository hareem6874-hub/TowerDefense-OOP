#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "Enemy.h"

class BasicEnemy : public Enemy
{
public:
	BasicEnemy(float y)
	{
		speed = 80;
		hp = 100;
		maxHp = 100;

		body.setSize(Vector2f(40, 40));
		body.setFillColor(Color::Red);
		body.setPosition(0, y);
	}

	void move(float timePerFrame)
	{
		body.move(speed * timePerFrame, 0);
	}
};

#endif