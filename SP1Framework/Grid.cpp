#include "Grid.h"
#include "GameObject.h"



Grid::Grid()
{
}


Grid::Grid(int width, int height, int cellWidth, int cellHeight) {

	this->width = width;
	this->height = height;
	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;

	// Fallback, Flaw: Prime Number

	while (this->width % this->cellWidth != 0)
		this->cellWidth++;
	while (this->height % this->cellHeight != 0)
		this->cellHeight++;

	cellsX = this->width / this->cellWidth;
	cellsY = this->height / this->cellHeight;
	for (int i = 0; i < cellsX*cellsY; i++) {
		std::vector<GameObject*> current;
		database.push_back(current);
	}
}

Grid::~Grid()
{
	for (int i = 0; i < cellsX*cellsY; i++) {
		std::vector<GameObject*> current = database.at(i);
		current.clear();
	}
	database.clear();
}

void Grid::sort(std::vector<GameObject*>* ref) {
	int cellID = 0;

	//for (int x = 0; x < width; x += cellWidth) {
	//	for (int y = 0; y < height; y += cellHeight) {
	//		for (int i = 0; i < ref->size(); i++) {
	//			GameObject *g = ref->at(i);
	//			COORD start = { (SHORT)x, (SHORT)y };
	//			COORD end = start;
	//			end.X += (SHORT)cellWidth;
	//			end.Y += (SHORT)cellHeight;
	//			if (withinBounds(start, end, g->getPosition()))
	//				addGameObjectToCell(g, cellID);
	//		}
	//		cellID++;
	//	}
	//}
	for (int i = 0; i < (int)ref->size(); i++) {
		GameObject *g = ref->at(i);
		COORD pos = g->getPosition();
		int cellID = getCell(pos);
		addGameObjectToCell(g, cellID);
		if (g->getWidth() >= cellWidth) {
			int xCells = 1;
			int currentX = ((int)pos.X) + cellWidth;
			while (currentX < (pos.X + g->getWidth())) {
				addGameObjectToCell(g, cellID + xCells);
				xCells++;
				currentX += cellWidth;
			}
		}
		cellID = getCell(pos);
		if (g->getHeight() >= cellHeight) {
			int yCells = 1;
			int currentY = ((int)pos.Y) + cellHeight;
			while (currentY < (pos.Y + g->getHeight())) {
				addGameObjectToCell(g, cellID + yCells);
				yCells++;
				currentY += cellHeight;
			}
		}

		/*for (int x = 0; x < width; x += cellWidth) {
		for (int y = 0; y < height; y += cellHeight) {
		for (int i = 0; i < ref->size(); i++) {
		GameObject *g = ref->at(i);
		COORD start = { (SHORT)x, (SHORT)y };
		COORD end = start;
		end.X += (SHORT)cellWidth;
		end.Y += (SHORT)cellHeight;
		if (withinBounds(start, end, g->getPosition()))
		addGameObjectToCell(g, cellID);
		}
		cellID++;
		}
		}*/
		for (int i = 0; i < (int)ref->size(); i++) {
			GameObject *g = ref->at(i);
			addGameObjectToCell(g, getCell(g->getPosition()));

		}
	}

}

std::vector<GameObject*> Grid::getNearbyGameObjectsInCell(int id) {
	std::vector<GameObject*> nearby;
	int cellAbove = id - cellsX;
	int cellBelow = id + cellsX;
	int cellLeft = id - 1;
	int cellRight = id + 1;

	if (cellLeft != 0 && cellLeft % cellsX != 0)
		getGameObjectsInCell(&nearby, cellLeft);
	if (cellRight + 1 % cellsX != 0)
		getGameObjectsInCell(&nearby, cellRight);
	if (cellAbove >= cellsX)
		getGameObjectsInCell(&nearby, cellAbove);
	if (cellBelow < cellsX*cellsY)
		getGameObjectsInCell(&nearby, cellBelow);
	return nearby;
}

bool Grid::withinBounds(COORD start, COORD end, COORD compare) {
	if (compare.X >= start.X && compare.X < end.X
		&& compare.Y >= start.Y && compare.Y < end.Y)
		return true;
	return false;
}

void Grid::addGameObjectToCell(GameObject* gameObj, int id) {

	if ((int)database.size() > id)

		if (database.size() > id)

			database.at(id).push_back(gameObj);
}

void Grid::getGameObjectsInCell(std::vector<GameObject*> *ref, int id) {
	std::vector<GameObject*> current = getGameObjectsInCell(id);

	for (int i = 0; i < (int)current.size(); i++) {
		for (int i = 0; i < current.size(); i++) {

			ref->push_back(current[i]);
		}
	}
}

std::vector<GameObject*> Grid::getGameObjectsInCell(int id) {
	if((int) database.size() > id)
		return database.at(id);
	return database.at(0);
}

int Grid::getCell(COORD pos) {
	int xCell = pos.X / cellWidth;
	int yCell = pos.Y / cellHeight;
	return xCell + yCell * cellsX;
}



int Grid::getCellsX() {
	return this->cellsX;
}

int Grid::getCellsY() {
	return this->cellsY;
}