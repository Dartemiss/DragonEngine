#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 glColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 colorGrid;

void main()
{
	gl_Position = proj*view*model*vec4(vertex_position, 1.0);
	colorGrid = glColor;
}