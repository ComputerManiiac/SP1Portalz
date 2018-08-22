#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>


class Component;
class Manager;
class Wall;
class MovingPlatform;
class Player;
class Portal;

struct AABB {
	COORD P1;
	COORD P2;
};


enum ComponentType {
	COMP_RENDERABLE, COMP_LOGIC,
	COMP_COLLIDABLE, COMP_PATROL,
	COMP_COUNT
};

class GameObject
{
public:
	GameObject();
	GameObject(std::string name, COORD position, int width, int height, int uid, std::vector<std::string> args);
	virtual ~GameObject();

	Manager *manager;
	Wall *wall;
	MovingPlatform *movingPlatform;
	Player *player;
	Portal *portal;
	Component *component;

	AABB getAABB();
	SHORT getRightX();
	SHORT getBottomY();

	void setPosition(COORD position);
	COORD getPosition();
	void setDimensions(int width, int height);
	int getWidth();
	int getHeight();
	void setActive(bool state);
	void destroy();

	std::string getName();
	int getUID();

	bool hasComponent(ComponentType type);

	void addComponent(Component* comp, ComponentType type);
	Component* getComponent(ComponentType id);

	Component* getFirstComponentInList();
private:
	COORD position;
	int width;
	int height;
	int UID = -1;
	bool isActive = true;
	std::string name;
	Component* componentList[COMP_COUNT];
};


#endif