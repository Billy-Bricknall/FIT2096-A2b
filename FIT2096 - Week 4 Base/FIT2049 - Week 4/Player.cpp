#include "Player.h"
#include "MathsHelper.h"


Player::Player(Vector3 position, InputController* newInput) : GameObject(NULL, NULL, position){
	m_input = newInput;
	m_moveSpeed = 5;
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

	Vector3 forward; //calculated which tile is infront of the player
	if (m_rotY < 0.785) {
		forward = Vector3(0.0f, 0.0f, 1.0f);
	}
	else if (m_rotY < 2.355) {
		forward = Vector3(1.0f, 0.0f, 0.0f);
	}
	else if (m_rotY < 3.925) {
		forward = Vector3(0.0f, 0.0f, -1.0f);
	}
	else if(m_rotY < 5.495){
		forward = Vector3(-1.0f, 0.0f, 0.0f);
	}
	else {
		forward = Vector3(0.0f, 0.0f, 1.0f);
	}
	Vector3 right = Vector3(0.0f, 1.0f, 0.0f).Cross(forward); //uses the infront vector and up vector to calculate right

	if (!isMoving) {
		if (m_input->GetKeyUp('W')) {
			targetPos = Vector3(posX, 0.0f, posZ) + forward;
			isMoving = true;
		}
		else if (m_input->GetKeyUp('A')) {
			targetPos = Vector3(posX, 0.0f, posZ) - right;
			isMoving = true;
		}
		else if (m_input->GetKeyUp('S')) {
			targetPos = Vector3(posX, 0.0f, posZ) - forward;
			isMoving = true;
		}
		else if (m_input->GetKeyUp('D')) {
			targetPos = Vector3(posX, 0.0f, posZ) + right;
			isMoving = true;
		}
		if (targetPos.x < -7 || targetPos.x > 7 || targetPos.z < -7 || targetPos.z > 7 || !gBoard[(int)targetPos.x + 7][(int)targetPos.z + 7]->getActive()) {
			targetPos = Vector3(posX, 0.0f, posZ); //if target is not possible
		}
	}

	Vector3 diff = targetPos - m_position; //gets vector to move
	diff.Normalize();
	if (Vector3::DistanceSquared(targetPos, m_position) > 0.001)
		m_position +=  diff  * m_moveSpeed * timestep;
	else{
		isMoving = false;
		posX = nearbyint(m_position.x); //stops posX from being incorrect and allows tiles at posX to be activated only when Player is actually there
		posZ = nearbyint(m_position.z);
		targetPos = Vector3(posX, 0.0f, posZ); //helps stop target innacuracy from floats

		if (gBoard[posX + 7][posZ + 7]->getType() != "blank" && gBoard[posX + 7][posZ + 7]->getActive() == true) //if further action is required
			actionTile(gBoard); //performs all special tiles traits

		gBoard[posX + 7][posZ + 7]->deactivate(); //deactivates current tile

		bool left, Right, up, down; //following checks if player is trapped
		left = Right = up = down = true;
		if (posX + 6 < 0 || !gBoard[posX + 6][posZ + 7]->getActive()) {
			left = false;
		}
		if (posX + 8 > 14 || !gBoard[posX + 8][posZ + 7]->getActive()) {
			Right = false;
		}
		if (posZ + 6 < 0 || !gBoard[posX + 7][posZ + 6]->getActive()) {
			up = false;
		}
		if (posZ + 8 > 14 || !gBoard[posX + 7][posZ + 8]->getActive()) {
			down = false;
		}
		if (!left && !Right && !up && !down) { //if surrounding tiles cant be moved to
			gState->changeGameLose();
		}
	}
}

Vector3 Player::getPosition(){
	return m_position;
}

GameState * Player::getGamestate(){
	return gState;
}

void Player::actionTile(Tile* gBoard[15][15]){
	if (gBoard[posX + 7][posZ + 7]->getType() == "tele") { //if tile is teleport
		gBoard[posX + 7][posZ + 7]->deactivate(); //deactivate tile early as next deactivation will do tile teleported to
		int tempX = posX; //needed for getting new posY 2 lines down
		posX = gBoard[posX + 7][posZ + 7]->getLink()->getPosX();
		posZ = gBoard[tempX + 7][posZ + 7]->getLink()->getPosY();
		m_position = Vector3(posX, 0.0f, posZ); // sets new position
		targetPos = m_position; //resets target
		movementCue.erase(movementCue.begin(), movementCue.end()); //erases movement cue as player may change mind after teleport
	}
	else if (gBoard[posX + 7][posZ + 7]->getType() == "heal") {//if tile is heal
		player1->changeHealth(10); //heals player
	}
	else if (gBoard[posX + 7][posZ + 7]->getType() == "mon") {//if tile is monster
		do {
			player1->battle(gBoard[posX + 7][posZ + 7]->getEnemy()); //player attacks mon
			if (gBoard[posX + 7][posZ + 7]->getEnemy()->getHealth() > 0) { //if mon alive after attack
				gBoard[posX + 7][posZ + 7]->getEnemy()->battle(player1); } //mon attacks player
		} while (player1->getHealth() > 0 && gBoard[posX + 7][posZ + 7]->getEnemy()->getHealth() > 0); //while player or mon are awake
		if (player1->getHealth() > 0) { //if player won
			gState->addMonDef(gBoard[posX + 7][posZ + 7]->getEnemy()); //add mon to defeated list
			player1->levelUp(); //level up player
		}
		else {
			gState->changeGameLose(); //otherwise game is lost
		}
		
	}
	SetUniformScale(MathsHelper::RemapRange(player1->getHealth(), 0.0f, player1->getMaxHealth(), 0.02f, 0.1f)); // sets player scale relevent to player health
}
