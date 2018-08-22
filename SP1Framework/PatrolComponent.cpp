#include "PatrolComponent.h"
#include "Manager.h"


PatrolComponent::PatrolComponent()
{
}

PatrolComponent::PatrolComponent(GameObject *owner) :Component(owner, COMP_PATROL) {
	mode = p_DetectbySteps;
	blocksPerStep = 1;
	axis = AxisX;
}


PatrolComponent::~PatrolComponent()
{
}

void PatrolComponent::updateTime(double deltaTime) {
	moveTimer += deltaTime;
}

void PatrolComponent::setParameters(int direction, int blocksPerStep, int maxStepsInDir, PatrolMode mode, PatrolAxis axis) {
	this->direction = direction;
	this->blocksPerStep = blocksPerStep;
	this->maxStepsInDir = maxStepsInDir;
	this->mode = mode;
	this->axis = axis;
	
}

void PatrolComponent::patrol() {
	//if (moveTimer >= moveSpeed) {
		switch (mode) {
			case p_DetectbySteps: {
				currentSteps++;
				COORD pos = getParent()->getPosition();
				if (axis == AxisX) {
					pos.X += direction * blocksPerStep;
				}
				else if (axis == AxisY) {
					pos.Y += direction * blocksPerStep;
				}
				getParent()->setPosition(pos);
				if (currentSteps >= maxStepsInDir) {
					direction *= -1;
					currentSteps = 0;
				}
			}
			case p_DetectbyBlock: {

			}
		}
		moveTimer = 0.0;
	//}
}
