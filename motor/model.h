#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "resource.h"
#include "engine.h"

class Mesh;

class Model : public Resource
{
public:
	Model(const std::string &fileName);
	void Draw(Shader &shader, const glm::mat4 transform);

	~Model();

private:
	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	void LoadModel(const std::string &path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh *ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial *material, aiTextureType type);
};

#endif // MODEL_H
