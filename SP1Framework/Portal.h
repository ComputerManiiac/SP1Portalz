#ifndef PORTAL_H
#define PORTAL_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "CollidableComponent.h"

class Manager;

enum PortalDir {
	PORTAL_TOP=2,
	PORTAL_RIGHT=3,
	PORTAL_BOTTOM=4,
	PORTAL_LEFT=9
};

class Portal : public GameObject
{
public:
	Manager * manager;
	Portal();
	Portal(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args);
	virtual ~Portal();
	void setPair(Portal *pair);
	void setDirection(PortalDir dir);
	PortalDir getDirection();
	COORD teleport();
	Portal* getPair();
	void onReset();
	bool checkCollision();
	void unstuckMe();
private:
	PortalDir dir;
	Portal *pair;
};

#endif