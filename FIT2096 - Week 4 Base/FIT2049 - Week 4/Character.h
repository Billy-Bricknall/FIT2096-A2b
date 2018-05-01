#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character { //generic character class for monsters and players stats

private:
	std::string name;
	int health;
	int attack;
	int defense;
	int maxHealth;

public:
	Character(std::string newName, int newHealth, int newAttack, int newDefense);
	//accessors
	std::string getName();
	int getHealth();
	int getAttack();
	int getDefense();
	int getMaxHealth();

	//mutators
	void changeHealth(int addHealth);
	void changeAttack(int addAttack);
	void changeDefense(int addDefense);

	void battle(Character* defender); //performs a single attack onto the enemy defender
	void levelUp(); //increases all of characters stats by 0-2 levels

};
#endif