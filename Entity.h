#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity 
{
protected:
	RectangleShape body;

public:
	virtual void update(float timePerFrame) = 0;
	virtual void draw(RenderWindow& window) = 0;

	virtual ~Entity() {}
};

#endif