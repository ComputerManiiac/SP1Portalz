#ifndef PATROL_COMPONENT_H
#define PATROL_COMPONENT_H

#include "Component.h"

class Manager;

enum PatrolMode {
	p_DetectbySteps,
	p_DetectbyBlock
};

enum PatrolAxis {
	AxisX,
	AxisY
};

class PatrolComponent : public Component
{
public:
	Manager *manager;
	PatrolComponent();
	PatrolComponent(GameObject *owner);
	virtual void patrol();
	virtual ~PatrolComponent();
	void updateTime(double deltaTime);
	void setParameters(int direction, int blocksPerStep, int maxStepsInDir, PatrolMode mode, PatrolAxis axis);
private:
	int direction = 1;
	int currentSteps = 0;
	int blocksPerStep = 2;
	int maxStepsInDir = 20;
	PatrolMode mode;
	PatrolAxis axis;
	double moveTimer = 0.0;
};

#endif
