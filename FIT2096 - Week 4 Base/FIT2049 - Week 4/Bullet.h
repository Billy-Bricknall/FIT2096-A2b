#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
#include "AudioSystem.h"
#include "GameConstants.h"

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
public:
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float rotation, AudioSystem* audio, GameConstants* gConsts);
	Bullet(Mesh* mesh, Shader* shader, Vector3 position, Texture* texture, float xRrotation, float yRotation, AudioSystem* audio, GameConstants* gConsts);
	float getDistanceTravelled();

	void update(float timestep);
};

#endif