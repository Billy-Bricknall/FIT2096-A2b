#ifndef PLAYER_H
#define PLAYER_H

#include "Tile.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "GameState.h"
#include "Bullet.h"
#include <vector>

class Player: public GameObject{

public:
	Player(Vector3 position, InputController* newInput, GameConstants* newGConsts, AudioSystem* audio);
	~Player();
	void update(float timestep, Tile* gBoard[15][15], TextureManager* textureManager, MeshManager* meshManager, Shader* shader);
	GameState* getGamestate(); //returns current gamestate
	Character* getCharacter() { return player1; }
	void shoot(TextureManager* textureManager, MeshManager* meshManager, Shader* shader);
	vector<Bullet*> getBullet();
	CBoundingBox getBounds();
	void hasCollided();


private:
	InputController * m_input;
	AudioSystem* m_audio;
	int posX; //x position
	int posZ; //z position
	Vector3 targetPos; //where player is moving to
	float m_moveSpeed;
	Character* player1; //holds and controls players stats/battles
	vector<int> movementCue; // holds all valid keystrokes
	GameState* gState; //holds the current games state.
	GameConstants* m_gConsts;
	vector<Bullet*> bullets;
	bool isMoving;
	CBoundingBox m_boundingBox;
	

	float m_heading;
	float m_pitch;

	void actionTile(Tile* gBoard[15][15]); //performs action tiles actions

};
#endif