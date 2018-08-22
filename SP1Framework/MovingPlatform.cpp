#include "MovingPlatform.h"



MovingPlatform::MovingPlatform()
{
}

MovingPlatform::MovingPlatform(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args)
	: GameObject("MovingPlatform", position, width, height, uid, args){

	RenderComponent *r = new RenderComponent(this);
	addComponent(r, COMP_RENDERABLE);
	m->addComponentToCategory(r, COMP_RENDERABLE);

	PatrolComponent *p = new PatrolComponent(this);
	addComponent(p, COMP_PATROL);
	m->addComponentToCategory(p, COMP_PATROL);

	CollidableComponent *c = new CollidableComponent(this);
	addComponent(c, COMP_COLLIDABLE);
	m->addComponentToCategory(c, COMP_COLLIDABLE);

	std::vector<Graphics> temp;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Graphics g = { ' ', 0 };
			g.colour = (WORD)(RED * 16 + WHITE);

			temp.push_back(g);
		}
		r->addToGraphics(y, temp);
		temp.clear();
	}
}

MovingPlatform::~MovingPlatform()
{

}



