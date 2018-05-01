/*	FIT2049 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "FirstPersonCamera.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

class Game
{
private:
	Camera* m_currentCam;		
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;

	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;

	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;

	Player* p1; //pointer to player object
	Tile* m_board[15][15]; //pointer to 2d array for tiles
	GameObject* m_wall[64]; // pointer to array for walls

	std::wstring m_monsDefeated;

	Texture* m_healthBar;
	Texture* m_healthBar1;

	// Splitting initialisation up into several steps
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void LoadFonts();
	void InitGameWorld();

	void InitUI();
	void DrawUI();
	void RefreshUI();

public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn
	void generateBoard();

	void Shutdown(); //Cleanup everything we initialised
};

#endif