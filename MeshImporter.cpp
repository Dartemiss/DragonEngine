#include "MeshImporter.h"

#include "Application.h"
#include "ModuleFilesystem.h"

using namespace std;

bool MeshImporter::Import(const char * file, const MeshData & mesh, string & output_file)
{
	unsigned int ranges[2] = { mesh.num_indices, mesh.num_vertices };

	unsigned int size = sizeof(ranges)					//ranges
		+ sizeof(unsigned int) * mesh.num_indices		//indices
		+ sizeof(float) * mesh.num_vertices * 3			//vertex positions
		+ sizeof(float) * mesh.num_vertices * 3			//vertex normals
		+ sizeof(float) * mesh.num_vertices * 2;		//vertex texture coord
	//	+ sizeof(float) * 6;							//AABB
	//TODO: add AABB and color to save and to load

	char* data = new char[size]; // Allocate
	char* cursor = data;

	unsigned int bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(unsigned int) * mesh.num_indices;
	memcpy(cursor, mesh.indices, bytes);

	cursor += bytes; // Store vertex positions
	bytes = sizeof(float) * mesh.num_vertices * 3;
	memcpy(cursor, mesh.positions, bytes);

	cursor += bytes; // Store vertex normals
	bytes = sizeof(float) * mesh.num_vertices * 3;
	memcpy(cursor, mesh.normals, bytes);

	cursor += bytes; // Store vertex texture coords
	bytes = sizeof(float) * mesh.num_vertices * 2;
	memcpy(cursor, mesh.texture_coords, bytes);

	return Import(file, data, size, output_file);
}

bool MeshImporter::Import(const char * file, const void * buffer, unsigned int size, std::string & output_file)
{
	if (!App->filesystem->IsDirectory("../Library"))
		App->filesystem->MakeDirectory("../Library");
	if (!App->filesystem->IsDirectory("../Library/Meshes"))
		App->filesystem->MakeDirectory("../Library/Meshes");

	string filename = file; filename += ".mesh";
	output_file = file;

	return App->filesystem->Save("../Library/Meshes/", filename.c_str(), buffer, size, false);
}

bool MeshImporter::Load(const char * exported_file, MeshData & mesh)
{
	char* buffer;
	string mesh_file = exported_file; mesh_file += ".mesh";

	if (!App->filesystem->Load("../Library/Meshes/", mesh_file.c_str(), &buffer))
		return false;

	char* cursor = buffer;

	unsigned int ranges[2]; //Load ranges
	unsigned int bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh.num_indices = ranges[0]; 
	mesh.num_vertices = ranges[1];

	cursor += bytes; // Load indices
	bytes = sizeof(unsigned int) * mesh.num_indices;
	mesh.indices = new unsigned int[mesh.num_indices];
	memcpy(mesh.indices, cursor, bytes);
	
	cursor += bytes; // Load vertex positions
	bytes = sizeof(float) * mesh.num_vertices * 3;
	mesh.positions = new float[mesh.num_vertices * 3];
	memcpy(mesh.positions, cursor, bytes);
	
	cursor += bytes; // Load vertex normals
	bytes = sizeof(float) * mesh.num_vertices * 3;
	mesh.normals = new float[mesh.num_vertices * 3];
	memcpy(mesh.normals, cursor, bytes);
	
	cursor += bytes; // Load vertex texture coords
	bytes = sizeof(float) * mesh.num_vertices * 2;
	mesh.texture_coords = new float[mesh.num_vertices * 2];
	memcpy(mesh.texture_coords, cursor, bytes);

	return true;
}
