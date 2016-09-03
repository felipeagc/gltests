#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glxew.h>
#include <iostream>
#include <string>
#include "resource.h"

class Texture : public Resource
{
public:
	Texture(const std::string &fileName);

	GLuint &GetId();
	std::string &GetPath();

	~Texture();
private:
	GLuint m_id;
	std::string m_path;
};

#endif // TEXTURE_H
