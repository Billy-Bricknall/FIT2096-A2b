#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet: public GameObject
{
private:
	Vector3 Trajectory;
	bool hasHit;
	float moveSpeed;
public:
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation);

	void update(float timestep);
};

#endif