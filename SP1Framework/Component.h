#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"
#include <string>
#include <algorithm>

class Manager;

class Component
{
public:
	Manager* m;
	Component();
	Component(GameObject* parent, ComponentType id);
	virtual ~Component();
	void setActive(bool active);
	bool isActive();
	void call();
	bool hasID(ComponentType id);
	void setID(ComponentType id);
	GameObject* getParent();
	void setParent(GameObject* parent);
private:
	bool active;
	GameObject* parent;
	int id;
};


#endif