#ifndef PRESSUREPLATE_H
#define PRESSUREPLATE_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "LogicComponent.h"
#include "CollidableComponent.h"

class Manager;

class PressurePlate : public GameObject
{
public:
	Manager * manager;
	PressurePlate();
	PressurePlate(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args);
	GameObject* getTrigger();
	void setTrigger(bool state);
	virtual void update();
	virtual ~PressurePlate();
	void resetTransition();
private:
	GameObject * trigger;
	COORD posActivated;
	COORD posDeactivated;
	bool inTransition = false;
	int cooldownCounter = 0;
	int transitionCooldown = 30;
	bool activated;
};

#endif