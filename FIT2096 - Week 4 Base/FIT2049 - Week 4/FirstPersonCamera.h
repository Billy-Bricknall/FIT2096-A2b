#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H

#include "Camera.h"
#include "GameObject.h"

class FirstPersonCamera : public Camera
{
private:
	GameObject * m_objectAtCamera;
	Vector3 m_offset;

public:
	FirstPersonCamera();
	FirstPersonCamera(GameObject* target);
	~FirstPersonCamera();

	void Update(float timestep);

};

#endif