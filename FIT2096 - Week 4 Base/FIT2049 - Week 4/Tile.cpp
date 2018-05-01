#include "Tile.h"

Tile::Tile(int newPosX, int newPosY, Mesh* mesh, Shader* shader, Vector3 position, Texture* texture) : GameObject(mesh, shader, position, texture){
	positionX = newPosX;
	positionY = newPosY;
	isActive = true;
	type = "blank"; //blank means tile doesnt have an action
	charMesh = NULL;
}

Tile::~Tile(){
	delete charMesh;
	delete enemy;
	delete b1;
}

void Tile::deactivate() {
	isActive = false;
	charMesh = NULL;
}

void Tile::linkTile(Tile * newLink) {
	linkingTile = newLink; //links tile for teleport
}

void Tile::setEnemy(Character * newEnemy){
	enemy = newEnemy; //stores enemy stats for monster tiles
}

void Tile::setType(string newType) {
	type = newType;
}

int Tile::getPosX() {
	return positionX;
}

int Tile::getPosY() {
	return positionY;
}

bool Tile::getActive() {
	return isActive;
}

string Tile::getType() {
	return type;
}

Tile* Tile::getLink() {
	return linkingTile;
}

Character * Tile::getEnemy(){
	return enemy;
}

GameObject * Tile::getCharMesh(){
	return charMesh;
}

Bullet * Tile::getBullet(){
	return b1;
}

void Tile::update(float timestep, TextureManager* textureManager, MeshManager* meshManager, Vector3 pos) { //changes textures depending on types and if active
	if(!isActive){
		m_texture = textureManager->GetTexture("Assets/Textures/tile_disabled.png");
	}
	else if (type == "heal") {
		m_texture = textureManager->GetTexture("Assets/Textures/tile_green.png");
		charMesh = new GameObject(meshManager->GetMesh("Assets/Meshes/player_capsule.obj"), m_shader, m_position + Vector3(0.5f, 0.2f, 0.0f), textureManager->GetTexture("Assets/Textures/tile_green.png")); //creates 3d sprite
		charMesh->SetZRotation(1.57); //rotation capsule to side
	}
	else if (type == "mon") {
		if (enemy->getMaxHealth() == 50) { m_texture = textureManager->GetTexture("Assets/Textures/tile_red1.png"); }
		else if (enemy->getMaxHealth() == 40) { m_texture = textureManager->GetTexture("Assets/Textures/tile_orange1.png"); }
		else if (enemy->getMaxHealth() == 30) { m_texture = textureManager->GetTexture("Assets/Textures/tile_orange.png"); }
		else if (enemy->getMaxHealth() == 20) { m_texture = textureManager->GetTexture("Assets/Textures/tile_red.png"); }
		else { m_texture = textureManager->GetTexture("Assets/Textures/tile_red_light.png"); }
		charMesh = new GameObject(meshManager->GetMesh("Assets/Meshes/enemy.obj"), m_shader, m_position, textureManager->GetTexture("Assets/Textures/gradient_redDarker.png")); //creates 3d sprite
	}
	else if (type == "tele") {
		m_texture = textureManager->GetTexture("Assets/Textures/tile_blue.png");
	}

	if (charMesh != NULL && type == "mon") {
		float angle;
		Vector3 difference = m_position - pos;
		angle = atan(difference.x / difference.z); //calculates rotation
		if (difference.z > 0) { angle = angle + 3.14; }
		
		charMesh->SetYRotation(angle); //makes enemies look at player

		charMesh->SetUniformScale(enemy->getHealth()/50.0f); //makes weaker enemies smaller
		Vector3 gunPos = Vector3(-0.133*enemy->getHealth() / 50.0f, 1.2*enemy->getHealth() / 50.0f, 0.137*enemy->getHealth() / 50.0f); //calculates gun position based on size
		gunPos = Vector3(m_position.x, m_position.y, m_position.z) + Vector3::TransformNormal(gunPos, Matrix::CreateRotationY(charMesh->GetYRotation())); //rotates gun position base on rotation and calculates exact position

		if (timer % 500 == 0) {
			if(b1)
				delete b1;
			b1 = new Bullet(meshManager->GetMesh("Assets/Meshes/bullet.obj"), m_shader, gunPos, textureManager->GetTexture("Assets/Textures/bullet.png"), charMesh->GetYRotation()); //creates bullet
		}
		timer++;
		charMesh->Update(timestep);
		b1->update(timestep);
	}
}