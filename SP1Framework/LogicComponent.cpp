#include "LogicComponent.h"
#include "Manager.h"


LogicComponent::LogicComponent(){}
LogicComponent::~LogicComponent(){}


LogicComponent::LogicComponent(GameObject *owner) : Component(owner, COMP_LOGIC) {

}



void LogicComponent::update(std::vector<std::string> args) {
	
}


