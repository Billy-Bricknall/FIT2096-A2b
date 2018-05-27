#include "Character.h"

Character::Character(std::string newName, int newHealth) {
	name = newName;
	health = newHealth;
	maxHealth = newHealth;
}

std::string Character::getName() { return name; }
int Character::getHealth() { return health; }
int Character::getMaxHealth(){return maxHealth;}

void Character::changeHealth(int addHealth) {
	health += addHealth;
	if (health > maxHealth) { health = maxHealth; } //health cant be greater than max
	if (health < 0) { health = 0;	} //health cant be lower than 0 for scaling reasons
}