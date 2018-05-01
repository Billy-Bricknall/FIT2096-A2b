#include "FirstPersonCamera.h"



FirstPersonCamera::FirstPersonCamera(){
	m_objectAtCamera = NULL;
}

FirstPersonCamera::FirstPersonCamera(GameObject * target){
	m_objectAtCamera = target;
	m_offset = Vector3(0, 0.6, 0);
}

FirstPersonCamera::~FirstPersonCamera(){
	delete m_objectAtCamera;
}

void FirstPersonCamera::Update(float timestep){
	if (m_objectAtCamera != NULL) {

		SetPosition(m_objectAtCamera->GetPosition() + m_offset); //sets camera position
		Matrix yRotation = Matrix::CreateRotationY(m_objectAtCamera->GetYRotation());
		Matrix xRotation = Matrix::CreateRotationX(m_objectAtCamera->GetXRotation());
		Vector3 localForward = Vector3(0, 0, 1);
		Vector3 lookAt = Vector3::TransformNormal(localForward, xRotation * yRotation);
		lookAt += m_objectAtCamera->GetPosition();

		SetLookAt(lookAt);
	}
	Camera::Update(timestep);
}