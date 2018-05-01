#include "Player.h"
#include "MathsHelper.h"


Player::Player(Vector3 position, InputController* newInput) : GameObject(NULL, NULL, position){
	m_input = newInput;
	m_moveSpeed = 3;
	player1 = new Character("player", 50, 2, 2);
	posX = 0;
	posZ = 0;
	gState = new GameState();
	m_heading = 0.0f;
	m_pitch = 0.0f;
	m_rotateSpeed = 1.0f;
	isMoving = false;
}

Player::~Player(){
	delete m_input;
	delete player1;
	delete gState;
}

void Player::update(float timestep, Tile * gBoard[15][15]){

	m_heading += m_input->GetMouseDeltaX() * m_rotateSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_rotateSpeed * timestep;
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f)); //avoids gimbal lock
	if (m_heading > 6.28) { m_heading = m_heading - 6.28; } //keeps heading within 2PI for forward calculation
	if (m_heading < 0) { m_heading = m_heading + 6.28; }
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

	
	
	
	posX = nearbyint(m_position.x); //stops posX from being incorrect and allows tiles at posX to be activated only when Player is actually there
	posZ = nearbyint(m_position.z);

	if (gBoard[posX + 7][posZ + 7]->getType() != "blank" && gBoard[posX + 7][posZ + 7]->getActive() == true) //if further action is required
		actionTile(gBoard); //performs all special tiles traits

	SetUniformScale(MathsHelper::RemapRange(player1->getHealth(), 0.0f, player1->getMaxHealth(), 0.02f, 0.1f)); // sets player scale relevent to player health
}

Vector3 Player::getPosition(){
	return m_position;
}

GameState * Player::getGamestate(){
	return gState;
}

void Player::actionTile(Tile* gBoard[15][15]){
	if (gBoard[posX + 7][posZ + 7]->getType() == "tele") { //if tile is teleport
		gBoard[posX + 7][posZ + 7]->setType("blank"); //deactivate tile early as next deactivation will do tile teleported to
		int tempX = posX; //needed for getting new posY 2 lines down
		posX = gBoard[posX + 7][posZ + 7]->getLink()->getPosX();
		posZ = gBoard[tempX + 7][posZ + 7]->getLink()->getPosY();
		m_position = Vector3(posX, 0.0f, posZ); // sets new position
		gBoard[posX + 7][posZ + 7]->setType("blank");
	}
}
