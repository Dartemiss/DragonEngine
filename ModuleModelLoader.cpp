#include "Application.h"
#include "ModuleModelLoader.h"
#include"ModuleTexture.h"
#include "ModuleCamera.h"
#include "Timer.h"
#include <Assimp/postprocess.h>
#include <Assimp/material.h>
#include <Assimp/mesh.h>
#include <Assimp/cimport.h>
#include "Assimp/Logger.hpp"
#include "Assimp/DefaultLogger.hpp"
#include "myStream.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "Utils/par_shapes.h"

using namespace Assimp;
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
	emptyScene();
	
	return true;
}

void ModuleModelLoader::Draw(const unsigned int program) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}


void ModuleModelLoader::loadModel(const string &path)
{
	LOG("Importing model \n");
	const unsigned int severity = Logger::Debugging | Logger::Info | Logger::Err | Logger::Warn;
	DefaultLogger::create("", Logger::NORMAL);
	Assimp::DefaultLogger::get()->attachStream(new myStream(), severity);

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("ERROR ASSIMP: %s \n", aiGetErrorString());
		return;
	}

	directory = computeDirectory(path);
	nameOfModel = ComputeName(path);

	if (directory == "")
		return;
	LOG("For each mesh located on the current node, processing meshes.")
	processNode(scene->mRootNode, scene);

	DefaultLogger::kill();

	return;

}

const int ModuleModelLoader::GetNumberOfMeshes() const
{
	return meshes.size();
}

const int ModuleModelLoader::GetNumberOfTriangles(const bool triangles) const
{
	int counter = 0;

	for (auto mesh : meshes)
	{
		counter += mesh->indices.size();
	}
	return triangles ? counter / 3 : counter;
}

void ModuleModelLoader::GetMeshes(vector<Mesh*> &meshes)
{
	meshes = this->meshes;
}

void ModuleModelLoader::processNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back((processMesh(mesh, scene)));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* ModuleModelLoader::processMesh(const aiMesh * mesh, const aiScene * scene)
{
	//Filling data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		float3 positions;
		positions.x = mesh->mVertices[i].x;
		positions.y = mesh->mVertices[i].y;
		positions.z = mesh->mVertices[i].z;
		vertex.Position = positions;

		float3 normals;
		normals.x = mesh->mNormals[i].x;
		normals.y = mesh->mNormals[i].y;
		normals.z = mesh->mNormals[i].z;
		vertex.Normal = normals;

		
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			float2 texturesCoords;
			texturesCoords.x = mesh->mTextureCoords[0][i].x;
			texturesCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texturesCoords;
		}
		else
			vertex.TexCoords = float2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices of each face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// process material
		
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			// 1. diffuse maps
			vector<Texture> diffuseMaps = App->texture->loadMaterialTextures(material,
				aiTextureType_DIFFUSE, "texture_diffuse", directory);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<Texture> specularMaps = App->texture->loadMaterialTextures(material,
				aiTextureType_SPECULAR, "texture_specular", directory);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			vector<Texture> normalMaps = App->texture->loadMaterialTextures(material,
				aiTextureType_NORMALS, "texture_normal",directory);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			vector<Texture> heightMaps = App->texture->loadMaterialTextures(material,
				aiTextureType_AMBIENT, "texture_height",directory);
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			//Count number of textures
			numberOfTextures += textures.size();

		}
		
	return new Mesh(vertices, indices, textures);
}

string ModuleModelLoader::computeDirectory(const string &path) const
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
		return path.substr(0, path.find_last_of('/') + 1);
	}
	size_t doubleRightSlash = path.find_last_of('//');
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
		return path.substr(0, path.find_last_of('//') + 1);
	}

	size_t doubleLeftSlash = path.find_last_of('\\');
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
		return path.substr(0, path.find_last_of('\\') + 1);
	}

	LOG("ERROR: Invalid path.");
	return "";
}

string ModuleModelLoader::ComputeName(const string & path) const
{

	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
		return path.substr(path.find_last_of('/') + 1, path.size()-1);
	}
	size_t doubleRightSlash = path.find_last_of('//');
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
		return path.substr(path.find_last_of('//') + 1, path.size()-1);
	}

	size_t doubleLeftSlash = path.find_last_of('\\');
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
		return path.substr(path.find_last_of('\\') + 1,path.size() -1);
	}

	return path;
}

void ModuleModelLoader::emptyScene()
{
	meshes.clear();
	modelBox.clear();

	numberOfTextures = 0;
}




bool ModuleModelLoader::LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
	unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh)
	{
		par_shapes_scale(mesh, size, size, size);

		//Filling data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;


		for (unsigned int i = 0; i < mesh->npoints; ++i)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
			vertex.Position = positions;

			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
			vertex.Normal = normals;


			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
			{
				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
				vertex.TexCoords = texturesCoords;
			}
			else
				vertex.TexCoords = float2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for(int j = 0; j < mesh->ntriangles;++j)
		{
			indices.push_back(mesh->triangles[j * 3]);
			indices.push_back(mesh->triangles[j * 3 + 1]);
			indices.push_back(mesh->triangles[j * 3 + 2]);
		}

			
		par_shapes_free_mesh(mesh);


		Mesh* sphere = new Mesh(vertices, indices, textures);

		meshes.push_back(sphere);

		return true;
	}

	return false;
}

