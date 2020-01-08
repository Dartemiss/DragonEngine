#ifndef __SceneImporter_H__
#define __SceneImporter_H__

#include "MyImporter.h"

class ModelImporter;
class MeshImporter;
class MaterialImporter;

struct ModelData;
struct MeshData;
struct Texture;

class SceneImporter : public MyImporter
{
public:
	SceneImporter();
	~SceneImporter();

	bool ImportModel(const char* path, const char* file, std::string& output_file);
	bool ImportMesh(const char* file, const MeshData& mesh, std::string& output_file);
	bool ImportMaterial(const char* path, const char* file, std::string& output_file);

	bool ImportModel(const char* file, const void* buffer, unsigned int size, std::string& output_file);
	bool ImportMesh(const char* file, const void* buffer, unsigned int size, std::string& output_file);
	bool ImportMaterial(const char* file, const void* buffer, unsigned int size, std::string& output_file);

	bool LoadModel(const char* exported_file, ModelData & model);
	bool LoadMesh(const char* exported_file, MeshData & mesh);
	bool LoadMaterial(const char* exported_file, Texture & material);

private:
	ModelImporter * modelImp;
	MeshImporter * meshImp;
	MaterialImporter * materialImp;
};

extern SceneImporter* Importer;

#endif __SceneImporter_H__