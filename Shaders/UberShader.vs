#version 330

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 textures;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main()
{
	gl_Position = proj * view * model * vec4(positions, 1.0);
	position = (model * vec4(positions, 1.0)).xyz;
	normal = (model * vec4(normals, 1.0)).xyz;
	texCoord = textures;
}
