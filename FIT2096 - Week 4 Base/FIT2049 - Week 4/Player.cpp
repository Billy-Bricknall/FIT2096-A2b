#include "Player.h"
#include "MathsHelper.h"


Player::Player(Vector3 position, InputController* newInput, GameConstants* newGConsts, AudioSystem* audio) : GameObject(NULL, NULL, position){
	m_input = newInput;
	m_moveSpeed = newGConsts->getMoveSpeed();
	player1 = new Character("player", 50);
	posX = 0;
	posZ = 0;
	gState = new GameState();
	m_heading = 0.0f;
	m_pitch = 0.0f;
	m_rotateSpeed = 0.5f;
	m_gConsts = newGConsts;
	m_audio = audio;
	m_boundingBox = CBoundingBox(m_position + m_gConsts->getMinPBounds(), m_position + m_gConsts->getMaxPBounds());
	shotTimer = 0;
}

Player::~Player(){
	delete m_input;
	delete player1;
	delete gState;
	delete m_gConsts;
}

void Player::update(float timestep, Tile * gBoard[15][15], TextureManager* textureManager, MeshManager* meshManager, Shader* shader){
	int halfWidth = (m_gConsts->getBoardWidth() - 1) / 2;
	int halfHeight = (m_gConsts->getBoardHeight() - 1) / 2;

	m_heading += m_input->GetMouseDeltaX() * m_rotateSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_rotateSpeed * timestep;
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f)); //avoids gimbal lock
	if (m_heading > PI*2) { m_heading = m_heading - PI*2; } //keeps heading within 2PI for forward calculation
	if (m_heading < 0) { m_heading = m_heading + PI*2; }
	m_rotY = m_heading;
	m_rotX = m_pitch;

	Vector3 worldForward = Vector3(0, 0, 1);

	Matrix heading = Matrix::CreateRotationY(m_rotY);

	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);
	Vector3 localRight = Vector3(0, 1, 0).Cross(localForward);
	

	if (m_input->GetKeyHold('W')) {
		m_position += localForward * m_moveSpeed * timestep;
	}
	else if (m_input->GetKeyHold('S')) {
		m_position -= localForward * m_moveSpeed * timestep;
	}
	if (m_input->GetKeyHold('D')) {
		m_position += localRight * m_moveSpeed * timestep;
	}
	else if (m_input->GetKeyHold('A')) {
		m_position -= localRight * m_moveSpeed * timestep;
	}

	if (shotTimer == 0) {
		if (m_input->GetMouseUp(LEFT_MOUSE)) {
			shoot(textureManager, meshManager, shader);
			shotTimer = m_gConsts->getPlayerShotTimer();
		}
	}
	else {
		shotTimer--;
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->update(timestep);
		if (bullets[i]->getDistanceTravelled() > m_gConsts->getBTime()) {
			bullets.erase(bullets.begin());
			i--;
		}
	}

	m_boundingBox.SetMin(m_position + m_gConsts->getMinPBounds());
	m_boundingBox.SetMax(m_position + m_gConsts->getMaxPBounds());
}

GameState * Player::getGamestate(){
	return gState;
}

void Player::shoot(TextureManager* textureManager, MeshManager* meshManager, Shader* shader){
	Vector3 bulletStart = Vector3(0, m_gConsts->getCamHeight(), 0) + m_position;
	Bullet* b1 = new Bullet(meshManager->GetMesh("Assets/Meshes/bullet.obj"), shader, bulletStart, textureManager->GetTexture("Assets/Textures/bullet.png"), m_rotY, m_rotX, m_audio, m_gConsts);
	bullets.push_back(b1);
}

vector<Bullet*> Player::getBullet(){
	return bullets;
}

CBoundingBox Player::getBounds(){
	return m_boundingBox;
}

void Player::bulletHasCollided(Bullet* bulletHit){
	player1->changeHealth(-bulletHit->getBulletDamage());

	int randNum = rand() % 4 + 1;
	switch (randNum) {
	case 1:
		m_audio->Play("Assets/Sounds/Impact1.wav", false);
		break;
	case 2:
		m_audio->Play("Assets/Sounds/Impact2.wav", false);
		break;
	case 3:
		m_audio->Play("Assets/Sounds/Impact3.wav", false);
		break;
	case 4:
		m_audio->Play("Assets/Sounds/Impact4.wav", false);
		break;
	}
	if (player1->getHealth() == 0) {
		gState->changeGameLose();
	}
}

void Player::wallHasCollided(){
	int halfWidth = (m_gConsts->getBoardWidth() - 1) / 2;
	int halfHeight = (m_gConsts->getBoardHeight() - 1) / 2;

	float tempX = MathsHelper::Clamp(m_position.x, -halfWidth, halfWidth);
	float tempZ = MathsHelper::Clamp(m_position.z, -halfHeight, halfHeight);
	m_position = Vector3(tempX, 0, tempZ);
}

void Player::enemyHasCollided(){
	player1->changeHealth(-100);
	gState->changeGameLose();
}

void Player::healthHasCollided(){
	player1->changeHealth(m_gConsts->getHealAmount());
	m_audio->Play("Assets/Sounds/Health.wav", false);
}

void Player::teleportHasCollided(Tile* teleport){
	m_audio->Play("Assets/Sounds/Teleport.wav", false);
	teleport->setType("blank", NULL);

	m_position = Vector3(teleport->getLink()->getPosX(), 0.0, teleport->getLink()->getPosY());
	teleport->getLink()->setType("blank", NULL);
}
