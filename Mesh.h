#ifndef __Mesh_H__
#define __Mesh_H__

#include "Dependencies/MathGeoLib/include/Math/float3.h"
#include "Dependencies/MathGeoLib/include/Math/float2.h"
#include <string>
#include <vector>

struct Texture;

struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};
class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//TODO remove textures from mesh
	std::vector<Texture> textures;

	/*  Functions  */
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	~Mesh();
	void Draw(const unsigned int program) const;

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;

	/*  Functions    */
	void setupMesh();
};
#endif __Mesh_H__
