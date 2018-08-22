#include "PressurePlate.h"
#include "Manager.h"


PressurePlate::PressurePlate()
{
}

PressurePlate::PressurePlate(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args)
	: GameObject("PressurePlate", position, width, height, uid, args) {
	
	manager = m;
	activated = false;

	posDeactivated = getPosition();
	posActivated = getPosition();
	posActivated.Y++;

	RenderComponent *r = new RenderComponent(this);
	addComponent(r, COMP_RENDERABLE);
	m->addComponentToCategory(r, COMP_RENDERABLE);

	CollidableComponent *c = new CollidableComponent(this);
	addComponent(c, COMP_COLLIDABLE);
	m->addComponentToCategory(c, COMP_COLLIDABLE);

	LogicComponent *l = new LogicComponent(this);
	addComponent(l, COMP_LOGIC);
	m->addComponentToCategory(l, COMP_LOGIC);

	std::vector<Graphics> temp;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			Graphics g = { ' ', 0 };
			g.colour = m->interpretColour(MAGENTA, WHITE);

			temp.push_back(g);
		}
		r->addToGraphics(y, temp);
		temp.clear();
	}
}

PressurePlate::~PressurePlate() {}

void PressurePlate::update(){


	AABB* bound = &getAABB();
	bound->P1.Y-=2;
	bound->P2.Y--;
	AABB playerBound = manager->getPlayer()->getAABB();
	GameObject *obj = manager->placeMeetingBound(*bound, "");
	if (obj != NULL || manager->placeOverlap(playerBound, *bound)) {
		if(!activated)
			setTrigger(true);
	}
	else {
		setTrigger(false);
	}

}


void PressurePlate::setTrigger(bool state){

	if (state) {
		setPosition(posActivated);
	}else {
		setPosition(posDeactivated);
	}
	activated = state;
}