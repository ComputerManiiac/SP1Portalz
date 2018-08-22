#include "Component.h"
#include "Manager.h"


//Component::Component()
//{
//}
//
//Component::Component(GameObject* owner, int id) {
//	this->owner = owner;
//	this->id = id;
//}
//
//
//Component::~Component()
//{
//}
//
//GameObject* Component::getOwner() {
//	return this->owner;
//}
//
//void Component::setOwner(GameObject* owner) {
//	this->owner = owner;
//}
//
//void Component::setActive(bool active) {
//	this->active = active;
//}
//
//bool Component::isActive() {
//	return this->active;
//}
//
//void Component::setID(int id) {
//	this->id;
//}
//
//void Component::call(std::string type) {
//	
//
//	/* Old Render Code
//	for (SHORT cY = pos.Y; cY < pos.Y + (SHORT)g->getHeight(); cY++) {
//	for (SHORT cX = pos.X; cX < pos.X + (SHORT)g->getWidth(); cX++) {
//	Manager::getInstance()->getConsole()->writeToBuffer({ cX,cY }, ' ', Manager::getInstance()->interpretColour(BLUE, WHITE));
//	}
//	}*/
//
//	
//}
//
//bool Component::hasID(int compare) {
//	return (compare == id) ? true : false;
//}

Component::Component() {}
Component::~Component() {}

Component::Component(GameObject* parent, ComponentType id) {
	this->setParent(parent);
	this->setID(id);
}

void Component::call() {

}

void Component::setActive(bool active) {
	this->active = active;
}

bool Component::isActive() {
	return this->active;
}

void Component::setID(ComponentType id) {
	this->id;
}

bool Component::hasID(ComponentType id) {
	return (this->id == id);
}

GameObject* Component::getParent() {
	return this->parent;
}

void Component::setParent(GameObject* parent) {
	this->parent = parent;
}