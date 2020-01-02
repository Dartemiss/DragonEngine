#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "MyImporter.h"

struct MeshData
{
	unsigned int num_indices;
	unsigned int num_vertices;

	unsigned int * indices;
	float * positions;
	float * normals;
	float * texture_coords;
};

class MeshImporter : public MyImporter
{
public:
	bool Save(const char* file, const MeshData & mesh, std::string& output_file);
	bool Load(const char* exported_file, MeshData & mesh);
};

#endif __MeshImporter_H__