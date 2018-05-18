#ifndef TILE_H
#define TILE_H

#include "GameObject.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Character.h"
#include "Bullet.h"
#include "GameConstants.h"
#include "AudioSystem.h"
#include "Player.h"

using namespace std;

class Tile: public GameObject{
private:
	bool isActive; // tile allowed to be used
	string type; //monster1, 2, 3, 4, heal, teleport
	Tile* linkingTile; //tile that is linked for teleporting
	int positionX; //x position of tile
	int positionY; //y position of tile
	TextureManager* textureManager;
	GameConstants* m_gConsts;
	AudioSystem* m_audio;
	//below only for enemies
	Character* enemy; //stores enemy stats and battle logic for monster tiles
	Bullet* b1;
	GameObject* charMesh;
	int charType;
	int timer;
	int shotSpeed;
	Vector3 targetPos;


public:
	Tile(int newPosX, int newPosY, Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, GameConstants* newGConsts, TextureManager* newTextureManager, AudioSystem* audio);
	~Tile();

	//accessors
	int getPosX();
	int getPosY();
	bool getActive();
	string getType();
	Tile* getLink();
	//below only for enemies
	Character* getEnemy();
	GameObject* getCharMesh();
	Bullet* getBullet();

	//mutators
	void deactivate(); //deactivates the tile
	void linkTile(Tile* newLink); //links 2 tiles
	void setType(string newType, MeshManager* meshManager); // sets type of tile
	//below only for enemies
	void setEnemy(Character* newEnemy); //sets and enemy to a tile
	static int charSelect;

	void enemyMovement1(float timestep);
	void enemyMovement2(float timestep);
	void enemyMovement3(float timestep, Vector3 pos, float rot);
	void enemyMovement4(float timestep);
	void enemyMovement5(float timestep, Vector3 pos);

	void hasCollided();

	void update(float timestep, MeshManager* meshManager, Vector3 pos, float rot); //changes texture depending on type
};
#endif