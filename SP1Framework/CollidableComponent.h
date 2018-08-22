#ifndef COLLIDABLE_COMPONENT_H
#define COLLIDABLE_COMPONENT_H

#include "Component.h"


class Manager;


class CollidableComponent : public Component
{
public:

	// Forward Declaration
	Manager * manager;

	CollidableComponent();
	CollidableComponent(GameObject *owner);
	virtual ~CollidableComponent();

};


#endif

