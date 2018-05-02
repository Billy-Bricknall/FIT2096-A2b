#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "GameConstants.h"

class Character { //generic character class for monsters and players stats

private:
	std::string name;
	int health;
	int maxHealth;

public:
	Character(std::string newName, int newHealth);
	//accessors
	std::string getName();
	int getHealth();
	int getMaxHealth();

	//mutators
	void changeHealth(int addHealth);
};
#endif