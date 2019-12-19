#include "ComponentMaterial.h"
#include "SceneLoader.h"
#include "ModuleTexture.h"
#include "GL/glew.h"


ComponentMaterial::ComponentMaterial(GameObject* go)
{
	myGameObject = go;
	myType = MATERIAL;
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
}


ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Update()
{
	return;
}

bool ComponentMaterial::CleanUp()
{
	return false;
}

void ComponentMaterial::SetTextures(std::vector<Texture> & textures)
{
	//TODO change textures parameter to pointers reference
	std::string name;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		name = textures[i].type;
		if (name == "texture_diffuse")
			diffuseMap = &textures[i];
		else if (name == "texture_specular")
			specularMap = &textures[i];
		else if (name == "texture_occlusive")
			occlusionMap = &textures[i];
		else if (name == "texture_emissive")
			emissiveMap = &textures[i];
	}
}

void ComponentMaterial::SetDrawTextures(const unsigned int program)
{
	/*glUniform1f(glGetUniformLocation(program, "material.k_diffuse"), kDiffuse);
	glUniform1f(glGetUniformLocation(program, "material.k_specular"), kSpecular);
	glUniform1f(glGetUniformLocation(program, "material.k_ambient"), KAmbient);
	glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);*/

	unsigned int tCount = 0;

	/*int n;
	glGetProgramStageiv(program, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &n);
	unsigned * indices = new unsigned[n];*/

	/*const unsigned difColor = 0, difTexture = 1, specColor = 2, specTexture = 3,
		occNot = 4, occTexture = 5, emisColor = 6, emisTexture = 7;*/

	//Set diffuse color or texture
	//int current_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "get_diffuse_color");
	if (diffuseMap != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + tCount);
		glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
		glProgramUniform1i(program, glGetUniformLocation(program, "material.diffuse_map"), tCount);
		++tCount;
		glProgramUniform4fv(program, glGetUniformLocation(program, "material.diffuse_color"), 1, &float4(255, 255, 255, 1)[0]);
		//indices[current_loc] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "get_diffuse_from_texture");
	}
	else
	{
		glProgramUniform4fv(program, glGetUniformLocation(program, "material.diffuse_color"), 1, &diffuseColor[0]);
		//indices[current_loc] = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "get_diffuse_from_color");
	}

	//Set specular color or texture
	//current_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "get_specular_color");
	//if (specularMap != nullptr)
	//{
	//	glActiveTexture(GL_TEXTURE0 + tCount);
	//	glUniform1i(glGetUniformLocation(program, "material.specular_map"), tCount);
	//	++tCount;
	//	indices[current_loc] = specTexture;
	//}
	//else
	//{
	//	glUniform3fv(glGetUniformLocation(program, "material.specular_color"), 3, &specularColor[0]);
	//	indices[current_loc] = specColor;
	//}

	////Set occlusion texture or disable
	//current_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "get_occlusion_color");
	//if (occlusionMap != nullptr)
	//{
	//	glActiveTexture(GL_TEXTURE0 + tCount);
	//	glUniform1i(glGetUniformLocation(program, "material.occlusion_map"), tCount);
	//	++tCount;
	//	indices[current_loc] = occTexture;
	//}
	//else
	//{
	//	indices[current_loc] = occNot;
	//}

	////Set emissive color or texture
	//current_loc = glGetSubroutineUniformLocation(program, GL_FRAGMENT_SHADER, "get_emissive_color");
	//if (emissiveMap != nullptr)
	//{
	//	glActiveTexture(GL_TEXTURE0 + tCount);
	//	glUniform1i(glGetUniformLocation(program, "material.emissive_map"), tCount);
	//	indices[current_loc] = emisTexture;
	//}
	//else
	//{
	//	glUniform3fv(glGetUniformLocation(program, "material.emissive_color"), 3, &emissiveColor[0]);
	//	indices[current_loc] = emisColor;
	//}

	//glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, n, indices);
	//delete[] indices;
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
