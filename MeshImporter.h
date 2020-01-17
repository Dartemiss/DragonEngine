#ifndef __MeshImporter_H__
#define __MeshImporter_H__

#include "MyImporter.h"

struct MeshData
{
	unsigned int num_indices;
	unsigned int num_vertices;

	unsigned int * indices = nullptr;
	float * positions = nullptr;
	float * normals = nullptr;
	float * texture_coords = nullptr;

	std::string name;
};

class MeshImporter : public MyImporter
{
public:
	bool Import(const char* file, const MeshData & mesh, std::string& output_file);
	bool Import(const char* file, const void* buffer, unsigned int size, std::string& output_file);
	bool Load(const char* exported_file, MeshData & mesh);
};

#endif __MeshImporter_H__