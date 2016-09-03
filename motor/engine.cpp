#include "engine.h"

bool windowSizeChanged;

Engine::Engine()
{

}

int Engine::Run(Game *game)
{
	/* Initialize window */
	if (!glfwInit())
	{
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(m_width, m_height, "OpenGL tests", NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		std::cout << "window doesnt exist!" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	/* Initialize GLEW */
	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
		return 1;
	}

	glDepthFunc(GL_LESS);
	glViewport(0, 0, m_width, m_height);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	game->Load();

	/*
	 * Setup the screen quad
	 */
	GLfloat quadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVBO);
	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	/* Frame Buffer Object */
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	/* Texture */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_screenTexture);
	glBindTexture(GL_TEXTURE_2D, m_screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Render Buffer Object */
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer object failed!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glfwSetWindowSizeCallback(m_window, [] (GLFWwindow* window, int width, int height)
	{
		windowSizeChanged = true;
	});
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		glfwGetWindowSize(m_window, &m_width, &m_height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		{
			/*
			 * Update the resolution
			 */
			if (windowSizeChanged)
			{
				glViewport(0, 0, m_width, m_height);
				UpdateRenderBuffer();
				windowSizeChanged = false;
			}
		}

		{
			/*
			 * Update the delta cursor position
			 */
			double xPos, yPos;
			glfwGetCursorPos(m_window, &xPos, &yPos);

			m_deltaX = m_prevXPos - xPos;
			m_deltaY = m_prevYPos - yPos;

			m_prevXPos = xPos;
			m_prevYPos = yPos;
		}

		glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		game->Update(glfwGetTime() - m_lastTime);
		m_lastTime = glfwGetTime();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		glBindVertexArray(m_screenVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_screenTexture);
			m_screenShader->Use();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(m_window);

	}

	game->Quit();

	glDeleteFramebuffers(1, &m_FBO);
	glfwTerminate();

	return 0;
}

void Engine::UpdateRenderBuffer()
{
	glBindTexture(GL_TEXTURE_2D, m_screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glDeleteRenderbuffers(1, &m_RBO);

	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Engine::SetScreenShader(Shader *shader)
{
	m_screenShader = shader;
}

int Engine::GetWidth()
{
	return m_width;
}

int Engine::GetHeight()
{
	return m_height;
}

glm::vec2 Engine::GetCursorPos()
{
	glm::ivec2 result;
	glfwGetCursorPos(m_window, (double*) &result.x, (double*) &result.y);
	return result;
}

glm::vec2 Engine::GetCursorDeltaPos()
{
	return glm::vec2(m_deltaX, m_deltaY);
}
