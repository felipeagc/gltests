#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "model.h"
#include "resourcemanager.h"

using namespace std;

#define WIDTH 1280
#define HEIGHT 720
#define SENS 0.1f

class Model;

class Game
{
public:
	Game();
	void Load();
	void Update(float delta);
	void Quit();
	int Run();
	static Game &Instance() {
		static Game *instance = new Game();
		return *instance;
	}
	ResourceManager &GetResources() {
		return m_resources;
	}

private:
	GLFWwindow* m_window;

	int prevWidth, prevHeight;
	double prevXPos, prevYPos;
	double xPos, yPos;
	double deltaX, deltaY;

	GLuint m_FBO;
	GLuint m_screenTexture;
	GLuint m_RBO;
	GLuint m_screenVAO;
	GLuint m_screenVBO;
	Shader* m_screenShader;

	ResourceManager m_resources;
	Camera* m_camera;
	Model* m_model;
	Transform m_transform;

};

#endif // GAME_H
