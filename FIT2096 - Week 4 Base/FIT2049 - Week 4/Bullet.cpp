#include "Bullet.h"
#include <iostream>
Bullet::Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation) : GameObject(mesh, shader, position, texture) {
	trajectory = Vector3(0, 0, 1); //holds local forward for 1 calculation
	trajectory = Vector3::TransformNormal(trajectory, Matrix::CreateRotationY(rotation)); //calculates bullet trajectory based on rotation
	startPos = position;
	moveSpeed = 3;
	SetUniformScale(position.y / 1.2); //makes the bullet smaller for smaller enemy meshes
	SetYRotation(rotation); //rotations bullet to match trajectory
}

Bullet::Bullet(Mesh * mesh, Shader * shader, Vector3 position, Texture * texture, float yRotation, float xRotation) : GameObject(mesh, shader, position, texture){
	trajectory = Vector3(0, 0, 1); //holds local forward for 1 calculation
	Matrix rotation = Matrix::CreateRotationX(xRotation) * Matrix::CreateRotationY(yRotation);
	trajectory = Vector3::TransformNormal(trajectory, rotation); //calculates bullet trajectory based on rotation

	moveSpeed = 3;
	SetUniformScale(position.y / 1.2); //makes the bullet smaller for smaller enemy meshes
	SetYRotation(yRotation); //rotations bullet to match trajectory
	SetXRotation(xRotation);
}

float Bullet::getDistanceTravelled(){
	return distanceTravelled;
}

void Bullet::update(float timestep){
	m_position = m_position + trajectory *timestep * moveSpeed; //moves bullet along trajectory
	distanceTravelled = Vector3::Distance(startPos, m_position);
}
