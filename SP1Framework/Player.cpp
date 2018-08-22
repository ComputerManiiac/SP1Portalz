#include "Player.h"
#include "Manager.h"



Player::Player()
{
}

Player::Player(Manager *m, COORD position, int width, int height, int horizSpeed, int gravity, int jumpPower)
	: GameObject("Player", position, width, height, -1, { "" }) {

	/* Initialise Player's Component
	-> Render: Called every render()
	-> 

	*/
	RenderComponent *r = new RenderComponent(this);
	addComponent(r, COMP_RENDERABLE);
	m->addComponentToCategory(r, COMP_RENDERABLE);

	LogicComponent *l = new LogicComponent(this);
	addComponent(l, COMP_LOGIC);
	m->addComponentToCategory(l, COMP_LOGIC);

	// Initialise Player Variables
	this->horizontalSpeed = horizSpeed;
	this->gravity = gravity;
	this->jumpPower = jumpPower;
	startPortal = -1;

	// Populate Player's Graphics [TEMP]
	std::vector<Graphics> temp;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Graphics g = { ' ', 0 };
			g.colour = manager->interpretColour(BLUE, WHITE);

			temp.push_back(g);
		}
		r->addToGraphics(y, temp);
		temp.clear();
	}
}


Player::~Player()
{
}


SHORT Player::getHorizontalSpeed() {
	return this->horizontalSpeed;
}


double Player::getGravity() {
	return this->gravity;
}

int Player::getJumpPower() {
	return this->jumpPower;
}

bool Player::isOnGround() {
	return this->onGround;
}

void Player::setOnGround(bool ground) {
	this->onGround = ground;
}
bool Player::isJumping() {
	return this->jumping;
}

void Player::setIsJumping(bool jumping) {
	this->jumping = jumping;
}

double Player::getJumpTime() {
	return this->jumpTime;
}


void Player::setHorizontalSpeed(SHORT horiz) {
	this->horizontalSpeed = horiz;
}

void Player::setJumpPower(int jump) {
	this->jumpPower = jump;
}


/*

UPDATE EVENT
-> This event is called every update() loop in game.cpp

Player's Update Event includes:
-> Moving in each direction
-> Gravity Pull

*/

void Player::update() {
	manager = Manager::getInstance();



	onMove();
	onShoot();

}

std::vector<COORD>* Player::getShootPoints() {
	return &(this->shootPoints);
}

void Player::onShoot() {
	if (manager->getTimer(TIMER_SHOOT) > manager->getTimer(TIMER_ELAPSED)) return;
	if (GetConsoleWindow() != GetForegroundWindow() || manager->getConsole()->getMouseXY().Y < 0) return;

	if (manager->checkKeyPressed(K_LBUTTON)) {

		shootPoints.clear();

		COORD mouse = manager->getConsole()->getMouseXY();
		mouse.X += Manager::getInstance()->camera.X;
		COORD player = getPosition();

	
		shootPoints = line(player, calculateEndPoint(player,mouse));
		COORD portal;
		int id = -1;
		for (int i = 0; i < (int) shootPoints.size(); i++) {

			GameObject *w = manager->placeMeeting(shootPoints[i], "");
			GameObject *hitPortal = manager->placeMeeting(shootPoints[i], "Portal");
			if (w != NULL && hitPortal == NULL) {
				portal = shootPoints[i-1];
				id = i;
				break;
			}
		}
		if (id != -1) {
			shootPoints.erase(shootPoints.begin() + id, shootPoints.end());
			optimiseShootPoints();
			shootPathCurrentBlock = shootPoints.size()-1;
			shootAnimation = true;
			numOfPortals++;
			onShootHit();

		}
		manager->setTimer(TIMER_SHOOT, manager->getTimer(TIMER_ELAPSED) + 0.75);
	}


}

void Player::onShootHit() {
	if (!shootAnimation) return;

	if (shootPathCurrentBlock < (int)(shootPoints.size() - 1)) {
		shootPathCurrentBlock++;
	}
	else {
		COORD spawnPosition = shootPoints[shootPathCurrentBlock];
		GameObject *obj = manager->spawnNewGameObject("Portal", spawnPosition, 2, 4, {""});
		Portal* portal = dynamic_cast<Portal*>(obj);
		portal->setDirection(getPortalDirection(spawnPosition));
		//portal->unstuckMe();

		if (numOfPortals == 3) {
			numOfPortals = 2;

			int endPortal = 1 - startPortal;

			portals[startPortal]->destroy();
			portals[startPortal] = portal;

			portals[startPortal]->setPair(portals[endPortal]);
			portals[endPortal]->setPair(portals[startPortal]);

			startPortal = endPortal;

		}
		else {

			if (numOfPortals == 2) {
				portals[0]->setPair(portal);
				portal->setPair(portals[0]);
				portals[1] = portal;
				startPortal = 0;
			}
			portals[numOfPortals - 1] = portal;
		}

		shootAnimation = false;
	}
}


