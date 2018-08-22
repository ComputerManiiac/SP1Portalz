#include "Manager.h"
#include "Wall.h"
#include "MovingPlatform.h"
#include "PressurePlate.h"
#include "Portal.h"
#include "InteractiveCube.h"

Manager* Manager::instance = 0;

Manager::Manager()
{

	showDebug = true;

	for (int i = 0; i < COMP_COUNT; i++) {
		std::vector<Component*> comp;
		categorisedComponents.push_back(comp);
	}

	for (int i = 0; i < TIMER_COUNT;i++) {
		timers.push_back(0.0);
	}

	Viewport camera = Viewport();
	camera.X = 0;
	camera.Y = 0;
	camera.X2 = camera.X + cameraWidth;
	camera.Y2 = camera.Y + cameraHeight;


	const char map[11] = " .,:;ox%#@";

	//spawnNewGameObject("Player", { 60,24 }, 2, 3, {"1","1","2"});

	

	

	
	std::ifstream file("leevl.txt");
	std::string line;
	std::vector<std::string> identifier;
	std::vector<std::string> parameters;

	while (std::getline(file, line)) {
		identifier = split(line, '=');
		if (identifier[0] == "GameObject") {
			parameters = split(identifier[1], ',');
			std::string name = parameters[0];
			std::vector<std::string> coords = split(parameters[1], '-');
			COORD c = { (SHORT)stoi(coords[0]), (SHORT)stoi(coords[1]) };
			int width = stoi(parameters[2]);
			int height = stoi(parameters[3]);
			if (height > 0 && width > 0) {
				spawnNewGameObject(name, c, width, height, { "true" });
		
			}
		}
	}
	parameters.clear();
	identifier.clear();
	file.close();
	
	spawnNewGameObject("PressurePlate", { 11,18 },3, 1, { "" });
}


/*
Free up memory allocated by new keyword 
*/

Manager::~Manager()
{

	for (int i = 0; i < (int)gameObjects.size();i++) {
		delete gameObjects[i];
	}

	for (int i = 0; i < (int)categorisedComponents.size(); i++) {
		categorisedComponents.at(i).clear();
	}

	categorisedComponents.clear();
	anims.clear();

	delete instance;
}

std::vector<std::string> Manager::split(const std::string &input, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(input);
	std::string item;
	while (std::getline(ss, item, delim))
		result.push_back(item);
	return result;
}

void Manager::setConsole(Console *cons) {
	console = cons;

	this->grid = new Grid(console->getConsoleSize().X, console->getConsoleSize().Y, 10, 10);
	grid->sort(&gameObjects);

}



Console* Manager::getConsole() {
	return console;

}

Manager* Manager::getInstance()
{
	if (instance == 0) instance = new Manager();
	return instance;
}

Grid* Manager::getGrid() {
	return this->grid;
}



/*
Takes two input colours and interprets it by converting it from decimal to 
hexadecimal format suitable to be stored in WORD for console writing.
*/

WORD Manager::interpretColour(Colour background, Colour foreground) {
	int num = ((int)background) * 16 + ((int)foreground);
	return ((WORD)num);
}

Player* Manager::getPlayer() {
	return player;
}


std::vector<GameObject*>* Manager::getGameObjects() {
	return &(this->gameObjects);
}

// Instantiates new Game Object
GameObject* Manager::spawnNewGameObject(std::string name, COORD position, 
	int width, int height, std::vector<std::string> args) {
	int uid = (int)gameObjects.size();
	GameObject *gameObj = NULL;
	if (name == "Wall") {
		gameObj = new Wall(this, position, width, height, uid, args);
	}else if (name == "Player") {
		/* 
		Argument List

		args[0] - Gravity
		args[1] - Jump Power
		args[2] - Movement Speed
		*/
		//stoi(args[0]), stoi(args[1]), stoi(args[2])
		player = new Player(this, position, 2, 3, 1,1,3);
		gameObj = player;
	} else if (name == "Portal") {
		gameObj = new Portal(this, position, width, height, uid, args);
	}
	else if (name == "MovingPlatform") {
		gameObj = new MovingPlatform(this, position, width, height, uid, args);
	}
	else if (name == "PressurePlate") {
		gameObj = new PressurePlate(this, position, width, height, uid, args);
	}
	else if (name == "InteractiveCube") {
		gameObj = new InteractiveCube(this, position, width, height, uid, args);
	}

	if(gameObj != NULL)
		addToGameObjects(gameObj);
	return gameObj;
}

