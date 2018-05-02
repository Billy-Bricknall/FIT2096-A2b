#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet: public GameObject
{
private:
	Vector3 trajectory;
	Vector3 startPos;
	bool hasHit;
	float moveSpeed;
	float distanceTravelled;
public:
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation);
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float xRrotation, float yRotation);
	float getDistanceTravelled();

	void update(float timestep);
};

#endif