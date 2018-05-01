#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Character.h"

class GameState //holds the current gameState being monsters defeated and if game is over
{
public:
	GameState();
	std::string getMonDef(); //returns string of defeated monsters info
	bool getGamewin(); 
	bool getGameLose();
	void changeGameWin();
	void changeGameLose();
	void addMonDef(Character* monDef); //adds a defeated monster to the list

private:
	bool gameWin;
	bool gameLose;
	std::string monstersDef;
	int totalDef;
};

#endif