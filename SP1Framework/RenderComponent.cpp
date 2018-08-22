#include "RenderComponent.h"
#include "Manager.h"


RenderComponent::RenderComponent(){}
RenderComponent::~RenderComponent(){}

RenderComponent::RenderComponent(GameObject *parent)
	:Component(parent, COMP_RENDERABLE){
	colour = ((rand() % 16) + 1) * 16 + 2;
}



void RenderComponent::draw() {

	std::vector<std::vector<Graphics>> g = getGraphics();

	
	for (int y = 0; y < (int)g.size(); y++) {
		std::vector<Graphics> inner = g.at(y);
		for (int x = 0; x < (int)inner.size(); x++) {
			Graphics graphic = g.at(y).at(x);
			COORD pos = getParent()->getPosition();
			pos.X += x;
			pos.X -= Manager::getInstance()->camera.X;
			pos.Y += y;
			Manager::getInstance()->getConsole()->writeToBuffer(pos, graphic.text, graphic.colour);
		}
	}
	
	
	

	/*
	GameObject *g = getOwner();
	COORD pos = g->getPosition();
	Manager *m = Manager::getInstance();
	std::vector<std::vector<Graphic>> *data = g->getGraphics();
	for (SHORT y = 0; y < (SHORT)data->size(); y++) {
		std::vector<Graphic>* currentLine = &(data->at((int)y));
		for (SHORT x = 0; x < (SHORT)currentLine->size(); x++) {
			Graphic g = currentLine->at((int)x);
			m->getConsole()->writeToBuffer({ pos.X + x, pos.Y + y }, g.text, g.colour);
		}
	}*/
	/*
	for(int cY = 0; )

	for (SHORT cY = pos.Y; cY < pos.Y + (SHORT)g->getHeight(); cY++) {
		for (SHORT cX = pos.X; cX < pos.X + (SHORT)g->getWidth(); cX++) {
			g_Console.writeToBuffer({ cX,cY }, ' ', manager->interpretColour(RED, WHITE));
		}
	}*/
}

std::vector<std::vector<Graphics>> RenderComponent::getGraphics() {
	return this->graphics;
}

void RenderComponent::addToGraphics(int y, std::vector<Graphics> g) {
	this->graphics.push_back(g);
}