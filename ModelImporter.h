#ifndef __ModelImporter_H__
#define __ModelImporter_H__

#include "MyImporter.h"
#include <vector>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

enum aiTextureType;

struct MeshTexPair
{
	unsigned int mesh;
	unsigned int tex;
};

struct ModelData
{
	std::vector<std::string> meshes;
	std::vector<std::string> textures;

	std::vector<MeshTexPair> pairs;
};

class ModelImporter : public MyImporter
{
public:
	bool Import(const char* path, const char* file, std::string& output_file);
	bool Import(const char* file, const void* buffer, unsigned int size, std::string& output_file);
	bool Load(const char* exported_file, ModelData & model);

private:
	void ProcessNode(aiNode * node, const aiScene * scene);
	void ProcessMesh(const aiMesh * mesh, const aiScene * scene);

	void SearchTextureDir(const std::string & fbxDir, const std::string & texDir, std::string & foundDir, std::string & texName);
	void SearchTextureByType(const aiMaterial* material, const aiTextureType texType, const unsigned int currentMeshCount, const std::string & typeName);
	void SaveModelFile(std::string & output_file);

	ModelData modelData;
	std::string modelName;
	std::string directory;
};

#endif __ModelImporter_H__