#include "ModelImporter.h"

#include "Application.h"
#include "ModuleFilesystem.h"
#include "MeshImporter.h"
#include "MaterialImporter.h"

#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>
#include <Assimp/material.h>
#include <Assimp/mesh.h>
#include <Assimp/cimport.h>
#include "Assimp/Logger.hpp"
#include "Assimp/DefaultLogger.hpp"
#include "myStream.h"

using namespace Assimp;
using namespace std;

bool ModelImporter::Import(const char * path, const char * file, string & output_file)
{
	string filepath = path; filepath += file;
	LOG("Importing mesh from %s.", filepath.c_str());

	//Prepare assimp debugger
	const unsigned int severity = Logger::Debugging | Logger::Info | Logger::Err | Logger::Warn;
	DefaultLogger::create("", Logger::NORMAL);
	Assimp::DefaultLogger::get()->attachStream(new myStream(), severity);

	//Load info from model
	const aiScene* scene = aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("ERROR ASSIMP: %s \n", aiGetErrorString());
		LOG("Can't load model from %s.", filepath.c_str());
		return false;
	}

	LOG("For each mesh located on the current node, processing meshes.");
	
	modelName = file;
	size_t lastindex = modelName.find_last_of(".");
	modelName = modelName.substr(0, lastindex);
	directory = path;

	modelData.meshes.clear();
	modelData.textures.clear();
	modelData.pairs.clear();

	ProcessNode(scene->mRootNode, scene);


	//Clear assimp debugger
	DefaultLogger::kill();

	SaveModelFile(output_file);


	return false;
}

bool ModelImporter::Import(const char * file, const void * buffer, unsigned int size, string & output_file)
{
	if (!App->filesystem->IsDirectory("../Library"))
		App->filesystem->MakeDirectory("../Library");
	if (!App->filesystem->IsDirectory("../Library/Meshes"))
		App->filesystem->MakeDirectory("../Library/Meshes");

	string filename = file; filename += ".notfbx";
	output_file = "../Library/Meshes/"; output_file += filename.c_str();

	return App->filesystem->Save("../Library/Meshes/", filename.c_str(), buffer, size, false);
}

void ModelImporter::ProcessNode(aiNode * node, const aiScene * scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ModelImporter::ProcessMesh(const aiMesh * mesh, const aiScene * scene)
{
	MeshData meshData;

	//Fill vertex data
	meshData.num_vertices = mesh->mNumVertices;

	meshData.positions = new float[meshData.num_vertices * 3];
	meshData.normals = new float[meshData.num_vertices * 3];
	meshData.texture_coords = new float[meshData.num_vertices * 2];
	
	for (unsigned int i = 0; i < meshData.num_vertices; i++)
	{
		meshData.positions[3*i + 0] = mesh->mVertices[i].x;
		meshData.positions[3*i + 1] = mesh->mVertices[i].y;
		meshData.positions[3*i + 2] = mesh->mVertices[i].z;

		meshData.normals[3*i + 0] = mesh->mNormals[i].x;
		meshData.normals[3*i + 1] = mesh->mNormals[i].y;
		meshData.normals[3*i + 2] = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			meshData.texture_coords[2*i + 0] = mesh->mTextureCoords[0][i].x;
			meshData.texture_coords[2*i + 1] = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			meshData.texture_coords[2 * i + 0] = 0.0f;
			meshData.texture_coords[2 * i + 1] = 0.0f;
		}
	}

	//Fill indices data
	meshData.num_indices = mesh->mNumFaces * 3; //TODO: check what happens if not all faces are triangles?
	meshData.indices = new unsigned int[meshData.num_indices];

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshData.indices[3*i + j] = face.mIndices[j];
	}

	//Import mesh into own filesystem
	MeshImporter meshImporter;
	string meshOutput;
	unsigned int currentMeshCount = modelData.meshes.size() + 1;
	string meshName = modelName; meshName += to_string(currentMeshCount);
	meshImporter.Import(meshName.c_str(), meshData, meshOutput);
	modelData.meshes.push_back(meshOutput);


	//Process material
	string materialOutput;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		//TODO: chech this works correctly
		SearchTextureByType(material, aiTextureType_DIFFUSE, currentMeshCount, "_diffuse");
		SearchTextureByType(material, aiTextureType_SPECULAR, currentMeshCount, "_specular");
		SearchTextureByType(material, aiTextureType_AMBIENT, currentMeshCount, "_occlusive");
		SearchTextureByType(material, aiTextureType_EMISSIVE, currentMeshCount, "_emissive");
		SearchTextureByType(material, aiTextureType_NORMALS, currentMeshCount, "_normal");
		SearchTextureByType(material, aiTextureType_HEIGHT, currentMeshCount, "_height");
	}
}

