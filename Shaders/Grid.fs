#version 330 core

out vec4 color;

in vec4 colorGrid;

uniform sampler2D texture0;

void main()
{
	color = colorGrid;
}
