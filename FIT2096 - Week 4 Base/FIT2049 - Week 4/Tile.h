#ifndef TILE_H
#define TILE_H

#include "GameObject.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Character.h"
#include "Bullet.h"
#include "GameConstants.h"

using namespace std;

class Tile: public GameObject{
private:
	bool isActive; // tile allowed to be used
	string type; //monster1, 2, 3, 4, heal, teleport
	Tile* linkingTile; //tile that is linked for teleporting
	int positionX; //x position of tile
	int positionY; //y position of tile
	Character* enemy; //stores enemy stats and battle logic for monster tiles
	Bullet* b1;
	GameObject* charMesh;
	GameConstants* m_gConsts;
	int timer;

public:
	Tile(int newPosX, int newPosY, Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, GameConstants* newGConsts);
	~Tile();

	//accessors
	int getPosX();
	int getPosY();
	bool getActive();
	string getType();
	Tile* getLink();
	Character* getEnemy();
	GameObject* getCharMesh();
	Bullet* getBullet();

	//mutators
	void deactivate(); //deactivates the tile
	void linkTile(Tile* newLink); //links 2 tiles
	void setEnemy(Character* newEnemy); //sets and enemy to a tile
	void setType(string newType); // sets type of tile

	void update(float timestep, TextureManager* textureManager, MeshManager* meshManager, Vector3 pos); //changes texture depending on type

	void enemyMovement1();
	void enemyMovement2();
	void enemyMovement3();
	void enemyMovement4();
	void enemyMovement5();

};
#endif