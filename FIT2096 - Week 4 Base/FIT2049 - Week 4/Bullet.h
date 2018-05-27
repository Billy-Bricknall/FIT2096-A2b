#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
#include "AudioSystem.h"
#include "GameConstants.h"
#include "CollisionManager.h"

class Bullet: public GameObject
{
private:
	Vector3 trajectory;
	Vector3 startPos;
	AudioSystem* m_audio;
	AudioClip* bulletSound;
	GameConstants* m_gConsts;
	bool hasHit;
	float moveSpeed;
	float distanceTravelled;
	int bulletDamage;
	CBoundingBox m_boundingBox;

public:
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation, AudioSystem* audio, GameConstants* gConsts);
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float xRrotation, float yRotation, AudioSystem* audio, GameConstants* gConsts);
	float getDistanceTravelled();
	int getBulletDamage();
	bool getHasHit();
	CBoundingBox getBounds();
	void hasCollided();

	void update(float timestep);
};

#endif