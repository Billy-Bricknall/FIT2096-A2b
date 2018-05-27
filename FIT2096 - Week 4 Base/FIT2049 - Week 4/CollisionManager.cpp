#include "CollisionManager.h"

CollisionManager::CollisionManager(Player* player, std::vector<Tile*>* tileObjects, GameState* gState)
{
	m_gState = gState;
	m_player = player;
	m_tileObjects = tileObjects;
	m_bullets = new vector<Bullet*>;
	m_walls.push_back(new CPlane(Vector3(1, 0, 0), 7));
	m_walls.push_back(new CPlane(Vector3(1, 0, 0), -7));
	m_walls.push_back(new CPlane(Vector3(0, 0, 1), 7));
	m_walls.push_back(new CPlane(Vector3(0, 0, 1), -7));

	m_enemies = new vector<Tile*>;
	for (int i = 0; i < m_tileObjects->size(); i++) {
		if ((*m_tileObjects)[i]->getType() == "mon")
			m_enemies->push_back((*m_tileObjects)[i]);
	}

	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions(){

	m_bullets->clear();
	for (int i = 0; i < m_player->getBullet().size(); i++) {
		m_bullets->push_back(m_player->getBullet()[i]);
	}

	PlayerToEnemy();
	PlayerToBullet();
	PlayerToWall();

	EnemyToBullet();
	EnemyToWall();
	

	// Move all current collisions into previous
	memcpy(m_previousCollisions, m_currentCollisions, sizeof(m_currentCollisions));

	// Clear out current collisions
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	
	// Now current collisions is empty, we'll start adding from the start again
	m_nextCurrentCollisionSlot = 0;

}

bool CollisionManager::ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second)
{
	// See if these two GameObjects appear one after the other in specified collisions array
	// Stop one before length so we don't overrun as we'll be checking two elements per iteration
	for (int i = 0; i < MAX_ALLOWED_COLLISIONS - 1; i += 2)
	{
		if ((arrayToSearch[i] == first && arrayToSearch[i + 1] == second) ||
			arrayToSearch[i] == second && arrayToSearch[i + 1] == first)
		{
			// Found them!
			return true;
		}
	}

	// These objects were not colliding last frame
	return false;
}

void CollisionManager::AddCollision(GameObject* first, GameObject* second)
{	
	// Add the two colliding objects to the current collisions array
	// We keep track of the next free slot so no searching is required
	m_currentCollisions[m_nextCurrentCollisionSlot] = first;
	m_currentCollisions[m_nextCurrentCollisionSlot + 1] = second;
	
	m_nextCurrentCollisionSlot += 2;
}

void CollisionManager::PlayerToEnemy(){

	for (unsigned int j = 0; j < m_tileObjects->size(); j++)
	{

		Player* player = m_player;
		Tile* tileMesh = (*m_tileObjects)[j];

		if (tileMesh->getType() == "mon" && tileMesh->getBullet() != NULL) {
			m_bullets->push_back(tileMesh->getBullet());
		}

		CBoundingBox playerBounds = player->getBounds();
		CBoundingBox tileMeshBounds = tileMesh->getBounds();

		// Are they colliding this frame?
		bool isColliding = CheckCollision(playerBounds, tileMeshBounds);

		// Were they colliding last frame?
		bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, tileMesh);

		if (isColliding)
		{
			// Register the collision
			AddCollision(player, tileMesh);

			if (wasColliding)
			{

			}
			else
			{
				// We are colliding this frame and we weren't last frame - that's a collision enter
				if (tileMesh->getType() == "mon") {
					player->enemyHasCollided();
				}
				else if (tileMesh->getType() == "heal") {
					player->healthHasCollided();
				}
				else if (tileMesh->getType() == "tele") {
					player->teleportHasCollided(tileMesh);
				}
				tileMesh->hasCollided();
			}
		}
		else
		{
			if (wasColliding)
			{

			}
		}
	}
}

void CollisionManager::PlayerToBullet(){


	for (unsigned int j = 0; j < m_bullets->size(); j++){
		bool ownBullet = false;
		Player* player = m_player;
		Bullet* bullet = (*m_bullets)[j];

		for (int i = 0; i < player->getBullet().size(); i++) {
			if (player->getBullet()[i] == bullet) {
				ownBullet = true;
				i = player->getBullet().size();
			}
		}

		if (!ownBullet && !bullet->getHasHit()) {
			CBoundingBox playerBounds = player->getBounds();
			CBoundingBox bulletBounds = bullet->getBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(playerBounds, bulletBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, bullet);

			if (isColliding){
				// Register the collision
				AddCollision(player, bullet);

				if (wasColliding){

				}
				else{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					player->bulletHasCollided(bullet);
					bullet->hasCollided();
				}
			}
			else{
				if (wasColliding){

				}
			}
		}
	}
}

