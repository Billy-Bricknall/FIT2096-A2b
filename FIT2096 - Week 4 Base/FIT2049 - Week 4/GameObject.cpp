#include "GameObject.h"

GameObject::GameObject(Mesh * mesh, Shader * shader, Vector3 position){
	m_mesh = mesh;
	m_shader = shader;
	m_input = NULL;
	m_position = position;
	m_texture = NULL;
	SetUniformScale(1.0f);
}

GameObject::GameObject(Mesh * mesh, Shader * shader, Vector3 position, Texture* texture) {
	m_mesh = mesh;
	m_shader = shader;
	m_input = NULL;
	m_position = position;
	m_texture = texture;
	SetUniformScale(1.0f);
}


GameObject::~GameObject(){

}

void GameObject::Update(float timestep){
}

void GameObject::Render(Direct3D * renderer, Camera * cam){
	if (m_mesh)
	{
		m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateRotationX(m_rotX) * Matrix::CreateRotationY(m_rotY) * Matrix::CreateRotationZ(m_rotZ) * Matrix::CreateTranslation(m_position);
		m_mesh->Render(renderer, m_shader, m_world, cam, m_texture);
	}
}

