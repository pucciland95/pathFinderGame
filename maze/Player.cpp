#include "Player.h"
#include <Windows.h>

Player::Player() {
}

void Player::initiatePlayer(int width, int height) {
		playerState.X = (int)width / 2;
		playerState.Y = (int)height / 2;

		m_ScreenSizeX = width;
		m_ScreenSizeY = height;
}

void Player::moveUp() {
		if (playerState.Y - 1 < 0)
			return;
		else
			playerState.Y = playerState.Y - 1;
}

void Player::moveDown() {
		if (playerState.Y + 1 > m_ScreenSizeY - 1)
			return;
		else
			playerState.Y = playerState.Y + 1;
}

void Player::moveRight() {

		if (playerState.X + 1 > m_ScreenSizeX - 1)
			return;
		else
			playerState.X = playerState.X + 1;
}

void Player::moveLeft() {

		if (playerState.X - 1 < 0)
			return;
		else
			playerState.X = playerState.X - 1;
}

COORD* Player::getPlayerState() {
		return &playerState;
}

void Player::setPlayerState(int X, int Y) {
		playerState.X = X;
		playerState.Y = Y;
}




