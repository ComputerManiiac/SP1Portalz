#include "CollidableComponent.h"



CollidableComponent::CollidableComponent(){}


CollidableComponent::~CollidableComponent(){}

CollidableComponent::CollidableComponent(GameObject *parent)
	:Component(parent, COMP_COLLIDABLE) {

}
