#include "model.h"


Model::Model(const std::string &fileName)
{
	LoadModel(fileName);
}

Model::~Model()
{
	for (Mesh *mesh : m_meshes)
	{
		delete mesh;
	}
}

void Model::Draw(Shader &shader)
{
	for (Mesh *mesh : m_meshes)
	{
		mesh->Draw(shader);
	}
}

void Model::LoadModel(const std::string &path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh *Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture*> textures;

	for(GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		glm::vec3 normals = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		glm::vec2 texCoords = glm::vec2();

		if (mesh->mTextureCoords[0])
		{
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
		}

		Vertex vertex(position, normals, texCoords);
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture*> textureMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), textureMaps.begin(), textureMaps.end());
	}

	return new Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial *material, aiTextureType type)
{
	std::vector<Texture*> textures;

	for (GLuint i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

			Texture *texture = Game::Instance().GetResources().Get<Texture>(m_directory + "/" + str.C_Str());
			textures.push_back(texture);
	}

	return textures;
}
