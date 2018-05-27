#include "Bullet.h"

Bullet::Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation, AudioSystem* audio, GameConstants* gConsts) : GameObject(mesh, shader, position, texture) {
	trajectory = Vector3(0, 0, 1); //holds local forward for 1 calculation
	trajectory = Vector3::TransformNormal(trajectory, Matrix::CreateRotationY(rotation)); //calculates bullet trajectory based on rotation
	startPos = position;
	moveSpeed = 3;
	SetUniformScale(position.y / 1.2); //makes the bullet smaller for smaller enemy meshes
	SetYRotation(rotation); //rotations bullet to match trajectory
	m_audio = audio;
	m_gConsts = gConsts;
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
	bulletDamage = MathsHelper::RandomRange(m_gConsts->getMinBulletDamage(), m_gConsts->getMaxBulletDamage());

	int randNum = rand() % 6 + 1;
	switch (randNum) {
	case 1:
		bulletSound = m_audio->Play("Assets/Sounds/Pew1.wav", false);
		break;
	case 2:
		bulletSound = m_audio->Play("Assets/Sounds/Pew2.wav", false);
		break;
	case 3:
		bulletSound = m_audio->Play("Assets/Sounds/Pew3.wav", false);
		break;
	case 4:
		bulletSound = m_audio->Play("Assets/Sounds/Pew4.wav", false);
		break;
	case 5:
		bulletSound = m_audio->Play("Assets/Sounds/Pew5.wav", false);
		break;
	case 6:
		bulletSound = m_audio->Play("Assets/Sounds/Pew6.wav", false);
		break;
	}

	bulletSound->SetVolume(m_gConsts->getBulletVolume());
}

Bullet::Bullet(Mesh * mesh, Shader * shader, Vector3 position, Texture * texture, float yRotation, float xRotation, AudioSystem* audio, GameConstants* gConsts) : GameObject(mesh, shader, position, texture){
	trajectory = Vector3(0, 0, 1); //holds local forward for 1 calculation
	Matrix rotation = Matrix::CreateRotationX(xRotation) * Matrix::CreateRotationY(yRotation);
	trajectory = Vector3::TransformNormal(trajectory, rotation); //calculates bullet trajectory based on rotation
	moveSpeed = 3;
	SetUniformScale(position.y / 1.2); //makes the bullet smaller for smaller enemy meshes
	SetYRotation(yRotation); //rotations bullet to match trajectory
	SetXRotation(xRotation);
	m_audio = audio;
	m_gConsts = gConsts;
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
	bulletDamage = MathsHelper::RandomRange(m_gConsts->getMinBulletDamage(), m_gConsts->getMaxBulletDamage());

	int randNum = rand() % 6 + 1;
	switch (randNum) {
	case 1:
		bulletSound = m_audio->Play("Assets/Sounds/Pew1.wav", false);
		break;
	case 2:
		bulletSound = m_audio->Play("Assets/Sounds/Pew2.wav", false);
		break;
	case 3:
		bulletSound = m_audio->Play("Assets/Sounds/Pew3.wav", false);
		break;
	case 4:
		bulletSound = m_audio->Play("Assets/Sounds/Pew4.wav", false);
		break;
	case 5:
		bulletSound = m_audio->Play("Assets/Sounds/Pew5.wav", false);
		break;
	case 6:
		bulletSound = m_audio->Play("Assets/Sounds/Pew6.wav", false);
		break;
	}

	bulletSound->SetVolume(m_gConsts->getBulletVolume());
}

float Bullet::getDistanceTravelled(){
	return distanceTravelled;
}

int Bullet::getBulletDamage(){
	return bulletDamage;
}

bool Bullet::getHasHit(){
	return hasHit;
}

CBoundingBox Bullet::getBounds(){
	return m_boundingBox;
}

void Bullet::hasCollided(){
	hasHit = true;
	m_mesh = NULL;
}

void Bullet::update(float timestep){
	m_position = m_position + trajectory * timestep * moveSpeed; //moves bullet along trajectory
	distanceTravelled = Vector3::Distance(startPos, m_position);
	if (!hasHit) {
		m_boundingBox.SetMin(m_position + m_mesh->GetMin());
		m_boundingBox.SetMax(m_position + m_mesh->GetMax());
	}
}
