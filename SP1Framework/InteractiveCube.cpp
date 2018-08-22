#include "InteractiveCube.h"
#include "Manager.h"


InteractiveCube::InteractiveCube()
{
}

InteractiveCube::InteractiveCube(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args)
	: GameObject("InteractiveCube", position, width, height, uid, args) {

	manager = m;

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
			g.colour = m->interpretColour(BROWN, WHITE);

			temp.push_back(g);
		}
		r->addToGraphics(y, temp);
		temp.clear();
	}
}

InteractiveCube::~InteractiveCube() {}

void InteractiveCube::update() {
	COORD pos = getPosition();
	GameObject* objectBelow = manager->placeMeeting(pos.X, pos.Y + 1, "");
	if (objectBelow == NULL) {
		if (manager->getTimer(TIMER_GRAVITY) >= 0.08) {
			pos.Y++;
			setPosition(pos);
			manager->setTimer(TIMER_GRAVITY, 0);
		}
	}

}

int InteractiveCube::push(int velocityX) {
	AABB* bound = &getAABB();
	//int widthOffset = (velocityX > 0) ? getWidth() : 0;
	bound->P1.X+= (SHORT)velocityX;
	bound->P2.X = bound->P1.X + getWidth() - 1;

	SHORT finalVelocityX = 0;

	GameObject* object = manager->placeMeetingBound(*bound, "");
	if (object != NULL) {
		bound = &getAABB();
		SHORT dir = (SHORT) manager->sign(velocityX);
		bound->P1.X += dir;
		bound->P2.X += dir;
		object = manager->placeMeetingBound(*bound, "");
		while (object == NULL) {

			bound->P1.X += dir;
			bound->P2.X += dir;
			finalVelocityX += dir;
			object = manager->placeMeetingBound(*bound, "");
		}
	}
	else {
		finalVelocityX = velocityX;
	}
	COORD pos = getPosition();
	pos.X += finalVelocityX;
	setPosition(pos);
	return finalVelocityX;

}