#include "game.h"

Game::Game()
{
}

void Game::Load()
{
	m_model = m_resources.Get<Model>("../assets/nanosuit/nanosuit.obj");
	m_camera = new Camera(glm::vec3(0, 10, -10), glm::radians(70.0f),
						  (float) WIDTH / (float) HEIGHT, 0.01f, 300.0f);
	m_camera->SetYaw(90.0f);
}

void Game::Update(float delta)
{
	{
		glfwGetCursorPos(m_window, &xPos, &yPos);

		deltaX = prevXPos - xPos;
		deltaY = prevYPos - yPos;

		prevXPos = xPos;
		prevYPos = yPos;
	}

	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		if(width != prevWidth && height != prevHeight)
		{
			glViewport(0, 0, width, height);
			m_camera->UpdateAspect(width, height);
		}
		prevWidth = width;
		prevHeight = height;
	}

	//m_camera->SetYaw(m_camera->GetYaw() - deltaX*SENS);
	//m_camera->SetPitch(m_camera->GetPitch() + deltaY*SENS);

	m_transform.SetRot(glm::vec3(0, glfwGetTime(), 0));

	m_camera->Update();

	Shader *shader = m_resources.Get<Shader>("../assets/passthrough");
	shader->Update(m_camera->GetViewProjection() * m_transform.GetModel());
	m_model->Draw(*shader);
}

void Game::Quit()
{

}

int Game::Run()
{
	/* Initialize window */
	if (!glfwInit())
	{
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL tests", NULL, NULL);
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
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	Load();

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Render Buffer Object */
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer object failed!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_screenShader = m_resources.Get<Shader>("../assets/screenshader");

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		Update(0);

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

	Quit();

	glDeleteFramebuffers(1, &m_FBO);
	glfwTerminate();

	return 0;
}
