#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string &fileName)
{
	m_path = fileName;
	int width, height, numComponents;

	unsigned char *imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if(imageData == NULL)
		std::cerr << "Texture loading failed for texture: " << fileName << std::endl;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

GLuint &Texture::GetId()
{
	return m_id;
}

std::string &Texture::GetPath()
{
	return m_path;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

