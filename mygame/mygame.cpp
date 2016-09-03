#include "mygame.h"

MyGame::MyGame() : m_engine(&Engine::Instance())
{

}

void MyGame::Load()
{
	m_engine->SetScreenShader(m_engine->GetResources().Get<Shader>("../assets/screenshader"));

	m_model = m_engine->GetResources().Get<Model>("../assets/nanosuit/nanosuit.obj");
	m_camera = new Camera(glm::vec3(0, 10, -10), glm::radians(70.0f),
						  m_engine->GetWidth() / m_engine->GetHeight(), 0.01f, 300.0f);
	m_camera->SetYaw(90.0f);
}

void MyGame::Update(float delta)
{

	m_camera->SetYaw(m_camera->GetYaw() - m_engine->GetCursorDeltaPos().x*SENS);
	m_camera->SetPitch(m_camera->GetPitch() + m_engine->GetCursorDeltaPos().y*SENS);

	m_transform.SetRot(glm::vec3(0, glfwGetTime(), 0));

	m_camera->Update();

	m_model->Draw(*m_engine->GetResources().Get<Shader>("../assets/passthrough"), m_camera->GetViewProjection() * m_transform.GetModel());
}

void MyGame::Quit()
{

}

RUN_GAME(MyGame);