bool ModuleModelLoader::LoadCylinder(const char * name, const math::float3 & pos, const math::Quat & rot, float height, float radius, unsigned slices, unsigned stacks, const math::float4 & color)
{
	par_shapes_mesh* mesh = par_shapes_create_cylinder(int(slices), int(stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, height*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(radius, int(slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, height*-0.5f, 0.0f);

	if(mesh)
	{
		par_shapes_scale(mesh, radius, height, radius);
		par_shapes_merge_and_free(mesh, top);
		par_shapes_merge_and_free(mesh, bottom);

		//Filling data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;


		for (unsigned int i = 0; i < mesh->npoints; ++i)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
			vertex.Position = positions;

			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
			vertex.Normal = normals;


			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
			{
				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
				vertex.TexCoords = texturesCoords;
			}
			else
				vertex.TexCoords = float2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (int j = 0; j < mesh->ntriangles; ++j)
		{
			indices.push_back(mesh->triangles[j * 3]);
			indices.push_back(mesh->triangles[j * 3 + 1]);
			indices.push_back(mesh->triangles[j * 3 + 2]);
		}


		par_shapes_free_mesh(mesh);
		Mesh* cylinder = new Mesh(vertices, indices, textures);
		meshes.push_back(cylinder);

		return true;
	}

	return false;
}

bool ModuleModelLoader::LoadTorus(const char * name, const math::float3 & pos, const math::Quat & rot, float inner_r, float outer_r, unsigned slices, unsigned stacks, const math::float4 & color)
{
	par_shapes_mesh* mesh = par_shapes_create_torus(int(slices), int(stacks), inner_r);

	if(mesh)
	{
		par_shapes_scale(mesh, outer_r, outer_r, outer_r);
		
		//Filling data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;


		for (unsigned int i = 0; i < mesh->npoints; ++i)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
			vertex.Position = positions;

			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
			vertex.Normal = normals;


			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
			{
				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
				vertex.TexCoords = texturesCoords;
			}
			else
				vertex.TexCoords = float2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (int j = 0; j < mesh->ntriangles; ++j)
		{
			indices.push_back(mesh->triangles[j * 3]);
			indices.push_back(mesh->triangles[j * 3 + 1]);
			indices.push_back(mesh->triangles[j * 3 + 2]);
		}

		par_shapes_free_mesh(mesh);
		Mesh* torus = new Mesh(vertices, indices, textures);
		meshes.push_back(torus);

		return true;
	}

	return false;
}

bool ModuleModelLoader::LoadCube(const char * name, const math::float3 & pos, const math::Quat & rot, float size, const math::float4 & color)
{
	par_shapes_mesh* mesh = par_shapes_create_plane(1, 1);
	par_shapes_mesh* top = par_shapes_create_plane(1, 1);
	par_shapes_mesh* bottom = par_shapes_create_plane(1, 1);
	par_shapes_mesh* back = par_shapes_create_plane(1, 1);
	par_shapes_mesh* left = par_shapes_create_plane(1, 1);
	par_shapes_mesh* right = par_shapes_create_plane(1, 1);

	par_shapes_translate(mesh, -0.5f, -0.5f, 0.5f);

	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, -0.5f, 0.5f, 0.5f);

	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(back, float(PAR_PI), (float*)&math::float3::unitX);
	par_shapes_translate(back, -0.5f, 0.5f, -0.5f);

	par_shapes_rotate(left, float(-PAR_PI * 0.5), (float*)&math::float3::unitY);
	par_shapes_translate(left, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(right, float(PAR_PI*0.5), (float*)&math::float3::unitY);
	par_shapes_translate(right, 0.5f, -0.5f, 0.5f);

	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);
	par_shapes_merge_and_free(mesh, back);
	par_shapes_merge_and_free(mesh, left);
	par_shapes_merge_and_free(mesh, right);

	if (mesh)
	{
		par_shapes_scale(mesh, size, size, size);

		//Filling data
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;


		for (unsigned int i = 0; i < mesh->npoints; ++i)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			float3 positions(mesh->points[i * 3], mesh->points[i * 3 + 1], mesh->points[i * 3 + 2]);
			vertex.Position = positions;

			float3 normals(mesh->normals[i * 3], mesh->normals[i * 3 + 1], mesh->normals[i * 3 + 2]);
			vertex.Normal = normals;


			if (mesh->tcoords[0]) // does the mesh contain texture coordinates?
			{
				float2 texturesCoords(mesh->tcoords[i * 2], mesh->tcoords[i * 2 + 1]);
				vertex.TexCoords = texturesCoords;
			}
			else
				vertex.TexCoords = float2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (int j = 0; j < mesh->ntriangles; ++j)
		{
			indices.push_back(mesh->triangles[j * 3]);
			indices.push_back(mesh->triangles[j * 3 + 1]);
			indices.push_back(mesh->triangles[j * 3 + 2]);
		}

		par_shapes_free_mesh(mesh);
		Mesh* cube = new Mesh(vertices, indices, textures);
		meshes.push_back(cube);

		return true;
	}


	return false;
}
