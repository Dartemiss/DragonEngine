#ifndef __ModuleModelLoader_h__
#define __ModuleModelLoader_h__

#include "Globals.h"
#include "Module.h"
#include <vector>
#include <map>
#include <string>
#include "GL/glew.h"
#include "Mesh.h"

struct Texture;
struct MeshData;

struct Model
{
	std::map<Mesh*, Texture*> Meshes;
	std::string Name = "";
	//TODO: add count of similar models to track delete and remove data?
};

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader() = default;
	~ModuleModelLoader() = default;

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadModel(const std::string &path, Model& model);

	const int GetNumberOfMeshes() const;
	/*bool LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCylinder(const char* name, const math::float3& pos, const math::Quat& rot, float height,
		float radius, unsigned slices, unsigned stacks, const math::float4& color);
	
	bool LoadTorus(const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r,
		unsigned slices, unsigned stacks, const math::float4& color);

	bool LoadCube(const char* name, const math::float3& pos, const math::Quat& rot, float size, const math::float4& color);*/
	

	float3 correctCameraPositionForModel = float3(0.0f, 0.0f, 0.0f);
	float3 modelCenter = float3(0.0f, 0.0f, 0.0f);

	std::vector<Model> models;
	std::string ComputeName(const std::string &path) const;

private:

	/*  Functions   */
	void ProcessMeshData(const MeshData & data, Mesh & mesh);
};

#endif __ModuleModelLoader_h__