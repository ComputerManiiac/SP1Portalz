#include "Portal.h"
#include "Manager.h"


Portal::Portal()
{
}

Portal::Portal(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args)
	: GameObject("Portal", position, width, height, uid, { "" }) {

	manager = Manager::getInstance();

	RenderComponent *r = new RenderComponent(this);
	addComponent(r, COMP_RENDERABLE);
	m->addComponentToCategory(r, COMP_RENDERABLE);

	CollidableComponent *c = new CollidableComponent(this);
	addComponent(c, COMP_COLLIDABLE);
	m->addComponentToCategory(c, COMP_COLLIDABLE);

	

	

}

Portal::~Portal()
{

}

void Portal::setPair(Portal *pair) {
	this->pair = pair;
}

void Portal::setDirection(PortalDir dir) {
	this->dir = dir;
	if (dir == PORTAL_TOP || dir == PORTAL_BOTTOM) {
		setDimensions(getHeight() + 2, getWidth() - 1);
	}
	//if (dir == PORTAL_BOTTOM) setPosition({ getPosition().X, getPosition().Y - 1 });
	std::vector<Graphics> temp;
	for (int y = 0; y < getHeight(); y++) {
		for (int x = 0; x < getWidth(); x++) {
			Graphics g = { ' ', 0 };
			g.colour = manager->interpretColour(RED, WHITE);

			temp.push_back(g);
		}
		RenderComponent *r = dynamic_cast<RenderComponent*>(getComponent(COMP_RENDERABLE));
		r->addToGraphics(y, temp);
		temp.clear();
	}

}

PortalDir Portal::getDirection() {
	return dir;
}

Portal* Portal::getPair() {
	return pair;
}

void Portal::onReset() {
	dir = (PortalDir)-1;
	setDimensions(2, 4);
	pair = NULL;
}

COORD Portal::teleport() {

	

	COORD pairPos = pair->getPosition();
	switch (pair->getDirection()) {
	case PORTAL_LEFT: pairPos.X -= 2;
		break;
	case PORTAL_RIGHT: pairPos.X += 2;
		break;
	case PORTAL_TOP: {pairPos.X += 2; pairPos.Y -= 3;}
		break;
	case PORTAL_BOTTOM: pairPos.X += 2;
		break;
	}


	return pairPos;
}

void Portal::unstuckMe() {


	if (dir == PORTAL_RIGHT) {
		COORD pos = getPosition();
		pos.X --;
		bool allClear = true;
		for (int i = 0; i < (getHeight() - 1); i++) {
			pos.Y += i;
			GameObject *obj = manager->placeMeeting(pos, "Wall");
			if (!obj) {
				allClear = false;
				break;
			}
		}
		if (allClear) {
			pos = getPosition();
			pos.X--;
			setPosition(pos);
		}
	}

	if (dir == PORTAL_LEFT || dir == PORTAL_RIGHT) {
		for (int i = 1; i < getHeight(); i++) {
			COORD pos = getPosition();
			pos.Y += i;
			GameObject *obj = manager->placeMeeting(pos, "Wall");
			while (obj != NULL) {
				pos = getPosition();
				pos.Y--;
				setPosition(pos);
				obj = manager->placeMeeting(pos, "Wall");
			}
		}
	}
}

bool Portal::checkCollision() {

	COORD pos = getPosition();
	bool collision = false;

	switch (dir) {
	case PORTAL_LEFT:
		{

		}
		break;

	case PORTAL_RIGHT:
		{

		}
		break;

	case PORTAL_TOP:
		{
			for (SHORT y = pos.Y - 1; y > pos.Y - 3; y--) {
				for (SHORT x = pos.X; x < pos.X + 2 - 1; x++) {
					if (manager->placeMeeting({ x,y }, "Wall")) {
						collision = true;
						break;
					}
				}
				if (collision) break;
			}
		}
		break;
	case PORTAL_BOTTOM:
	{
		for (SHORT y = pos.Y + getHeight(); y < pos.Y + (SHORT)getHeight() + 3; y++) {
			for (SHORT x = pos.X; x < pos.X + (SHORT)getWidth() - 1; x++) {
				if (manager->placeMeeting({ x,y }, "Wall")) {
					collision = true;
					break;
				}
			}
			if (collision) break;
		}
	}
		break;
	}

	return collision;
}