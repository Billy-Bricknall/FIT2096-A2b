#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "Player.h"
#include "Tile.h"
#include "Bullet.h"

#define MAX_ALLOWED_COLLISIONS 2048

class Player;
class Tile;
class Bullet;
class GameState;

class CollisionManager
{
private:
	Player* m_player;
	std::vector<Tile*>* m_tileObjects;
	std::vector<Bullet*>* m_bullets;
	std::vector<CPlane*> m_walls;
	std::vector<Tile*>* m_enemies;
	GameState* m_gState;

	GameObject* m_currentCollisions[MAX_ALLOWED_COLLISIONS];

	// We need to know what objects were colliding last frame so we can determine if a collision has just begun or ended
	GameObject* m_previousCollisions[MAX_ALLOWED_COLLISIONS];

	int m_nextCurrentCollisionSlot;

	// Check if we already know about two objects colliding
	bool ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second);

	// Register that a collision has occurred
	void AddCollision(GameObject* first, GameObject* second);

	// Collision check helpers
	void PlayerToEnemy();
	void PlayerToBullet();
	void PlayerToWall();

	void EnemyToBullet();
	void EnemyToWall();

public:
	CollisionManager(Player* player, std::vector<Tile*>* tileObjects, GameState* gState);
	void CheckCollisions();

};

#endif