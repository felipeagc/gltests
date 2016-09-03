#ifndef ENGINE_H
#define ENGINE_H

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
#include "game.h"

using namespace std;

#define SENS 0.1f

#define RUN_GAME(game) int main() \
{\
	return Engine::Instance().Run(new game());\
}

class Engine
{
public:
	Engine();
	int Run(Game *game);

	void SetScreenShader(Shader *shader);
	int GetWidth();
	int GetHeight();
	glm::vec2 GetCursorPos();
	glm::vec2 GetCursorDeltaPos();

	static Engine &Instance() {
		static Engine *instance = new Engine();
		return *instance;
	}
	ResourceManager &GetResources() {
		return m_resources;
	}


protected:
	GLFWwindow* m_window;


	int m_prevWidth, m_prevHeight;
	double m_prevXPos, m_prevYPos;
	double m_deltaX, m_deltaY;

	double m_lastTime;

	int m_width = 1280;
	int m_height = 720;

	GLuint m_FBO;
	GLuint m_screenTexture;
	GLuint m_RBO;
	GLuint m_screenVAO;
	GLuint m_screenVBO;
	Shader* m_screenShader;

	ResourceManager m_resources;

	void UpdateRenderBuffer();

};

#endif // ENGINE_H