void Manager::addToGameObjects(GameObject *g) {
	this->gameObjects.push_back(g);
}

int Manager::sign(int num) {
	if (num > 0) return 1;
	if (num < 0) return -1;
	return 0;
}


GameObject* Manager::placeMeeting(COORD coord, std::string type) {

	//std::vector<GameObject*> nearby = grid->getGameObjectsInCell(grid->getCell(coord));
	for (int i = 0; i < (int)gameObjects.size(); i++) {
		GameObject* g = gameObjects.at(i);
		//g->hasComponent(COMP_COLLIDABLE)
		if (g->hasComponent(COMP_COLLIDABLE)) {
			if ((type != "" && g->getName() == type) || type == "") {
				collisionCalls++;
				COORD refPos = g->getPosition();
				SHORT rightX = refPos.X + (SHORT)g->getWidth();
				SHORT rightY = refPos.Y + (SHORT)g->getHeight();
				if (coord.X >= refPos.X && coord.X < rightX) {
					if (coord.Y >= refPos.Y && coord.Y < rightY) {
						return g;
					}
				}
			}

		}
	}
	return NULL;
}

GameObject* Manager::placeMeeting(SHORT x, SHORT y, std::string name) {
	return placeMeeting({ x,y }, "");
}

GameObject* Manager::placeMeetingBound(AABB bounds, std::string type) {
	for (int i = 0; i < (int)gameObjects.size(); i++) {
		collisionCalls++;
		GameObject* g = gameObjects.at(i);
		//g->hasComponent(COMP_COLLIDABLE)
		if (g->hasComponent(COMP_COLLIDABLE)) {
			if ((type != "" && g->getName() == type) || type == "") {
				if (placeOverlap(bounds, g->getAABB()))
					return g;		
			}

		}
	}
	return NULL;
}

bool Manager::placeOverlap(AABB a, AABB b) {
	if (a.P2.X < b.P1.X || a.P1.X > b.P2.X || a.P2.Y < b.P1.Y || a.P1.Y > b.P2.Y)
		return false;
	else
		return true;
}

//COORD Manager::moveSmoothly(COORD start, COORD end, )
SHORT Manager::distanceTillCollide(AABB start, AABB end) {
	if (placeMeetingBound(end, "")) return end.P1.X - start.P1.X;

	SHORT distance = 0;
	SHORT dir = sign(end.P1.X - start.P1.X);
	//int widthOffset = (dir == 1) ? objWidth : 0;

	AABB bound = start;
	bound.P1.X += dir;
	bound.P2.X += dir;

	while (!placeMeetingBound(bound, "")) {
		distance++;
		bound.P1.X += dir;
		bound.P2.X += dir;
	}
	return distance;

}

COORD Manager::calculateDiff(COORD start, COORD end) {
	SHORT changeX = end.X - start.X;
	SHORT changeY = end.Y - start.Y;
	return { changeX, changeY };
}

void Manager::addComponentToCategory(Component *c, int id) {
	categorisedComponents[id].push_back(c);
}

void Manager::removeComponentFromCategory(Component* c, int id) {
	std::vector<Component*>* current = &(categorisedComponents[id]);
	current->erase(std::remove(current->begin(), current->end(), c), current->end());
}

std::vector<Component*> Manager::getCategory(int id) {
	return this->categorisedComponents[id];
}

