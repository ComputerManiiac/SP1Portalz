#ifndef LOGIC_COMPONENT_H
#define LOGIC_COMPONENT_H

#include "Component.h"

class Manager;

class LogicComponent : public Component
{
public:
	Manager *manager;
	LogicComponent();
	LogicComponent(GameObject *owner);
	virtual void update(std::vector<std::string> args);
	virtual ~LogicComponent();

	
};

#endif
