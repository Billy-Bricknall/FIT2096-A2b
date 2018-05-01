/*	FIT2049 - Example Code
*	Game.cpp
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"
#include <iostream>
#include "DirectXTK/CommonStates.h"
#include <sstream>

Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_unlitTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();


	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	LoadFonts();
	InitUI();

	InitGameWorld();
	RefreshUI();
	m_currentCam = new FirstPersonCamera(p1);

	return true;
}

bool Game::InitShaders()
{
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/VertexColourPixelShader.ps"))
		return false;
	
	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/TexturedPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/player_capsule.obj"))
		return false;
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/floor_tile.obj"))
		return false;
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/wall_tile.obj"))
		return false;
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;
	
	return true;
}

bool Game::LoadTextures()
{
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_blue.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_disabled.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_green.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_orange.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_orange1.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_purple.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_red.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_red1.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_red_light.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/tile_white.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/block_tiles_red.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redDarker.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar1.png"))
		return false;
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/bullet.png"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
}

void Game::InitUI()
{
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());

	m_healthBar = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");

	m_healthBar1 = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar1.png");
}

void Game::RefreshUI()
{
	if (p1){
		std::wstringstream ss;
		ss << p1->getGamestate()->getMonDef().c_str();
		m_monsDefeated = ss.str();
	}
}

void Game::InitGameWorld()
{
	generateBoard(); //randomly generates a gameboard
	p1 = new Player(Vector3(0.0f, 0.0f, 0.0f), m_input);
}

void Game::Update(float timestep){
	m_input->BeginUpdate();

	if (!p1->getGamestate()->getGameLose() && !p1->getGamestate()->getGamewin()) { //if game isnt over
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				m_board[i][j]->update(timestep, m_textureManager, m_meshManager, p1->GetPosition()); //update all tiles
			}
		}
		p1->update(timestep, m_board); //update player
	}
	else {
		MessageBox(NULL, p1->getGamestate()->getMonDef().c_str(), "GameOver", MB_OK); //gameover box with all mons defeated
		PostQuitMessage(0); //quits application
	}

	RefreshUI();
	
	m_currentCam->Update(timestep);

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

	// TODO render all gameobjects
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			m_board[i][j]->Render(m_renderer, m_currentCam); //renders board
			if (m_board[i][j]->getCharMesh()) {
				m_board[i][j]->getCharMesh()->Render(m_renderer, m_currentCam); //renders character meshes
				if (m_board[i][j]->getBullet()) {
					m_board[i][j]->getBullet()->Render(m_renderer, m_currentCam);
				}
			}
		}
	}
	for (int i = 0; i < 64; i++) {
		m_wall[i]->Render(m_renderer, m_currentCam); //renders walls
	}
	DrawUI();
	m_renderer->EndScene();		
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	std::wstringstream ss;
	ss << "|";
	wstring temp = ss.str();

	m_arialFont12->DrawString(m_spriteBatch, m_monsDefeated.c_str(), Vector2(0, 100), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0)); //draws the score as in monsters defeated

	for (int i = 0; i < 10; i++) {
		m_spriteBatch->Draw(m_healthBar->GetShaderResourceView(), Vector2(479 + 32 * i, 5), Color(1.0f, 1.0f, 1.0f)); //draws health bar background
	}
	int health = p1->getCharacter()->getHealth();
	for (int i = 0; i < health; i++) {
		m_spriteBatch->Draw(m_healthBar1->GetShaderResourceView(), Vector2(639 + 3.18 * i, 5), Color(1.0f, 0.0f, 0.0f));//draws current health level
		m_spriteBatch->Draw(m_healthBar1->GetShaderResourceView(), Vector2(635 - 3.18 * i, 5), Color(1.0f, 0.0f, 0.0f));
	}
	m_arialFont12->DrawString(m_spriteBatch, temp.c_str(), Vector2(636, 10), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0));//draws a line at the midpoint of the health bar
	m_spriteBatch->End();
}

void Game::Shutdown()
{
	// TODO delete all gameobjects

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

}

void Game::generateBoard() {
	int numToDeactivate = 24; //number of tiles to deactivate
	int numToLink = 2; //number of tiles to link (1 makes a link between 2 tiles)
	int monNum = 5; //number of monsters
	int healNum = 6; //number of heal tiles
	int x;
	int y;

	int temp = 0;
	for (int i = -8; i < 9; i++) { //from here...
		m_wall[temp] = new GameObject(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"), m_unlitTexturedShader, Vector3(i, 0, -8), m_textureManager->GetTexture("Assets/Textures/ground.png"));
		m_wall[temp]->Update(NULL);
		temp++;
		m_wall[temp] = new GameObject(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"), m_unlitTexturedShader, Vector3(i, 0, 8), m_textureManager->GetTexture("Assets/Textures/ground.png"));
		m_wall[temp]->Update(NULL);
		temp++;
	}
	for (int i = -7; i < 8; i++) {
		m_wall[temp] = new GameObject(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"), m_unlitTexturedShader, Vector3(-8, 0, i), m_textureManager->GetTexture("Assets/Textures/ground.png"));
		m_wall[temp]->Update(NULL);
		temp++;
		m_wall[temp] = new GameObject(m_meshManager->GetMesh("Assets/Meshes/wall_tile.obj"), m_unlitTexturedShader, Vector3(8, 0, i), m_textureManager->GetTexture("Assets/Textures/ground.png"));
		m_wall[temp]->Update(NULL);
		temp++;
	} //...to here spawns walls


	for (int i = -7; i < 8; i++) {
		for (int j = -7; j < 8; j++) { //following creates blank slate
			m_board[i+7][j+7] = new Tile(i, j, m_meshManager->GetMesh("Assets/Meshes/floor_tile.obj"), m_unlitTexturedShader, Vector3(i, 0, j), m_textureManager->GetTexture("Assets/Textures/tile_white.png")); //string can be blank, mon1-5, tele, heal
		}
	}

	do { // the rest randomly generates which tiles are where
		do {
			x = rand() % 15;
			y = rand() % 15;
		} while (m_board[x][y]->getActive() == false || m_board[x][y]->getType() != "blank" || (x == 7 && y == 7)); // if tile hasnt been selected yet
		
		if (numToDeactivate > 0) {
			m_board[x][y]->deactivate(); //deactivates tile
			numToDeactivate--;
		}
		else if (numToLink > 0) {
			int linkX, linkY;
			m_board[x][y]->setType("tele"); //sets type

			do {
				linkX = rand() % 15;
				linkY = rand() % 15;
			} while (m_board[linkX][linkY]->getActive() == false || m_board[linkX][linkY]->getType() != "blank" || (x == 7 && y == 7)); //if tile hasnt been selected yet
			m_board[linkX][linkY]->setType("tele"); //sets type

			m_board[linkX][linkY]->linkTile(m_board[x][y]); //links tile
			m_board[x][y]->linkTile(m_board[linkX][linkY]); //links tile
			numToLink--;
		}
		else if (monNum > 0) {
			Character* tempMon;
			m_board[x][y]->setType("mon"); //sets type
			if (monNum == 1) { tempMon = new Character("mon1", 10, 1, 1); } //makes first mon
			 else if (monNum == 2) { tempMon = new Character("mon2", 20, 3, 3); }
			 else if (monNum == 3) { tempMon = new Character("mon3", 30, 5, 4); }
			 else if (monNum == 4) { tempMon = new Character("mon4", 40, 4, 8); }
			 else if (monNum == 5) { tempMon = new Character("mon5", 50, 7, 6); }
			 else { tempMon = NULL; }
			m_board[x][y]->setEnemy(tempMon);
			monNum--;
		}
		else if (healNum > 0) {
			m_board[x][y]->setType("heal"); //sets type
			healNum--;
		}
	} while (numToDeactivate > 0 || numToLink > 0 || monNum > 0 || healNum > 0);
}