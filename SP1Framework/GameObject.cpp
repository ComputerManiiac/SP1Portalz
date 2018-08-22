#include "GameObject.h"
#include "Component.h"
#include "Manager.h"
#include "Wall.h"
#include "MovingPlatform.h"
#include "Portal.h"
#include "Player.h"

GameObject::GameObject()
{

}

GameObject::GameObject(std::string name, COORD position, int width, int height, int uid, std::vector<std::string> args) {

	this->name = name;
	this->position = position;
	this->width = width;
	this->height = height;
	this->UID = uid;

	for (int i = 0; i < COMP_COUNT; i++)
		componentList[i] = 0;
}

GameObject::~GameObject()
{

}

AABB GameObject::getAABB() {
	AABB bounds = AABB();
	bounds.P1 = position;
	bounds.P2 = { position.X + (SHORT)width - 1, position.Y + (SHORT)height - 1 };
	return bounds;
}

SHORT GameObject::getRightX() {
	return position.X + (SHORT) width - 1;
}

SHORT GameObject::getBottomY() {
	return position.Y + (SHORT)height - 1;
}

void GameObject::setPosition(COORD position) {
	this->position = position;
}

COORD GameObject::getPosition() {
	return this->position;
}

void GameObject::setDimensions(int width, int height) {
	this->width = width;
	this->height = height;
}

void GameObject::setActive(bool state) {
	this->isActive = state;
}



int GameObject::getWidth() {
	return this->width;
}

int GameObject::getHeight() {
	return this->height;
}

std::string GameObject::getName() {
	return this->name;
}

int GameObject::getUID() {
	return this->UID;
}

Component* GameObject::getComponent(ComponentType id) {
	return this->componentList[id];
}

void GameObject::addComponent(Component* comp, ComponentType type) {
	componentList[type] = comp;
}

bool GameObject::hasComponent(ComponentType type) {
	if (componentList[type] == 0 || (componentList[type] != 0 && !componentList[type]->isActive()))
		return false;
	else
		return true;
}


Component* GameObject::getFirstComponentInList() {
	return componentList[0];
}

void GameObject::destroy() {
	for (int i = 0; i < (int)COMP_COUNT; i++) {
		if (componentList[i] != 0) {
			componentList[i]->setActive(false);
		}
	}

	GameObject::~GameObject();
}