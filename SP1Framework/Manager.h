#ifndef MANAGER_H
#define MANAGER_H


#include "Framework/console.h"
#include "../SP1Framework/CImg-2.3.4_pre072618/CImg.h"
#include "Player.h"
#include "Component.h"
#include "PatrolComponent.h"
#include "LogicComponent.h"
#include "PressurePlate.h"


#include "Grid.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

using namespace cimg_library;

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A


enum Colour {
	BLACK, BLUE, GREEN, CYAN,
	RED, MAGENTA, BROWN, DIMWHITE,
	GRAY, BRIGHT_BLUE, BRIGHT_GREEN, BRIGHT_CYAN,
	BRIGHT_RED, BRIGHT_MAGENTA, BRIGHT_YELLOW, WHITE
};

enum Timer {
	TIMER_ELAPSED,
	TIMER_DELTA,
	TIMER_BOUNCE,
	TIMER_GRAVITY,
	TIMER_SHOOT,
	TIMER_JUMP,
	TIMER_COLLISION,
	TIMER_SHOOT_ANIM,
	TIMER_MOMENTUM,
	TIMER_COUNT
};



// Enumeration to store the control keys that your game will have
enum EKEYS
{
	K_UP, K_LEFT, K_RIGHT,
	K_ESCAPE, K_W, K_A, K_S,
	K_D, K_SPACE, K_LBUTTON,
	K_TILDE,
	K_COUNT
};

struct Viewport {
	SHORT X;
	SHORT X2;
	SHORT Y;
	SHORT Y2;
};

class Wall;
class MovingPlatform;
class Portal;
class PressurePlate;
class InteractiveCube;

typedef void(*pFunction)(void);

class Manager
{
public:
	Manager();
	~Manager();
	static Manager* getInstance();


	// Forward declaration of class references
	Component *component;
	Wall *wall;
	MovingPlatform *movingPlatform;
	Portal *portal;
	PressurePlate *pressurePlate;
	InteractiveCube *interactiveCube;

	Player* getPlayer();

	void setConsole(Console *console);
	Console *getConsole();

	void printScaledImage(const char *path, const char map[11], int subGroupWidth = 2, int subGroupHeight = 4, int maxWidth = 120);
	std::vector<std::vector<std::string>> anims;

	int sign(int num);

	std::vector<GameObject*>* getGameObjects();
	void addToGameObjects(GameObject *g);
	GameObject* spawnNewGameObject(std::string name, COORD position, int width, int height, std::vector<std::string> args);
	GameObject* placeMeeting(COORD coord, std::string name);
	GameObject* placeMeeting(SHORT x, SHORT y, std::string name);
	GameObject* placeMeetingBound(AABB bounds, std::string type);
	bool placeOverlap(AABB a, AABB b);
	COORD calculateDiff(COORD start, COORD end);
	SHORT distanceTillCollide(AABB start, AABB end);

	std::vector<Component*> getCategory(int id);
	void addComponentToCategory(Component* c, int id);
	void removeComponentFromCategory(Component* c, int id);

	WORD interpretColour(Colour background, Colour foreground);


	std::vector<std::string> split(const std::string &input, char delim);

	void updateKeyPress();
	bool checkKeyPressed(EKEYS key);

	void updateTimers(double deltaTime);
	void setTimer(Timer t, double time);
	double getTimer(Timer t);
	Grid* getGrid();

	Viewport camera;
	int cameraWidth = 120;
	int cameraHeight = 50;
	
	int collisionCalls = 0;

	void callAfterSeconds(pFunction func, double delay);
	void updateLater(double deltaTime);

	
	bool showDebug;

private:
	static Manager* instance;
	Console *console;
	Player *player;



	std::vector<GameObject*> gameObjects;
	std::vector<double> timers;
	std::vector<std::vector<Component*>> categorisedComponents;
	std::map<int, double> doLaterTimings;
	std::map<int, void(*)> doLaterFunctions;
	bool g_abKeyPressed[K_COUNT];
	

	Grid *grid;
};

#endif