#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "Timer.h"
#include "SceneImporter.h"
#include "ModelImporter.h"
#include "MeshImporter.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "Utils/par_shapes.h"

using namespace std;


bool ModuleModelLoader::Init()
{
	return true;
}

update_status ModuleModelLoader::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleModelLoader::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{

	//It is necessary?
	
	for(auto mod : models)
	{
		for(auto mesh : mod.Meshes)
		{
			delete mesh.first;
			delete mesh.second;
		}
	}
	models.clear();

	return true;
}


void ModuleModelLoader::LoadModel(const string &path, Model &model)
{
	model.Name = ComputeName(path);
	
	//Check if model is already loaded
	for (unsigned int i = 0; i < models.size(); i++)
	{
		if (model.Name == models[i].Name)
		{
			model = models[i];
			return;
		}
	}

	LOG("Importing model \n");
	ModelData modelData;
	Importer->LoadModel(path.c_str(), modelData);

	vector<Mesh> meshes;
	MeshData currentMeshData;
	vector<Texture> textures;
	
	for (int i = 0; i < modelData.meshes.size(); i++)
	{
		Mesh currentMesh;
		if (Importer->LoadMesh(modelData.meshes[i].c_str(), currentMeshData))
		{
			ProcessMeshData(currentMeshData, currentMesh);
			meshes.push_back(currentMesh);
		}
		else
			LOG("Error loading model mesh: %s.", modelData.meshes[i].c_str());

		delete[] currentMeshData.indices;
		delete[] currentMeshData.positions;
		delete[] currentMeshData.normals;
		delete[] currentMeshData.texture_coords;
	}

	for (int i = 0; i < modelData.textures.size(); i++)
	{
		Texture currentTexture;
		if (Importer->LoadMaterial(modelData.textures[i].c_str(), currentTexture))
		{
			App->texture->LoadTexture(currentTexture);
			textures.push_back(currentTexture);
		}
		else
			LOG("Error loading model texture: %s.", modelData.textures[i].c_str());
	}

	MeshTexPair pair;
	Mesh * newMesh;
	unsigned lastMeshIndex = 0;
	for (int i = 0; i < modelData.pairs.size(); i++)
	{
		pair = modelData.pairs[i];

		if (pair.mesh != lastMeshIndex)
		{
			newMesh = new Mesh();
			newMesh->vertices = meshes[pair.mesh - 1].vertices;
			newMesh->indices = meshes[pair.mesh - 1].indices;
			newMesh->name = meshes[pair.mesh - 1].name;
			newMesh->setupMesh();

			lastMeshIndex = pair.mesh;
		}

		Texture * newTex = new Texture();
		if (pair.tex == 0)
		{
			Texture fallback = App->texture->white_fallback;

			newTex->id = fallback.id;
			newTex->width = fallback.width;
			newTex->height = fallback.height;
			newTex->depth = fallback.depth;
			newTex->format = fallback.format;
			newTex->data = fallback.data;
			newTex->type = fallback.type;
			newTex->path = fallback.path;
		}

		else
		{
			newTex->id = textures[pair.tex - 1].id;
			newTex->width = textures[pair.tex - 1].width;
			newTex->height = textures[pair.tex - 1].height;
			newTex->depth = textures[pair.tex - 1].depth;
			newTex->format = textures[pair.tex - 1].format;
			newTex->data = textures[pair.tex - 1].data;
			newTex->type = textures[pair.tex - 1].type;
			newTex->path = textures[pair.tex - 1].path;
		}

		model.Meshes.emplace(newMesh, newTex);
	}

	models.push_back(model);
	return;
}

const int ModuleModelLoader::GetNumberOfMeshes() const
{
	//TODO change or delete
	return 0;
}

string ModuleModelLoader::ComputeName(const string & path) const
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
		return path.substr(path.find_last_of('/') + 1, path.size()-1);
	}
	size_t simpleLeftSlash = path.find_last_of('\\');
	if (string::npos != simpleLeftSlash)
	{
		LOG("Directory with simpleLeftSlashes.")
			return path.substr(path.find_last_of('\\') + 1, path.size() - 1);
	}
	size_t doubleRightSlash = path.find_last_of("//");
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
		return path.substr(path.find_last_of("//") + 1, path.size()-1);
	}

	size_t doubleLeftSlash = path.find_last_of("\\\\");
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
		return path.substr(path.find_last_of("\\\\") + 1,path.size() -1);
	}

	return path;
}

