#include "Wall.h"
#include "Manager.h"


Wall::Wall()
{
}

Wall::Wall(Manager *m, COORD position, int width, int height, int uid, std::vector<std::string> args) 
	: GameObject("Wall", position, width, height, uid, args){

	RenderComponent *r = new RenderComponent(this); 
	addComponent(r, COMP_RENDERABLE);
	m->addComponentToCategory(r, COMP_RENDERABLE);

	CollidableComponent *c = new CollidableComponent(this);
	addComponent(c, COMP_COLLIDABLE);
	m->addComponentToCategory(c, COMP_COLLIDABLE);


	

	this->canPortal = (args[0] == "true") ? true : false;
	
	std::vector<Graphics> temp;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			
			Graphics g = { '#', 0 };
			g.colour = m->interpretColour(BLACK, WHITE);
		
			temp.push_back(g);
		}
		r->addToGraphics(y, temp);
		temp.clear();
	}
}

Wall::~Wall(){}

