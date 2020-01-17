#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D myTexture;

void main()
{
	color = texture2D(myTexture, texCoord);
}
