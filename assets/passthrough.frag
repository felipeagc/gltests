#version 330
in vec3 position0;
in vec2 texCoords0;

uniform sampler2D texture0;

void main()
{
	gl_FragColor = texture2D(texture0, texCoords0);
}
