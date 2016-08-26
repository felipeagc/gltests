#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <GL/glxew.h>
#include <string>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "resource.h"

class Shader : public Resource
{
public:
	Shader(const std::string &path);
	void Use();
	void Update(const glm::mat4 &transform);

	GLuint &GetProgram();
private:
	std::string LoadCode(const std::string &path);
	void CompileShaders(const GLchar* vertexCode, const GLchar* fragmentCode);
	GLuint m_program;

};

#endif // SHADER_H