void carryMomentum() {
	Player *p = Manager::getInstance()->getPlayer();
	p->velocityY -= p->accelerationY;
	p->accelerationY = 0;
}

void Player::onTeleport(Portal *portal) {
	if (!portal->getPair()->checkCollision()) {
		coord = portal->teleport();
		PortalDir in = portal->getDirection();
		PortalDir out = portal->getPair()->getDirection();

		//// VERTICAL MOMENTUM COMBO
		if (in == PORTAL_TOP && out == PORTAL_TOP) {
			accelerationY = -previousVelocityY;		
		}
		justTeleported = true;
	}
}

void resetPlayerSpeed() {
	Manager::getInstance()->getPlayer()->setHorizontalSpeed(1);
	Manager::getInstance()->getPlayer()->accelerationY=0;
}


PortalDir Player::getPortalDirection(COORD pos) {

	if (manager->placeMeeting({ pos.X + (SHORT)portalWidth, pos.Y }, "Wall")) {
		return PORTAL_LEFT;
	}
	else if (manager->placeMeeting({ pos.X - 1, pos.Y }, "Wall")) {
		return PORTAL_RIGHT;
	}
	else if (manager->placeMeeting({ pos.X, pos.Y - 1 }, "Wall")) {
		return PORTAL_BOTTOM;
	}
	else if (manager->placeMeeting({ pos.X, pos.Y + (SHORT)portalHeight }, "Wall")) {
		return PORTAL_TOP;
	}
	return PORTAL_TOP;
}

void resetTeleport() {
	Manager::getInstance()->getPlayer()->justTeleported = false;
}



void Player::onMove() {

	// Reset velocity
	velocityX = 0;
	velocityY = 0;

	coord = getPosition();
	AABB* bound = &getAABB();

	// Update onGround()
	bound->P2.Y++;
	bound->P1.Y = bound->P2.Y;
	GameObject* ground = manager->placeMeetingBound(*bound, "");
	setOnGround(ground != NULL);

	// Gravity and Jumping
	if (!isOnGround()) {
		accelerationY = 1;

		manager->callAfterSeconds(resetTeleport, 1.0);

	}
	else {
		// Collision with Portal below the Player
		if (ground != NULL) {
			if (ground->getName() == "Portal" && numOfPortals == 2) {
				onTeleport(dynamic_cast<Portal*>(ground));
			}
			else if (ground->getName() != "Portal") {



				accelerationY = 0;
				if (velocityY == 0) {
					int jump = (manager->checkKeyPressed(K_W) || manager->checkKeyPressed(K_UP)) * jumpPower;
					velocityY += -jump;
				}
			}
			
		}
	}


	velocityY += accelerationY;

	bound = &getAABB();
	bound->P1.Y += velocityY;
	bound->P2.Y += velocityY;

	int correctedVelocityY = 0;
	GameObject *objBelow = manager->placeMeetingBound(*bound, "");
	if (objBelow) {

		bound = &getAABB();
		SHORT dir = sign(velocityY);
		bound->P1.Y += dir;
		bound->P2.Y += dir;
		while (!manager->placeMeetingBound(*bound, "")) {
			bound->P1.Y += dir;
			bound->P2.Y += dir;
			correctedVelocityY += dir;
		}
		velocityY = correctedVelocityY;
	}


	// Update Y coordinate
	coord.Y += velocityY;
	setPosition(coord);

	bound = &getAABB();

	// Update Horizontal Movement
	int left = -(manager->checkKeyPressed(K_A) || manager->checkKeyPressed(K_LEFT));
	int right = manager->checkKeyPressed(K_D) || manager->checkKeyPressed(K_RIGHT);
	int direction = right+left;

	//if (direction == 0) {
	//	if (justTeleported && (portals[0]->getDirection() != portals[1]->getDirection()))
	//		justTeleported = false;
	//}


	int shouldReflectMovement =  (justTeleported && (portals[0]->getDirection() % 3 == 0) && (portals[1]->getDirection() % 3 == 0)
		&& portals[0]->getDirection() == portals[1]->getDirection()) ? -1 : 1;

	velocityX = direction *  (int) horizontalSpeed * shouldReflectMovement;
	

	if (velocityX != 0) {
		int widthOffset = (velocityX > 0) ? getWidth() : 0;
		bound->P1.X = coord.X + (SHORT)velocityX;
		bound->P2.X = bound->P1.X + getWidth() - 1;

		// Checks for collision on the left/right of the Player with ALL type of objects
		GameObject* object = manager->placeMeetingBound(*bound, "");
		if (object != NULL) {

			// Collision with Portal
			if (object->getName() == "Portal") {
				if (numOfPortals == 2) {
					Portal* portal = dynamic_cast<Portal*>(object);
					// PORTAL_RIGHT = 3, PORTAL_LEFT = 6
					if (portal->getDirection() % 3 == 0)
						onTeleport(portal);
				}
			} else{

				if (object->getName() == "InteractiveCube") {
					AABB cubeBounds = object->getAABB();
					InteractiveCube *cube = dynamic_cast<InteractiveCube*>(object);
					coord.X += cube->push(velocityX);
				}

				/*bound->P1.X = coord.X + (SHORT)sign(velocityX) + widthOffset;
				bound->P2.X = bound->P1.X + widthOffset;
				while (!manager->placeMeetingBound(*bound, "")) {
					bound->P1.X += (SHORT)sign(velocityX);
					bound->P2.X = bound->P1.X + getWidth() - 1;
					velocityX += (SHORT)sign(velocityX);
				}*/
			}
			velocityX = 0;
			
			
		}
	}

	// Update X coordinate
	coord.X += velocityX;
	setPosition(coord);

	



}


