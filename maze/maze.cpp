
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

#include "Player.h"
#include "Node.h"
#include "Prm.h"

#define MOVE_UP  0x0077
#define MOVE_DOWN 0x0073
#define MOVE_LEFT 0x0061
#define MOVE_RIGHT 0x0064

class Game {

public:
	Game(int width, int heigth, int fontWidth, int fontHeight) {
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		m_ScreenWidth = width;
		m_ScreenHeight = heigth;

		COORD consoleSize = {(short) m_ScreenWidth, (short) m_ScreenHeight};

		SetConsoleScreenBufferSize(hOut, consoleSize);

		//Sets the screenBuffer to active
		if (!SetConsoleActiveScreenBuffer(hOut))
			abort();

		//Check if the windowSize is bigger than the maximal buffer size
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hOut, &csbi))
			abort();

		if (m_ScreenWidth > csbi.dwMaximumWindowSize.X){
			std::cout << "To big windows width" << std::endl;
			abort();
		}
		if (m_ScreenHeight > csbi.dwMaximumWindowSize.Y) {
			std::cout << "To big windows heigth" << std::endl;
			abort();
		}

		m_rectWidow = {0, 0, (short)m_ScreenWidth - 1, (short)m_ScreenHeight - 1 };
		if (!SetConsoleWindowInfo(hOut, true, &m_rectWidow))
			abort();

		// Remotion of the blinking cursor

		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hOut, &cci);

		cci.bVisible = false;

		SetConsoleCursorInfo(hOut, &cci);

		//Setting the fon size
		CONSOLE_FONT_INFOEX cfi;

		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontWidth;
		cfi.dwFontSize.Y = fontHeight;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		//Creation of a fitticious buffer

		m_ScreenBuffer = new CHAR_INFO[m_ScreenWidth * m_ScreenHeight];
		memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_ScreenHeight * m_ScreenWidth);

		// Input console

		hIn = GetStdHandle(STD_INPUT_HANDLE);

		if (hIn == INVALID_HANDLE_VALUE)
			abort();

		DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;

		if (!SetConsoleMode(hIn, fdwMode))
			abort();

	}

	void initializeGame() {
		
		player.initiatePlayer(m_ScreenWidth, m_ScreenHeight);

		COORD* playerState = player.getPlayerState();

		playerState->X = (int) m_ScreenWidth / 2;
		playerState->Y = (int) m_ScreenHeight / 2;

		// Setting player at the initial position

		m_ScreenBuffer[m_ScreenWidth * playerState->Y + playerState->X].Char.UnicodeChar = L'#';
		m_ScreenBuffer[m_ScreenWidth * playerState->Y + playerState->X].Attributes = 0x000F;

		if(!WriteConsoleOutput(hOut, m_ScreenBuffer, { (short)m_ScreenWidth, (short)m_ScreenHeight }, { 0, 0 }, &m_rectWidow))
			abort();

		buildLevel();

	}


	void gameLoop() {

		COORD finalTempPosition;
		COORD* playerPosition;

		Node startNode;
		Node goalNode;

		Prm prmSolver;

		int deltaX;
		int deltaY;

		int maxReps = 100;
		bool pathFound = false;


		while (true) {

			//Gets how many events are happening

			//readCommand();

			//executeCommand();

			if (foodEaten)
				createFood();


			playerPosition = player.getPlayerState();

			// Updating the buffer (after flushing it)
			memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_ScreenHeight * m_ScreenWidth);

			if (!pathFound) {

				startNode.initialize(playerPosition->X, playerPosition->Y);
				goalNode.initialize(foodPositionX, foodPositionY);

				prmSolver.initialize(startNode, goalNode, maxReps, m_ScreenWidth, m_ScreenHeight);

				pathFound = prmSolver.findPath();

				actualNodePosition = prmSolver.getStartPosition();

			}

			// Update the node we are at
			if (pathFound) {

				actualNodePosition = actualNodePosition->getNext();
				finalTempPosition = actualNodePosition->getPosition();

				deltaX = finalTempPosition.X - playerPosition->X;
				deltaY = finalTempPosition.Y - playerPosition->Y;


				// Displacement along X
				for (int j = 0; j < abs(deltaX); j++) {

					m_ScreenBuffer[m_ScreenWidth * playerPosition->Y + playerPosition->X].Char.UnicodeChar = L'#';
					m_ScreenBuffer[m_ScreenWidth * playerPosition->Y + playerPosition->X].Attributes = 0x000F;

					if (deltaX > 0)
						playerPosition->X++;
					else
						playerPosition->X--;

					// Printing on screen

					m_ScreenBuffer[m_ScreenWidth  * foodPositionY + foodPositionX].Char.UnicodeChar = L'*';
					m_ScreenBuffer[m_ScreenWidth  * foodPositionY + foodPositionX].Attributes = 0x000F;

					if (!WriteConsoleOutput(hOut, m_ScreenBuffer, { (short)m_ScreenWidth, (short)m_ScreenHeight }, { 0, 0 }, &m_rectWidow))
						abort();

					memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_ScreenHeight * m_ScreenWidth);

					Sleep(250);
				}

				//Displacement along Y
				for (int i = 0; i < abs(deltaY); i++) {

					m_ScreenBuffer[m_ScreenWidth * playerPosition->Y + playerPosition->X].Char.UnicodeChar = L'#';
					m_ScreenBuffer[m_ScreenWidth * playerPosition->Y + playerPosition->X].Attributes = 0x000F;

					if (deltaY > 0)
						playerPosition->Y++;
					else
						playerPosition->Y--;

					m_ScreenBuffer[m_ScreenWidth  * foodPositionY + foodPositionX].Char.UnicodeChar = L'*';
					m_ScreenBuffer[m_ScreenWidth  * foodPositionY + foodPositionX].Attributes = 0x000F;

					// Printing on screen
					if (!WriteConsoleOutput(hOut, m_ScreenBuffer, { (short)m_ScreenWidth, (short)m_ScreenHeight }, { 0, 0 }, &m_rectWidow))
						abort();

					memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_ScreenHeight * m_ScreenWidth);
					Sleep(250);
				}

				if (playerPosition->X == foodPositionX && playerPosition->Y == foodPositionY) {
					foodEaten = true;
					pathFound = false;
				}
			}
		}

	}
	
	void cleanConsoleScreen(){

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		COORD topLeft = { 0, 0 };

		std::cout.flush();

		// Function that fills the variable csbi. If fails aborts :(
		if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {

			abort();
		}

		DWORD consoleLength = csbi.dwSize.X * csbi.dwSize.Y;

		DWORD written;

		FillConsoleOutputCharacter(hOut, TEXT(' '), consoleLength, topLeft, &written);

		FillConsoleOutputAttribute(hOut, csbi.wAttributes, consoleLength, topLeft, &written);

		SetConsoleCursorPosition(hOut, topLeft);
	}

	void setCursorPosition(int x, int y) {

		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hOut == INVALID_HANDLE_VALUE)
			abort();

		COORD cordinates = { x, y };

		SetConsoleCursorPosition(hOut, cordinates);
	}

	int screenWidth() {
		return m_ScreenWidth;
	}

	int screenHeight() {
		return m_ScreenHeight;
	}

	void createFood() {
		srand(time(NULL));

		//int foodPositionX = rand() % m_ScreenWidth;
		foodPositionX = rand() % m_ScreenWidth;
		foodPositionY = rand() % m_ScreenHeight;

		foodEaten = false;

		return; 
	}

	void readCommand() {

		DWORD nEvents;
		GetNumberOfConsoleInputEvents(hIn, &nEvents);

		if(!ReadConsoleInput(hIn, commandBuffer, nEvents, &cNumRead))
			abort();

	}

	void executeCommand() {

		WCHAR unicodeCommand;
		INPUT_RECORD command;
		BOOL buttonPressed;

		for (size_t i = 0; i < cNumRead; i++) {
			command = commandBuffer[i];
			buttonPressed = command.Event.KeyEvent.bKeyDown;

			if (command.EventType == KEY_EVENT && buttonPressed) {

				unicodeCommand = command.Event.KeyEvent.uChar.UnicodeChar;

				if (unicodeCommand == MOVE_UP && buttonPressed) {
					player.moveUp();
				}

				else if (unicodeCommand == MOVE_DOWN && buttonPressed) {
					player.moveDown();
				}

				else if (unicodeCommand == MOVE_LEFT && buttonPressed) {
					player.moveLeft();
				}

				else if (unicodeCommand == MOVE_RIGHT && buttonPressed) {
					player.moveRight();
				}

				else {
					continue;
				}

			}
			else
				continue;
		}
	}

	void buildLevel() {
		//TODO
		return;

	}

private:

	HANDLE hOut;
	HANDLE hIn;

	int m_ScreenWidth;
	int m_ScreenHeight;

	bool foodEaten = true;
	int foodPositionX;
	int foodPositionY;

	Node* actualNodePosition;

	Player player;

	INPUT_RECORD commandBuffer[256];
	DWORD cNumRead;

	SMALL_RECT m_rectWidow;
	CHAR_INFO *m_ScreenBuffer;

};




int main()
{
    //Window size
	int windowWidth = 60;
	int windowHeigth = 20;

	//Font size

	int fontWidth = 2;
	int fontHeight = 2;

	Game maze(windowWidth, windowHeigth, fontWidth, fontHeight);

	maze.initializeGame();

	maze.gameLoop();

	return 0;
}