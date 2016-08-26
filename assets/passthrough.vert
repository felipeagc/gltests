#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
out vec3 position0;
out vec2 texCoords0;

uniform mat4 transform;

void main()
{
	position0 = position;
	texCoords0 = texCoords * vec2(1, -1);
	gl_Position = transform * vec4(position0, 1.0f);
}
