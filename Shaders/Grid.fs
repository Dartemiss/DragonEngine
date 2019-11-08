#version 330 core

out vec4 color;

in vec3 colorGrid;

void main()
{
	color = vec4(colorGrid,1.0f);
}
