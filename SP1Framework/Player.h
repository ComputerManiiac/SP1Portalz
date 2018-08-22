#ifndef PLAYER_H
#define PLAYER_H


#include "GameObject.h"
#include "RenderComponent.h"
#include "LogicComponent.h"
#include "Portal.h"
#include "InteractiveCube.h"

class Manager;


class Player : public GameObject
{
public:
	Manager * manager;
	Player();
	Player(Manager *m, COORD position, int width, int height, int horizSpeed, int gravity, int jumpPower);
	~Player();

	SHORT getHorizontalSpeed();
	double getGravity();
	int getJumpPower();

	bool isOnGround();
	void setOnGround(bool ground);

	bool isJumping();
	void setIsJumping(bool jumping);
	double getJumpTime();

	void update();
	void onMove();
	void onShoot();
	void onTeleport(Portal *portal);
	void onShootHit();
	void onGravityPull();

	void draw();

	int velocityX = 0;
	int velocityY = 0;

	int accelerationX = 0;
	int accelerationTP = 0;
	int accelerationY = 0;
	int maxAccelerationY = 4;
	int previousVelocityY = 0;

	std::vector<COORD> line(COORD one, COORD two);
	int sign(int dxy);
	std::vector<COORD>* getShootPoints();
	COORD calculateEndPoint(COORD one, COORD two);
	void optimiseShootPoints();


	void setHorizontalSpeed(SHORT horiz);
	void setJumpPower(int jump);


	PortalDir getPortalDirection(COORD pos);


	// Public for debug purposes
	int numOfPortals = 0;
	Portal* portals[2];
	int portalWidth = 2;
	int portalHeight = 4;
	int currentJumpSteps = 0;
	int targetJumpSteps = 0;
	bool justTeleported = false;
private:



	SHORT horizontalSpeed;
	int gravity;
	bool onGround;
	bool shootAnimation = false;
	int shootPathCurrentBlock = 0;

	bool jumping = false;
	int jumpPower;
	double jumpTime;

	int startPortal = -1;
	COORD coord;

	std::vector<COORD> shootPoints;
	

};


#endif
