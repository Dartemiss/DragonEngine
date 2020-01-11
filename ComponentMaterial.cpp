#include "ComponentMaterial.h"
#include "SceneLoader.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "SceneImporter.h"
#include "GL/glew.h"

using namespace std;

ComponentMaterial::ComponentMaterial(GameObject* go)
{
	myGameObject = go;
	myType = MATERIAL;

	whiteFallbackTexture = App->texture->getWhiteFallbackTexture();
	whitefallbackColor = float4(1, 1, 1, 1);




	kDiffuse = 0.5;
	kSpecular = 0.2;
	kAmbient = 0.2;
	shininess = 128;

	diffuseColor = float4(1, 0 ,0, 1);
	specularColor = float3(0, 1, 0);
	emissiveColor = float3(0, 0, 0.2);
}

ComponentMaterial::ComponentMaterial(GameObject * go, ComponentMaterial * comp)
{
	myGameObject = go;
	myType = MATERIAL;
	//TODO: When this component is done this method have to copy all comp relevant data to this object

	this->kDiffuse = comp->kDiffuse;
	this->kAmbient = comp->kAmbient;
	this->kSpecular = comp->kSpecular;
	this->shininess = comp->shininess;

	this->diffuseColor = comp->diffuseColor;
	this->specularColor = comp->specularColor;
	this->emissiveColor = comp->emissiveColor;

	this->diffuseMap = comp->diffuseMap;
	this->specularMap = comp->specularMap;
	this->occlusionMap = comp->occlusionMap;
	this->emissiveMap = comp->emissiveMap;

	this->whiteFallbackTexture = comp->whiteFallbackTexture;
	this->whitefallbackColor = comp->whitefallbackColor;
}


ComponentMaterial::~ComponentMaterial()
{
	//TODO: check it doesnt delete from moduleTextures too
	delete diffuseMap;
	delete specularMap;
	delete occlusionMap;
	delete emissiveMap;
}

void ComponentMaterial::Update()
{
	return;
}

bool ComponentMaterial::CleanUp()
{
	return false;
}

void ComponentMaterial::SetTextures(vector<Texture*> & textures)
{
	//TODO change textures parameter to pointers reference
	string name;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		name = textures[i]->type;
		if (strcmp(name.data(), "_diffuse") == 0)
			diffuseMap = textures[i];
		else if (strcmp(name.data(), "_specular") == 0)
			specularMap = textures[i];
		else if (strcmp(name.data(), "_occlusive") == 0)
			occlusionMap = textures[i];
		else if (strcmp(name.data(), "_emissive") == 0)
			emissiveMap = textures[i];
	}
}

void ComponentMaterial::SetDrawTextures(const unsigned int program)
{
	glUniform1f(glGetUniformLocation(program, "material.k_diffuse"), kDiffuse);
	glUniform1f(glGetUniformLocation(program, "material.k_specular"), kSpecular);
	glUniform1f(glGetUniformLocation(program, "material.k_ambient"), kAmbient);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);

	unsigned int tCount = 0;

	//Set diffuse color or texture
	glActiveTexture(GL_TEXTURE0 + tCount);
	glProgramUniform1i(program, glGetUniformLocation(program, "material.diffuse_map"), tCount);
	if (diffuseMap != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
		glProgramUniform4fv(program, glGetUniformLocation(program, "material.diffuse_color"), 1, &whitefallbackColor[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, whiteFallbackTexture->id);
		glProgramUniform4fv(program, glGetUniformLocation(program, "material.diffuse_color"), 1, &diffuseColor[0]);
	}

	//Set specular color or texture
	++tCount;
	glActiveTexture(GL_TEXTURE0 + tCount);
	glProgramUniform1i(program, glGetUniformLocation(program, "material.specular_map"), tCount);
	if (specularMap != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, specularMap->id);
		glUniform3fv(glGetUniformLocation(program, "material.specular_color"), 1, &whitefallbackColor[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, whiteFallbackTexture->id);
		glUniform3fv(glGetUniformLocation(program, "material.specular_color"), 1, &specularColor[0]);
	}

	//Set occlusion texture or disable
	++tCount;
	glActiveTexture(GL_TEXTURE0 + tCount);
	glProgramUniform1i(program, glGetUniformLocation(program, "material.occlusion_map"), tCount);
	if (occlusionMap != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, occlusionMap->id);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, whiteFallbackTexture->id);
	}

	//Set emissive color or texture
	++tCount;
	glActiveTexture(GL_TEXTURE0 + tCount);
	glUniform1i(glGetUniformLocation(program, "material.emissive_map"), tCount);
	if (emissiveMap != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, emissiveMap->id);
		glUniform3fv(glGetUniformLocation(program, "material.emissive_color"), 1, &whitefallbackColor[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, whiteFallbackTexture->id);
		glUniform3fv(glGetUniformLocation(program, "material.emissive_color"), 1, &emissiveColor[0]);
	}
}

void ComponentMaterial::OnSave(SceneLoader & loader)
{
	loader.AddUnsignedInt("Type", myType);

	loader.AddFloat("kDiffuse", kDiffuse);
	loader.AddFloat("kSpecular", kSpecular);
	loader.AddFloat("kAmbient", kAmbient);
	loader.AddFloat("shininess", shininess);


	if (diffuseMap != nullptr)
		loader.AddString("diffuseMap", diffuseMap->path.c_str());
	else
		loader.AddVec4f("diffuseColor", diffuseColor);

	if (specularMap != nullptr)
		loader.AddString("specularMap", specularMap->path.c_str());
	else
		loader.AddVec3f("specularColor", specularColor);

	if (occlusionMap != nullptr)
		loader.AddString("occlusionMap", occlusionMap->path.c_str());

	if (emissiveMap != nullptr)
		loader.AddString("emissiveMap", emissiveMap->path.c_str());
	else
		loader.AddVec3f("emissiveColor", emissiveColor);
}

void ComponentMaterial::OnLoad(SceneLoader & loader)
{
	kDiffuse = loader.GetFloat("kDiffuse", 0.0f);
	kSpecular = loader.GetFloat("kSpecular", 0.0f);
	kAmbient = loader.GetFloat("kAmbient", 0.0f);
	shininess = loader.GetFloat("shininess", 0.0f);

	string currName;
	currName = loader.GetString("diffuseMap", "non-existent");
	if (currName != "non-existent")
	{
		diffuseMap = new Texture();
		Importer->LoadMaterial(currName.c_str(), *diffuseMap);
		App->texture->LoadTexture(*diffuseMap);
	}
	else
		diffuseColor = loader.GetVec4f("diffuseColor", float4(0, 0, 0, 0));
	
	currName = loader.GetString("specularMap", "non-existent");
	if (currName != "non-existent")
	{
		specularMap = new Texture();
		Importer->LoadMaterial(currName.c_str(), *specularMap);
		App->texture->LoadTexture(*specularMap);
	}
	else
		specularColor = loader.GetVec3f("specularColor", float3(0, 0, 0));
	
	currName = loader.GetString("occlusionMap", "non-existent");
	if (currName != "non-existent")
	{
		occlusionMap = new Texture();
		Importer->LoadMaterial(currName.c_str(), *occlusionMap);
		App->texture->LoadTexture(*occlusionMap);
	}

	currName = loader.GetString("emissiveMap", "non-existent");
	if (currName != "non-existent")
	{
		emissiveMap = new Texture();
		Importer->LoadMaterial(currName.c_str(), *emissiveMap);
		App->texture->LoadTexture(*emissiveMap);
	}
	else
		emissiveColor = loader.GetVec3f("emissiveColor", float3(0, 0, 0));
}
