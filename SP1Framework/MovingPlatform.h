#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "CollidableComponent.h"
#include "Manager.h"

class Manager;

class MovingPlatform : public GameObject
{
public:
	Manager * manager;
	MovingPlatform();
	MovingPlatform(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args);
	virtual ~MovingPlatform();
private:

};

#endif