void CollisionManager::PlayerToWall(){
	for (unsigned int j = 0; j < m_walls.size(); j++)
	{

		Player* player = m_player;
		CPlane wall = *m_walls[j];

		CBoundingBox playerBounds = player->getBounds();

		// Are they colliding this frame?
		bool isColliding = CheckPlane(wall, playerBounds);

		if (isColliding)
		{
			player->wallHasCollided();
		}
	}
}

void CollisionManager::EnemyToBullet(){
	for (unsigned int i = 0; i < m_enemies->size(); i++) {
		if ((*m_enemies)[i]->getType() == "mon") {
			for (unsigned int j = 0; j < m_bullets->size(); j++) {

				bool ownBullet = false;
				Tile* tileMesh = (*m_enemies)[i];
				Bullet* bullet = (*m_bullets)[j];

				if (bullet == tileMesh->getBullet())
					ownBullet = true;

				if (!ownBullet && !bullet->getHasHit()) {
					CBoundingBox enemyBounds = tileMesh->getBounds();
					CBoundingBox bulletBounds = bullet->getBounds();

					// Are they colliding this frame?
					bool isColliding = CheckCollision(enemyBounds, bulletBounds);

					// Were they colliding last frame?
					bool wasColliding = ArrayContainsCollision(m_previousCollisions, tileMesh, bullet);

					if (isColliding) {
						// Register the collision
						AddCollision(tileMesh, bullet);

						if (wasColliding) {

						}
						else {
							// We are colliding this frame and we weren't last frame - that's a collision enter
							tileMesh->bulletHasCollided(bullet, m_gState);
							bullet->hasCollided();
						}
					}
					else {
						if (wasColliding) {

						}
					}
				}
			}
		}
	}
}

void CollisionManager::EnemyToWall(){
	for (unsigned int i = 0; i < m_enemies->size(); i++) {
		if ((*m_enemies)[i]->getType() == "mon") {
			for (unsigned int j = 0; j < m_walls.size(); j++) {

				Tile* enemy = (*m_enemies)[i];
				CPlane wall = *m_walls[j];

				CBoundingBox enemyBounds = enemy->getBounds();

				// Are they colliding this frame?
				bool isColliding = CheckPlane(wall, enemyBounds);

				if (isColliding)
				{
					enemy->wallHasCollided();
				}
			}
		}
	}
}

/*void CollisionManager::KartToKart()
{
	// We'll check each kart against every other kart
	// Note this is not overly efficient, both in readability and runtime performance

	for (int i = 0; i < m_karts->size(); i++)
	{
		for (int j = 0; j < m_karts->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			Kart* firstKart = (*m_karts)[i];
			Kart* secondKart = (*m_karts)[j];

			// Don't compare a kart to itself
			// Comparing memory addresses - beauty of pointers
			if (firstKart != secondKart)
			{
				CBoundingBox firstBounds = firstKart->GetBounds();
				CBoundingBox secondBounds = secondKart->GetBounds();

				// Do we already know about a collision between these two karts
				bool alreadyHandled = ArrayContainsCollision(m_currentCollisions, firstKart, secondKart);

				if (!alreadyHandled)
				{
					// Are they colliding this frame?
					bool isColliding = CheckCollision(firstBounds, secondBounds);

					// Were they colliding last frame?
					bool wasColliding = ArrayContainsCollision(m_previousCollisions, firstKart, secondKart);

					if (isColliding)
					{
						// Register the collision
						AddCollision(firstKart, secondKart);

						if (wasColliding)
						{
							// We are colliding this frame and we were also colliding last frame - that's a collision stay
							// Tell both karts so they can respond. Also give each one the other kart.
							firstKart->OnKartCollisionStay(secondKart);
							secondKart->OnKartCollisionStay(firstKart);
						}
						else
						{
							// We are colliding this frame and we weren't last frame - that's a collision enter
							firstKart->OnKartCollisionEnter(secondKart);
							secondKart->OnKartCollisionEnter(firstKart);
						}
					}
					else
					{
						if (wasColliding)
						{
							// We aren't colliding this frame but we were last frame - that's a collision exit
							firstKart->OnKartCollisionExit(secondKart);
							secondKart->OnKartCollisionExit(firstKart);
						}
					}
				}
			}
		}
	}
}*/

