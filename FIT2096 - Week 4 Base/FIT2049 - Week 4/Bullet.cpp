#include "Bullet.h"
#include <iostream>
Bullet::Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation) : GameObject(mesh, shader, position, texture) {
	Trajectory = Vector3(0, 0, 1); //holds local forward for 1 calculation
	Trajectory = Vector3::TransformNormal(Trajectory, Matrix::CreateRotationY(rotation)); //calculates bullet trajectory based on rotation

	moveSpeed = 3;
	SetUniformScale(position.y / 1.2); //makes the bullet smaller for smaller enemy meshes
	SetYRotation(rotation); //rotations bullet to match trajectory
}

void Bullet::update(float timestep){
	m_position = m_position + Trajectory *timestep * moveSpeed; //moves bullet along trajectory
}
