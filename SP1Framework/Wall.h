#ifndef WALL_H
#define WALL_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "CollidableComponent.h"

class Manager;

class Wall : public GameObject
{
public:
	Manager *manager;
	Wall();
	Wall(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args);
	virtual ~Wall();
private:
	bool canPortal;
};

#endif