void Player::optimiseShootPoints() {
	int size = (int) shootPoints.size();
	std::vector<COORD> optimised;

	int increment = size / 4;
	if (increment == 0) increment = 1;
	for (int i = 0; i < size; i+= increment) {
		optimised.push_back(shootPoints[i]);
	}
	COORD final;
	COORD lastPointLeft = shootPoints[size - 1];
	COORD lastPointRight = lastPointLeft;
	lastPointRight.X++;
	if (manager->placeMeeting(lastPointRight, "Wall")) {
		lastPointLeft.X--;
		final = lastPointLeft;
	} else if (manager->placeMeeting(lastPointLeft, "Wall")) {
		lastPointLeft.X++;
		final = lastPointLeft;
	}
	optimised.push_back(lastPointLeft);
	shootPoints = optimised;
}


int Player::sign(int dxy)
{
	if (dxy<0) return -1;
	else if (dxy>0) return 1;
	else return 0;
}

COORD Player::calculateEndPoint(COORD one, COORD two) {

	int x1 = (int)one.X;
	int y1 = (int)one.Y;
	int x2 = (int)two.X;
	int y2 = (int)two.Y;


	if (x1 == x2 && abs(y2-y1) > 0)
		return two;

	int x3 = (int) Manager::getInstance()->camera.X2;
	int lowerX = Manager::getInstance()->camera.X;

	if (x2 - x1 < 0) x3 = lowerX;

	double gradient = ((double)(y2 - y1)) / ((double)(x2 - x1));
	double yintercept = (y1 - gradient * x1);
	int y3 = (int)((gradient * x3) + yintercept);
	return{ (SHORT)x3,(SHORT)y3 };
}

std::vector<COORD> Player::line(COORD one, COORD two) {

	std::vector<COORD> list;
	int x1 = (int)one.X;
	int y1 = (int)one.Y;
	int x2 = (int)two.X;
	int y2 = (int)two.Y;

	int diffY = abs(y2 - y1);



	int dX = x2 - x1;
	int dY = y2 - y1;

	//# Increments
	int sX = sign(dX);
	int sY = sign(dY);

	//# Segment length
	dX = abs(dX);
	dY = abs(dY);
	int maxD = max(dX, dY);

	//# Initial remainder
	double r = maxD / 2;

	int x = x1;
	int y = y1;

	if (dX > dY){
		//# Main loop
		for (int i = 0; i< maxD; i++) {
			Viewport v = Manager::getInstance()->camera;
			if (x >=  v.X  && x <= v.X2 && y >= 0 && y <= 50)
				list.push_back({ (SHORT)x, (SHORT)y });
			//# Update (X, Y) and R
			x += sX;
			r += dY; // Horizontal Movement
			if (r >= dX)
			{
				y += sY;
				r -= dX; // Diagonal Movement
			}
		}
	} else {
		for (int i = 0; i< maxD; i++) {
			Viewport v = Manager::getInstance()->camera;
			if (x >= v.X  && x <= v.X2 && y >= 0 && y <= 50)
				list.push_back({ (SHORT)x, (SHORT)y });
			//# Update (X, Y) and R
			y += sY;
			r += dX; // Horizontal Movement
			if (r >= dY)
			{
				x += sX;
				r -= dY; // Diagonal Movement
			}
		}
	}
	return list;
}