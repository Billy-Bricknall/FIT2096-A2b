#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include "Direct3d.h"


class GameConstants {
private:
	//game.cpp
	const int BOARD_WIDTH = 15;
	const int BOARD_HEIGHT = 15;
	int halfBoardWidth = (BOARD_WIDTH - 1) / 2;
	int halfBoardHeight = (BOARD_HEIGHT - 1) / 2;
	const int MON_NUM = 5;
	const int HEAL_NUM = 5;
	const int TELE_NUM = 2;
	const float MUSIC_VOLUME = 0.3;
	
	//Player.cpp
	const float BULLET_TIMEOUT = 10;
	const float MOVE_SPEED = 2.5;
	const float CAMERA_HEIGHT = 0.6;
	const int HEAL_AMOUNT = 20;
	const Vector3 MIN_PLAYER_BOUNDS = Vector3(-0.2, 0.0, -0.2);
	const Vector3 MAX_PLAYER_BOUNDS = Vector3(0.2, 2.0, 0.2);
	const int PLAYER_SHOT_TIMER = 50;

	//Tile.cpp
	const float GUN_OFFSET_X = -0.133;
	const float GUN_OFFSET_Y = 1.2;
	const float GUN_OFFSET_Z = 0.137;
	const int SHOT_TIMER = 1000;
	const float CUTOFF_DISTANCE = 3;

	//Bullet.cpp
	const float BULLET_VOLUME = 0.3f;
	const int MIN_BULLET_DAMAGE = 2;
	const int MAX_BULLET_DAMAGE = 6;


public:
	GameConstants() {}
	int getBoardWidth() { return BOARD_WIDTH; }
	int getBoardHeight() { return BOARD_HEIGHT; }
	int getHalfWidth() { return halfBoardWidth; }
	int getHalfHeight() { return halfBoardHeight; }
	int getMonNum() { return MON_NUM; }
	int getHealNum() { return HEAL_NUM; }
	int getTeleNum() { return TELE_NUM; }
	float getMusicVol() { return MUSIC_VOLUME; }

	float getBTime() { return BULLET_TIMEOUT; }
	float getMoveSpeed() { return MOVE_SPEED; }
	float getCamHeight() { return CAMERA_HEIGHT; }
	int getHealAmount() { return HEAL_AMOUNT; }
	Vector3 getMinPBounds() { return MIN_PLAYER_BOUNDS; }
	Vector3 getMaxPBounds() { return MAX_PLAYER_BOUNDS; }
	int getPlayerShotTimer() { return PLAYER_SHOT_TIMER; }

	float getGunOffsetX() { return GUN_OFFSET_X; }
	float getGunOffsetY() { return GUN_OFFSET_Y; }
	float getGunOffsetZ() { return GUN_OFFSET_Z; }
	int getShotTimer() { return SHOT_TIMER; }
	float getCutoffDistance() { return CUTOFF_DISTANCE; }

	float getBulletVolume() { return BULLET_VOLUME; }
	int getMinBulletDamage() { return MIN_BULLET_DAMAGE; }
	int getMaxBulletDamage() { return MAX_BULLET_DAMAGE; }

};
#endif