void ModelImporter::SearchTextureDir(const string & fbxDir, const string & texDir, string & foundDir, string & texName)
{
	size_t lastindex = texDir.find_last_of("/");
	texName = texDir.substr(lastindex + 1, texDir.size() + 1);

	string checkingDir = fbxDir + texDir; 
	if (App->filesystem->Exists(checkingDir.c_str()))
	{
		size_t slash = checkingDir.find_last_of("/");
		foundDir = checkingDir.substr(0, slash + 1);
		LOG("Texture found where specified by fbx.");
		return;
	}
	
	checkingDir = fbxDir + texName;
	if (App->filesystem->Exists(checkingDir.c_str()))
	{
		foundDir = fbxDir;
		LOG("Texture found in same folder as fbx.");
		return;
	}

	checkingDir = "../Textures/" + texName;
	if (App->filesystem->Exists(checkingDir.c_str()))
	{
		foundDir = "../Textures/";
		LOG("Texture found in Textures folder.");
		return;
	}	

	checkingDir = "";
	LOG("Couldn't find texture");
}

void ModelImporter::SearchTextureByType(const aiMaterial * material, const aiTextureType texType, const unsigned int currentMeshCount, const string & typeName)
{
	for (unsigned int i = 0; i < material->GetTextureCount(texType); i++)
	{
		aiString str;
		aiTextureMapping mapping = aiTextureMapping_UV;

		material->GetTexture(texType, i, &str, &mapping, 0);
		string textureName = str.C_Str();
		size_t lastindex = textureName.find_last_of(".");
		textureName = textureName.substr(0, lastindex) + typeName;

		bool skip = false;
		for (unsigned int i = 0; i < modelData.textures.size(); i++)
		{
			if (strcmp(modelData.textures[i].c_str(), textureName.c_str()) == 0)
			{
				MeshTexPair pair; pair.mesh = currentMeshCount; pair.tex = i + 1;
				modelData.pairs.push_back(pair);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			MaterialImporter materialImporter;
			string materialOutput = typeName;

			string directoryPath;
			string name;

			SearchTextureDir(directory, str.C_Str(), directoryPath, name);

			materialImporter.Import(directoryPath.c_str(), name.c_str(), materialOutput);

			modelData.textures.push_back(materialOutput);
			MeshTexPair pair; pair.mesh = currentMeshCount; pair.tex = modelData.textures.size();
			modelData.pairs.push_back(pair);
		}

	}
}

void ModelImporter::SaveModelFile(string & output_file)
{
	unsigned int ranges[3] = { modelData.meshes.size(), modelData.textures.size(), modelData.pairs.size() };

	//Set up mesh sizes
	unsigned int * meshNameSizes = new unsigned int[modelData.meshes.size()];
	unsigned int meshNamesSize = 0;
	for (unsigned int i = 0; i < modelData.meshes.size(); i++)
	{
		meshNameSizes[i] = modelData.meshes[i].size();
		meshNamesSize += meshNameSizes[i] + 1;
	}

	//Set up texture sizes
	unsigned int * textureNameSizes = new unsigned int[modelData.textures.size()];
	unsigned int textureNamesSize = 0;
	for (unsigned int i = 0; i < modelData.textures.size(); i++)
	{
		textureNameSizes[i] = modelData.textures[i].size();
		textureNamesSize += textureNameSizes[i] + 1;
	}

	//Set up pair sizes
	unsigned int * pairData = new unsigned int[modelData.pairs.size() * 2];
	for (unsigned int i = 0; i < modelData.pairs.size(); i++)
	{
		pairData[2*i] = modelData.pairs[i].mesh;
		pairData[2*i + 1] = modelData.pairs[i].tex;
	}


	//Calculate total size
	unsigned int size = sizeof(ranges)							//ranges
		+ sizeof(unsigned int) * modelData.meshes.size()		//mesh name sizes
		+ meshNamesSize											//mesh names
		+ sizeof(unsigned int) * modelData.textures.size()		//texture name sizes
		+ textureNamesSize										//texture names
		+ sizeof(unsigned int) * 2 * modelData.pairs.size();	//mesh - texture pairs

	char* data = new char[size];
	char* cursor = data;

	unsigned int bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store mesh name sizes
	bytes = sizeof(unsigned int) * modelData.meshes.size();
	memcpy(cursor, meshNameSizes, bytes);
	
	for (unsigned int i = 0; i < modelData.meshes.size(); i++) // Store mesh names (strings)
	{
		cursor += bytes;
		bytes = modelData.meshes[i].size() + 1;
		memcpy(cursor, modelData.meshes[i].c_str(), bytes);
	}
	
	cursor += bytes; // Store texture name sizes
	bytes = sizeof(unsigned int) * modelData.textures.size();
	memcpy(cursor, textureNameSizes, bytes);
	
	for (unsigned int i = 0; i < modelData.textures.size(); i++) // Store texture names (strings)
	{
		cursor += bytes;
		bytes = modelData.textures[i].size() + 1;
		memcpy(cursor, modelData.textures[i].c_str(), bytes);
	}

	cursor += bytes; // Store mesh - texture pairs
	bytes = sizeof(unsigned int) * 2 * modelData.pairs.size();
	memcpy(cursor, pairData, bytes);

	Import(modelName.c_str(), data, size, output_file);
}

bool ModelImporter::Load(const char* exported_file, ModelData & model)
{
	char* buffer;

	string model_file = exported_file; model_file += ".notfbx";

	if (!App->filesystem->Load("../Library/Meshes/", model_file.c_str(), &buffer))
		return false;

	char* cursor = buffer;

	unsigned int ranges[3]; //Load ranges
	unsigned int bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	unsigned int meshNum = ranges[0];
	unsigned int texNum = ranges[1];
	unsigned int pairNum = ranges[2];


	cursor += bytes; //Load mesh name sizes
	bytes = sizeof(unsigned int) * meshNum;
	unsigned int * meshSizes = new unsigned int[meshNum];
	memcpy(meshSizes, cursor, bytes);

	char * name = new char;
	for (unsigned int i = 0; i < meshNum; i++) //For each mesh name size, load its name
	{
		cursor += bytes;
		bytes = meshSizes[i] + 1;
		memcpy(name, cursor, bytes);
		model.meshes.push_back(name);
	}

	cursor += bytes; //Load texture name sizes
	bytes = sizeof(unsigned int) * texNum;
	unsigned int * textureSizes = new unsigned int[texNum];
	memcpy(textureSizes, cursor, bytes);

	for (unsigned int i = 0; i < texNum; i++) //For each texture name size, load its name
	{
		cursor += bytes;
		bytes = textureSizes[i] + 1;
		memcpy(name, cursor, bytes);
		model.textures.push_back(name);
	}

	unsigned int * pair = new unsigned int[2]; //Load mesh - texture pairs
	MeshTexPair pairData;
	for (unsigned int i = 0; i < pairNum; i++)
	{
		cursor += bytes;
		bytes = sizeof(unsigned int) * 2;
		memcpy(pair, cursor, bytes);
		pairData.mesh = pair[0];
		pairData.tex = pair[1];
		model.pairs.push_back(pairData);
	}
}

void ModelImporter::LoadBunny()
{
	modelName = "Zombunny";

	modelData.meshes.push_back("Zombunny1");
	modelData.meshes.push_back("Zombunny2");

	modelData.textures.push_back("ZomBunnyDiffuse_diffuse");
	modelData.textures.push_back("ZomBunnyEmissive_emissive");
	modelData.textures.push_back("ZomBunnyOcclusion_occlusion");
	modelData.textures.push_back("ZomBunnySpecular_specular");

	MeshTexPair pair; pair.mesh = 1; pair.tex = 1;
	modelData.pairs.push_back(pair);
	
	pair.mesh = 1; pair.tex = 2;
	modelData.pairs.push_back(pair);

	pair.mesh = 1; pair.tex = 3;
	modelData.pairs.push_back(pair);

	pair.mesh = 1; pair.tex = 4;
	modelData.pairs.push_back(pair);

	pair.mesh = 2; pair.tex = 0;
	modelData.pairs.push_back(pair);

	string output;
	SaveModelFile(output);
}
