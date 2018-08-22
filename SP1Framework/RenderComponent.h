#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"


// To be removed - used for random
#include <stdlib.h> 



class Manager;

struct Graphics {
	char text;
	WORD colour;
};


class RenderComponent : public Component
{
public:
	RenderComponent();
	RenderComponent(GameObject *parent);
	Manager *manager;
	virtual void draw();
	virtual ~RenderComponent();
	std::vector<std::vector<Graphics>> getGraphics();
	void addToGraphics(int y, std::vector<Graphics> g);
private:
	std::vector<std::vector<Graphics>> graphics;
	int colour;




};


#endif

