#ifndef PLAYER_H
#define PLAYER_H

#include "Tile.h"
#include "MeshManager.h"
#include "GameState.h"
#include <vector>

class Player: public GameObject{

public:
	Player(Vector3 position, InputController* newInput);
	~Player();
	void update(float timestep, Tile* gBoard[15][15]);
	Vector3 getPosition();
	GameState* getGamestate(); //returns current gamestate
	Character* getCharacter() { return player1; }

private:
	InputController * m_input;
	int posX; //x position
	int posZ; //z position
	Vector3 targetPos; //where player is moving to
	float m_moveSpeed;
	Character* player1; //holds and controls players stats/battles
	vector<int> movementCue; // holds all valid keystrokes
	GameState* gState; //holds the current games state.
	bool isMoving;

	float m_heading;
	float m_pitch;

	void actionTile(Tile* gBoard[15][15]); //performs action tiles actions

};
#endif