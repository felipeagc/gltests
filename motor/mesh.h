#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include "resource.h"
#include "engine.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures);
	~Mesh();

	void Draw(Shader &shader, const glm::mat4 transform);
private:
	void SetupMesh();

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture*> m_textures;


	GLuint m_VAO, m_VBO, m_EBO;
};

#endif // MESH_H
