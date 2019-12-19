#ifndef __Mesh_H__
#define __Mesh_H__

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float2.h"
#include <vector>

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

	/*  Functions  */
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();
	void Draw(const unsigned int program) const;

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;

	/*  Functions    */
	void setupMesh();
};
#endif __Mesh_H__
