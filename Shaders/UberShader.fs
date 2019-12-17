#version 430 core

struct Material
{
    sampler2D diffuse_map;
    vec4 diffuse_color;
};


uniform Material material;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 color;

void main()
{
    color = texture(material.diffuse_map, texCoord);
    //color = material.diffuse_color;
}