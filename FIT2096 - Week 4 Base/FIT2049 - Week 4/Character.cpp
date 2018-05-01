#include "Character.h"

Character::Character(std::string newName, int newHealth, int newAttack, int newDefense) {
	name = newName;
	health = newHealth;
	attack = newAttack;
	defense = newDefense;
	maxHealth = newHealth;
}

std::string Character::getName() { return name; }
int Character::getHealth() { return health; }
int Character::getAttack() { return attack; }
int Character::getDefense() { return defense; }
int Character::getMaxHealth(){return maxHealth;}

void Character::changeHealth(int addHealth) {
	health += addHealth;
	if (health > maxHealth) { health = maxHealth; } //health cant be greater than max
	if (health < 0) { health = 0; } //health cant be lower than 0 for scaling reasons
}

void Character::changeAttack(int addAttack) {
	attack += addAttack;
	if (attack > 10) { attack = 10; }
}

void Character::changeDefense(int addDefense) {
	defense += addDefense;
	if (defense > 10) { defense = 10; }
}

void Character::battle(Character* defender) { //similar battle logic from FIT1048
	int totalAttack = attack + (rand() % 6); //random total attack
	int totalDefense = defender->getDefense() + (rand() % 6); //random total defense
	int damage = totalAttack - totalDefense; 
	if (damage < 0) { damage = 0; } //cant do negative damage
	defender->changeHealth(-damage);
}

void Character::levelUp() { //random stat increase
	changeAttack(rand() % 3);
	changeDefense(rand() % 3);
}