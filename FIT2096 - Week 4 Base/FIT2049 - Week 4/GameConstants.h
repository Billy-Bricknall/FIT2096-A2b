#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

class GameConstants {
private:
	//game.cpp
	const int BOARD_WIDTH = 15;
	const int BOARD_HEIGHT = 15;
	const int MON_NUM = 5;
	const int HEAL_NUM = 5;
	const int TELE_NUM = 2;
	
	//Player.cpp
	const float BULLET_TIMEOUT = 10;
	const float MOVE_SPEED = 2.5;
	const float CAMERA_HEIGHT = 0.6;

	//Tile.cpp
	const float GUN_OFFSET_X = -0.133;
	const float GUN_OFFSET_Y = 1.2;
	const float GUN_OFFSET_Z = 0.137;
	const int SHOT_TIMER = 1000;
	const float CUTOFF_DISTANCE = 3;

	//Bullet.cpp
	const float BULLET_VOLUME = 0.3f;


public:
	GameConstants() {}
	int getBoardWidth() { return BOARD_WIDTH; }
	int getBoardHeight() { return BOARD_HEIGHT; }
	int getMonNum() { return MON_NUM; }
	int getHealNum() { return HEAL_NUM; }
	int getTeleNum() { return TELE_NUM; }
	float getBTime() { return BULLET_TIMEOUT; }
	float getMoveSpeed() { return MOVE_SPEED; }
	float getCamHeight() { return CAMERA_HEIGHT; }
	float getGunOffsetX() { return GUN_OFFSET_X; }
	float getGunOffsetY() { return GUN_OFFSET_Y; }
	float getGunOffsetZ() { return GUN_OFFSET_Z; }
	int getShotTimer() { return SHOT_TIMER; }
	float getCutoffDistance() { return CUTOFF_DISTANCE; }
	float getBulletVolume() { return BULLET_VOLUME; }
};
#endif