void Manager::updateKeyPress() {
	g_abKeyPressed[K_UP] = isKeyPressed(VK_UP);
	g_abKeyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	g_abKeyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_W] = isKeyPressed(VK_W);
	g_abKeyPressed[K_A] = isKeyPressed(VK_A);
	g_abKeyPressed[K_S] = isKeyPressed(VK_S);
	g_abKeyPressed[K_D] = isKeyPressed(VK_D);
	g_abKeyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
	g_abKeyPressed[K_LBUTTON] = isKeyPressed(VK_LBUTTON);
	g_abKeyPressed[K_TILDE] = isKeyPressed(VK_OEM_3);
}

bool Manager::checkKeyPressed(EKEYS key) {
	return g_abKeyPressed[key];
}



void Manager::callAfterSeconds(pFunction func, double delay) {
	int id = (int)doLaterTimings.size();
	doLaterTimings[id] = delay;
	doLaterFunctions[id] = func;
}

void Manager::updateLater(double deltaTime) {
	std::vector<int> toErase;
	for (auto const& i : doLaterTimings)
	{
		int key = i.first; 
		double value = i.second;
		value -= deltaTime;
		doLaterTimings[key] = value;
		if (value <= 0.0) {
			pFunction func = (pFunction) doLaterFunctions[key];
			func();
			toErase.push_back(key);
		}
			//int result3 = (instance1.*pt2Member)(12, 'a', 'b');
	}
	if (toErase.size() > 0) {
		for (int i = 0; i < (int)toErase.size(); i++) {
			int id = toErase[i];
			doLaterTimings.erase(id);
			doLaterFunctions.erase(id);
		}
		toErase.clear();
	}
}



void Manager::setTimer(Timer t, double time) {
	timers[t] = time;
}

void Manager::updateTimers(double deltaTime) {
	for (int i = 0; i < TIMER_COUNT; i++) {

		if (i == TIMER_BOUNCE || i == TIMER_SHOOT 
			|| i == TIMER_COLLISION) continue;

		if (i == TIMER_DELTA) {
			timers[i] = deltaTime;
		}
		else {
			timers[i] += deltaTime;
		}
	}
}

double Manager::getTimer(Timer t) {
	return timers[t];
}

void Manager::printScaledImage(const char *path, const char map[11], int subGroupWidth, int subGroupHeight, int maxWidth) {
	std::vector<int> bitmapValues;
	std::vector<std::string> mapped;

	CImg<unsigned char> src(path);
	int width = src.width();
	int height = src.height();
	int totalBrightness = 0;


	// Calculate brightness values for each pixel
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int r = (int)src(x, y, 0, 0);
			int g = (int)src(x, y, 0, 1);
			int b = (int)src(x, y, 0, 2);
			int brightnessPerPixel = (r + g + b) / 3;
			bitmapValues.push_back(brightnessPerPixel);
		}
	}

	// Find a suitable sub group width and height
	while (width % subGroupWidth != 0) width--;
	if (height % subGroupHeight != 0) height--;

	// Calculate number of sub groups both horizontally and vertically
	int subGroupsH = width / subGroupWidth;
	int subGroupsV = height / subGroupHeight;

	while (subGroupsH > maxWidth) {
		subGroupWidth++;
		subGroupHeight = subGroupWidth * 2;
		subGroupsH = width / subGroupWidth;
		subGroupsV = height / subGroupHeight;
	}

	for (int y = 0; y < height; y += subGroupHeight) {
		std::string currentLine = "";
		for (int x = 0; x < width; x += subGroupWidth) {
			int subGroupBrightness = 0;
			for (int sX = 0; sX < subGroupWidth; sX++) {
				for (int sY = 0; sY < subGroupHeight; sY++) {
					int posInVector = y * src.width() + x;
					subGroupBrightness += bitmapValues[posInVector];
				}
			}
			int averageSubGroupBrightness = subGroupBrightness / (subGroupWidth*subGroupHeight);

			double ratio = 10 / 256.0;
			int v = (int)(averageSubGroupBrightness * ratio);
			char t = map[v];
			currentLine.push_back(t);
		}
		mapped.push_back(currentLine);
	}

	anims.push_back(mapped);
	mapped.clear();
	bitmapValues.clear();
}
