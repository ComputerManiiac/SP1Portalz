#ifndef INTERACTIVECUBE_H
#define INTERACTIVECUBE_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "LogicComponent.h"
#include "CollidableComponent.h"

class Manager;

class InteractiveCube : public GameObject
{
public:
	Manager * manager;
	InteractiveCube();
	InteractiveCube(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args);
	int push(int velocityX);
	virtual void update();
	virtual ~InteractiveCube();
};

#endif