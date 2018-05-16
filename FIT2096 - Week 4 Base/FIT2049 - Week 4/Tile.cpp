#include "Tile.h"

int Tile::charSelect;

Tile::Tile(int newPosX, int newPosY, Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, GameConstants* newGConsts, TextureManager* newTextureManager, AudioSystem* audio) : GameObject(mesh, shader, position, texture){
	positionX = newPosX;
	positionY = newPosY;
	targetPos = Vector3(positionX, 0, positionY);
	isActive = true;
	type = "blank"; //blank means tile doesnt have an action
	charMesh = NULL;
	m_gConsts = newGConsts;
	textureManager = newTextureManager;
	m_audio = audio;
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

void Tile::setType(string newType, MeshManager* meshManager) {
	type = newType;
	if (type == "mon") {
		charMesh = new GameObject(meshManager->GetMesh("Assets/Meshes/enemy.obj"), m_shader, m_position, textureManager->GetTexture("Assets/Textures/gradient_redDarker.png"));
		if (charSelect == NULL)
			charSelect = 1;
		else
			charSelect++;
		charType = charSelect;
		m_moveSpeed = 1;
		shotSpeed = m_gConsts->getShotTimer() * MathsHelper::RandomRange(0.5f, 1.0f);
	}
	else if (type == "heal") {
		charMesh = new GameObject(meshManager->GetMesh("Assets/Meshes/player_capsule.obj"), m_shader, m_position + Vector3(0.5f, 0.2f, 0.0f), textureManager->GetTexture("Assets/Textures/tile_green.png")); //creates 3d sprite
		charMesh->SetZRotation(PI / 2); //rotation capsule to side
	}
	else if (type == "tele") {
		m_texture = textureManager->GetTexture("Assets/Textures/tile_blue.png");
	}
	else {
		m_texture = textureManager->GetTexture("Assets/Textures/tile_white.png");
	}

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

void Tile::update(float timestep, MeshManager* meshManager, Vector3 pos, float rot) { //changes textures depending on types and if active

	if (charMesh != NULL && type == "mon") {
		float angle;
		Vector3 difference = charMesh->GetPosition() - pos;
		angle = atan(difference.x / difference.z); //calculates rotation
		if (difference.z > 0) { angle = angle + PI; }
		
		charMesh->SetYRotation(angle); //makes enemies look at player

		charMesh->SetUniformScale(enemy->getHealth()/50.0f); //makes weaker enemies smaller
		m_moveSpeed = MathsHelper::RemapRange(enemy->getHealth(), 0.0f, 50.0f, 3.0f, 1.0f);
		
		
		if (timer % shotSpeed == 0) {
			Vector3 gunPos = Vector3(m_gConsts->getGunOffsetX()*enemy->getHealth() / 50.0f, m_gConsts->getGunOffsetY()*enemy->getHealth() / 50.0f, m_gConsts->getGunOffsetZ()*enemy->getHealth() / 50.0f); //calculates gun position based on size
			gunPos = Vector3(charMesh->GetPosition().x, charMesh->GetPosition().y, charMesh->GetPosition().z) + Vector3::TransformNormal(gunPos, Matrix::CreateRotationY(charMesh->GetYRotation())); //rotates gun position base on rotation and calculates exact position

			timer = 0;
			if(b1)
				delete b1;
			b1 = new Bullet(meshManager->GetMesh("Assets/Meshes/bullet.obj"), m_shader, gunPos, textureManager->GetTexture("Assets/Textures/bullet.png"), charMesh->GetYRotation(), m_audio, m_gConsts); //creates bullet
		}
		timer++;

		switch (charType) {
		case 1:
			enemyMovement1(timestep);
			break;
		case 2:
			enemyMovement2(timestep);
			break;
		case 3:
			if (Vector3::DistanceSquared(charMesh->GetPosition(), pos) < 9)
				enemyMovement1(timestep);
			else 
				enemyMovement3(timestep, pos, rot);
			break;
		case 4:
			enemyMovement4(timestep);
			break;
		case 5:
			enemyMovement5(timestep, pos);
			break;
		}


		charMesh->Update(timestep);
		b1->update(timestep);
	}
}

void Tile::enemyMovement1(float timestep){
	Matrix heading = Matrix::CreateRotationY(charMesh->GetYRotation());
	Vector3 localForward = Vector3::TransformNormal(Vector3(0, 0, 1), heading);

	Vector3 tempPos = charMesh->GetPosition();
	tempPos += localForward * m_moveSpeed * timestep;
	charMesh->SetPosition(tempPos);
}

void Tile::enemyMovement2(float timestep) {
	Matrix heading = Matrix::CreateRotationY(charMesh->GetYRotation());
	Vector3 localForward = Vector3::TransformNormal(Vector3(0, 0, 1), heading);

	Vector3 tempPos = charMesh->GetPosition();
	tempPos -= localForward * m_moveSpeed * timestep;

	int width = m_gConsts->getHalfWidth();
	int height = m_gConsts->getHalfHeight();
	float tempX = MathsHelper::Clamp(tempPos.x, -width, width);
	float tempZ = MathsHelper::Clamp(tempPos.z, -height, height);
	tempPos = Vector3(tempX, 0, tempZ);
	charMesh->SetPosition(tempPos);
}

void Tile::enemyMovement3(float timestep, Vector3 pos, float rot) {
	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix playerHeading = Matrix::CreateRotationY(rot);
	Vector3 playerForward = Vector3::TransformNormal(worldForward, playerHeading);

	Vector3 goTo = pos + playerForward * m_gConsts->getCutoffDistance();

	Vector3 tempPos = charMesh->GetPosition();
	Vector3 moveVec = goTo - tempPos;
	moveVec.Normalize();
	
	tempPos += moveVec * m_moveSpeed * timestep;

	int width = m_gConsts->getHalfWidth();
	int height = m_gConsts->getHalfHeight();
	float tempX = MathsHelper::Clamp(tempPos.x, -width, width);
	float tempZ = MathsHelper::Clamp(tempPos.z, -height, height);
	tempPos = Vector3(tempX, 0, tempZ);
	charMesh->SetPosition(tempPos);
}

void Tile::enemyMovement4(float timestep) {
	int width = m_gConsts->getHalfWidth();
	int height = m_gConsts->getHalfHeight();

	if (Vector3::DistanceSquared(charMesh->GetPosition(), targetPos) < 0.1f) {
		float targetPosX = MathsHelper::RandomRange(-width, width);
		float targetPosZ = MathsHelper::RandomRange(-height, height);
		targetPos = Vector3(targetPosX, 0, targetPosZ);
	}

	Vector3 tempPos = charMesh->GetPosition();
	Vector3 moveVec = targetPos - tempPos;
	moveVec.Normalize();

	tempPos += moveVec * m_moveSpeed * timestep;

	charMesh->SetPosition(tempPos);
}

void Tile::enemyMovement5(float timestep, Vector3 pos) {
	int width = m_gConsts->getHalfWidth();
	int height = m_gConsts->getHalfHeight();

	if (Vector3::DistanceSquared(charMesh->GetPosition(), targetPos) < 0.1f && Vector3::DistanceSquared(charMesh->GetPosition(), pos) < 4.0f) {
		float targetPosX = MathsHelper::RandomRange(-width, width);
		float targetPosZ = MathsHelper::RandomRange(-height, height);
		targetPos = Vector3(targetPosX, 0, targetPosZ);
	}

	Vector3 tempPos = charMesh->GetPosition();
	Vector3 moveVec = targetPos - tempPos;
	moveVec.Normalize();

	tempPos += moveVec * m_moveSpeed * timestep;

	charMesh->SetPosition(tempPos);
}
