#pragma once
#include <Windows.h>

class Player {

public:
	Player();

	void initiatePlayer(int width, int height);

	void moveUp();

	void moveDown();

	void moveRight();

	void moveLeft();

	COORD* getPlayerState();

	void setPlayerState(int X, int Y);


private:

	COORD playerState;
	int m_ScreenSizeX;
	int m_ScreenSizeY;
	
};