//bool ModuleModelLoader::LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
//	unsigned slices, unsigned stacks, const math::float4& color)
//{
//	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));
//
//	if (mesh)
//	{
//		par_shapes_scale(mesh, size, size, size);
//
//		//Filling data
//		vector<Vertex> vertices;
//		vector<unsigned int> indices;
//		vector<Texture> textures;
//
//
//		for (unsigned int i = 0; i < mesh->npoints; ++i)
//		{
//			Vertex vertex;
//			// process vertex positions, normals and texture coordinates
//			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
//			vertex.Position = positions;
//
//			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
//			vertex.Normal = normals;
//
//
//			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
//			{
//				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
//				vertex.TexCoords = texturesCoords;
//			}
//			else
//				vertex.TexCoords = float2(0.0f, 0.0f);
//
//			vertices.push_back(vertex);
//		}
//
//		for(int j = 0; j < mesh->ntriangles;++j)
//		{
//			indices.push_back(mesh->triangles[j * 3]);
//			indices.push_back(mesh->triangles[j * 3 + 1]);
//			indices.push_back(mesh->triangles[j * 3 + 2]);
//		}
//
//			
//		par_shapes_free_mesh(mesh);
//
//
//		//Mesh* sphere = new Mesh(vertices, indices, textures);
//		//meshes.push_back(sphere);
//
//		return true;
//	}
//
//	return false;
//}
//
//bool ModuleModelLoader::LoadCylinder(const char * name, const math::float3 & pos, const math::Quat & rot, float height, float radius, unsigned slices, unsigned stacks, const math::float4 & color)
//{
//	par_shapes_mesh* mesh = par_shapes_create_cylinder(int(slices), int(stacks));
//	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);
//
//	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
//	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);
//
//	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
//	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);
//
//	if(mesh)
//	{
//		par_shapes_scale(mesh, radius, height, radius);
//		par_shapes_merge_and_free(mesh, top);
//		par_shapes_merge_and_free(mesh, bottom);
//
//		//Filling data
//		vector<Vertex> vertices;
//		vector<unsigned int> indices;
//		vector<Texture> textures;
//
//
//		for (unsigned int i = 0; i < mesh->npoints; ++i)
//		{
//			Vertex vertex;
//			// process vertex positions, normals and texture coordinates
//			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
//			vertex.Position = positions;
//
//			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
//			vertex.Normal = normals;
//
//
//			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
//			{
//				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
//				vertex.TexCoords = texturesCoords;
//			}
//			else
//				vertex.TexCoords = float2(0.0f, 0.0f);
//
//			vertices.push_back(vertex);
//		}
//
//		for (int j = 0; j < mesh->ntriangles; ++j)
//		{
//			indices.push_back(mesh->triangles[j * 3]);
//			indices.push_back(mesh->triangles[j * 3 + 1]);
//			indices.push_back(mesh->triangles[j * 3 + 2]);
//		}
//
//
//		par_shapes_free_mesh(mesh);
//		//Mesh* cylinder = new Mesh(vertices, indices, textures);
//		//meshes.push_back(cylinder);
//
//		return true;
//	}
//
//	return false;
//}
//
//bool ModuleModelLoader::LoadTorus(const char * name, const math::float3 & pos, const math::Quat & rot, float inner_r, float outer_r, unsigned slices, unsigned stacks, const math::float4 & color)
//{
//	par_shapes_mesh* mesh = par_shapes_create_torus(int(slices), int(stacks), inner_r);
//
//	if(mesh)
//	{
//		par_shapes_scale(mesh, outer_r, outer_r, outer_r);
//		
//		//Filling data
//		vector<Vertex> vertices;
//		vector<unsigned int> indices;
//		vector<Texture> textures;
//
//
//		for (unsigned int i = 0; i < mesh->npoints; ++i)
//		{
//			Vertex vertex;
//			// process vertex positions, normals and texture coordinates
//			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
//			vertex.Position = positions;
//
//			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
//			vertex.Normal = normals;
//
//
//			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
//			{
//				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
//				vertex.TexCoords = texturesCoords;
//			}
//			else
//				vertex.TexCoords = float2(0.0f, 0.0f);
//
//			vertices.push_back(vertex);
//		}
//
//		for (int j = 0; j < mesh->ntriangles; ++j)
//		{
//			indices.push_back(mesh->triangles[j * 3]);
//			indices.push_back(mesh->triangles[j * 3 + 1]);
//			indices.push_back(mesh->triangles[j * 3 + 2]);
//		}
//
//		par_shapes_free_mesh(mesh);
//		//Mesh* torus = new Mesh(vertices, indices, textures);
//		//meshes.push_back(torus);
//
//		return true;
//	}
//
//	return false;
//}
//
//bool ModuleModelLoader::LoadCube(const char * name, const math::float3 & pos, const math::Quat & rot, float size, const math::float4 & color)
//{
//	par_shapes_mesh* mesh = par_shapes_create_plane(1, 1);
//	par_shapes_mesh* top = par_shapes_create_plane(1, 1);
//	par_shapes_mesh* bottom = par_shapes_create_plane(1, 1);
//	par_shapes_mesh* back = par_shapes_create_plane(1, 1);
//	par_shapes_mesh* left = par_shapes_create_plane(1, 1);
//	par_shapes_mesh* right = par_shapes_create_plane(1, 1);
//
//	par_shapes_translate(mesh, -0.5f, -0.5f, 0.5f);
//
//	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(top, -0.5f, 0.5f, 0.5f);
//
//	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
//	par_shapes_translate(bottom, -0.5f, -0.5f, -0.5f);
//
//	par_shapes_rotate(back, float(PAR_PI), (float*)&math::float3::unitX);
//	par_shapes_translate(back, -0.5f, 0.5f, -0.5f);
//
//	par_shapes_rotate(left, float(-PAR_PI * 0.5), (float*)&math::float3::unitY);
//	par_shapes_translate(left, -0.5f, -0.5f, -0.5f);
//
//	par_shapes_rotate(right, float(PAR_PI*0.5), (float*)&math::float3::unitY);
//	par_shapes_translate(right, 0.5f, -0.5f, 0.5f);
//
//	par_shapes_merge_and_free(mesh, top);
//	par_shapes_merge_and_free(mesh, bottom);
//	par_shapes_merge_and_free(mesh, back);
//	par_shapes_merge_and_free(mesh, left);
//	par_shapes_merge_and_free(mesh, right);
//
//	if (mesh)
//	{
//		par_shapes_scale(mesh, size, size, size);
//
//		//Filling data
//		vector<Vertex> vertices;
//		vector<unsigned int> indices;
//		vector<Texture> textures;
//
//
//		for (unsigned int i = 0; i < mesh->npoints; ++i)
//		{
//			Vertex vertex;
//			// process vertex positions, normals and texture coordinates
//			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
//			vertex.Position = positions;
//
//			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
//			vertex.Normal = normals;
//
//
//			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
//			{
//				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
//				vertex.TexCoords = texturesCoords;
//			}
//			else
//				vertex.TexCoords = float2(0.0f, 0.0f);
//
//			vertices.push_back(vertex);
//		}
//
//		for (int j = 0; j < mesh->ntriangles; ++j)
//		{
//			indices.push_back(mesh->triangles[j * 3]);
//			indices.push_back(mesh->triangles[j * 3 + 1]);
//			indices.push_back(mesh->triangles[j * 3 + 2]);
//		}
//
//		par_shapes_free_mesh(mesh);
//		//Mesh* cube = new Mesh(vertices, indices, textures);
//		//meshes.push_back(cube);
//
//		return true;
//	}
//
//
//	return false;
//}

void ModuleModelLoader::ProcessMeshData(const MeshData & data, Mesh & mesh)
{
	for (unsigned int i = 0; i < data.num_vertices; i++)
	{
		Vertex vertex;
		float3 positions;
		positions.x = data.positions[i*3];
		positions.y = data.positions[i*3 + 1];
		positions.z = data.positions[i*3 + 2];
		vertex.Position = positions;

		float3 normals;
		normals.x = data.normals[i*3];
		normals.y = data.normals[i*3 + 1];
		normals.z = data.normals[i*3 + 2];
		vertex.Normal = normals;

		//TODO: check if mesh contains texture coords or not
		float2 texturesCoords;
		texturesCoords.x = data.texture_coords[i*2];
		texturesCoords.y = data.texture_coords[i*2 + 1];
		vertex.TexCoords = texturesCoords;

		mesh.vertices.push_back(vertex);
	}
	//TODO: fix error
	for (unsigned int i = 0; i < data.num_indices; i++)
	{
		mesh.indices.push_back(data.indices[i]);
	}

	mesh.name = data.name;
}
