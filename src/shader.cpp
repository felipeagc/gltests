#include "shader.h"

Shader::Shader(const std::string &path)
{
	std::string vertexCode = LoadCode(path + ".vert");
	std::string fragmentCode = LoadCode(path + ".frag");

	CompileShaders(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::Use()
{
	glUseProgram(m_program);
}

void Shader::Update(const glm::mat4 &transform)
{
	GLuint transformLocation = glGetUniformLocation(m_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::CompileShaders(const GLchar* vertexCode, const GLchar* fragmentCode)
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	m_program = glCreateProgram();
	glAttachShader(m_program, vertex);
	glAttachShader(m_program, fragment);
	glLinkProgram(m_program);
	// Print linking errors if any
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string Shader::LoadCode(const std::string &path)
{
	std::string code;
	std::ifstream file;

	file.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		file.open(path);
		std::stringstream shaderStream, fShaderStream;
		// Read file's buffer contents into streams
		shaderStream << file.rdbuf();
		// close file handlers
		file.close();
		// Convert stream into GLchar array
		code = shaderStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return code;
}

GLuint &Shader::GetProgram()
{
	return m_program;
}
