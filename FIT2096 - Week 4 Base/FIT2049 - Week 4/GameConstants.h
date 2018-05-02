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


public:
	GameConstants() {}
	int getBoardWidth() { return BOARD_WIDTH; }
	int getBoardHeight() { return BOARD_HEIGHT; }
	int getMonNum() { return MON_NUM; }
	int getHealNum() { return HEAL_NUM; }
	int getTeleNum() { return TELE_NUM; }
};
#endif