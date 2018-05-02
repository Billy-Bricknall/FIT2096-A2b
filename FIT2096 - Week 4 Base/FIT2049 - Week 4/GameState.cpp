#include "GameState.h"
#include <sstream>

GameState::GameState(){
	gameWin = false;
	gameLose = false;
	monstersDef = "Total monsters defeated:\n"; //start of endgame string
	totalDef = 0;
}

std::string GameState::getMonDef(){
	return monstersDef; //returns string of defeated monsters stats
}

bool GameState::getGamewin(){
	return gameWin;
}

bool GameState::getGameLose(){
	return gameLose;
}

void GameState::changeGameWin(){
	gameWin = true;
}

void GameState::changeGameLose(){
	gameLose = true;
}

void GameState::addMonDef(Character * monDef){
	std::stringstream ss;
	ss << "Monster name: " << monDef->getName() << "\n";
	ss << monDef->getName() << "'s Health: " << monDef->getHealth() << "/" << monDef->getMaxHealth() << "\n" << "\n";
	monstersDef = monstersDef + ss.str();
	totalDef++;
	if (totalDef == 5) { gameWin = true; } //game is won if all 5 mons are defeated
}