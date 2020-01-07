#include "ComponentMaterial.h"
#include "SceneLoader.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "GL/glew.h"


ComponentMaterial::ComponentMaterial(GameObject* go)
{
	myGameObject = go;
	myType = MATERIAL;

	whiteFallbackTexture = App->texture->getWhiteFallbackTexture();
	whitefallbackColor = float4(1, 1, 1, 1);




	kDiffuse = 0.5;
	kSpecular = 0.2;
	KAmbient = 0.2;
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
	this->KAmbient = comp->KAmbient;
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

void ComponentMaterial::SetTextures(std::vector<Texture*> & textures)
{
	//TODO change textures parameter to pointers reference
	std::string name;
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
	//TODO: remove this and put into scene
	float3 light = float3(0, 10, 5);
	glUniform3fv(glGetUniformLocation(program, "directionalLight"), 1, &light[0]);


	glUniform1f(glGetUniformLocation(program, "material.k_diffuse"), kDiffuse);
	glUniform1f(glGetUniformLocation(program, "material.k_specular"), kSpecular);
	glUniform1f(glGetUniformLocation(program, "material.k_ambient"), KAmbient);
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
	//TODO implement save
}

void ComponentMaterial::OnLoad(SceneLoader & loader)
{
	//TODO implement load
}
