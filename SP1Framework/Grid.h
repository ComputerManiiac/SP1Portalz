#ifndef GRID_H
#define GRID_H


#include <vector>
#include <Windows.h>

class GameObject;


class Grid
{
public:
	Grid();
	Grid(int width, int height, int cellWidth, int cellHeight);
	~Grid();
	std::vector<GameObject*> getGameObjectsInCell(int id);
	void addGameObjectToCell(GameObject* gameObj, int id);
	void removeGameObjectFromCell(int uid, int id);
	void sort(std::vector<GameObject*>* ref);
	bool withinBounds(COORD start, COORD end, COORD compare);
	int getCell(COORD pos);
	int getCellsY();
	int getCellsX();
	std::vector<GameObject*> getNearbyGameObjectsInCell(int id);
	void getGameObjectsInCell(std::vector<GameObject*> *ref, int id);
private:

	// Forward Declare References
	GameObject *gameObject;

	// Initialised by Constructor
	int width;
	int height;
	int cellWidth;
	int cellHeight;

	// Others
	int cellsX;
	int cellsY;

	std::vector<std::vector<GameObject*>> database;
};

#endif