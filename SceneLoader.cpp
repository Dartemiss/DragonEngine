#include "SceneLoader.h"
#include "Globals.h"

#include "Rapidjson/prettywriter.h"
#include <fstream>

using namespace rapidjson;

SceneLoader::SceneLoader()
{
	document.SetObject();
	Value gameObjects;
	gameObjects.SetArray();
	document.AddMember("Game Objects", gameObjects, document.GetAllocator());
	currentObject.SetNull();
	currentComponent.SetNull();
}

SceneLoader::~SceneLoader()
{
}

void SceneLoader::ClearScene()
{
	document.SetObject();
	Value gameObjects;
	gameObjects.SetArray();
	document.AddMember("Game Objects", gameObjects, document.GetAllocator());
	currentObject.SetNull();
	currentComponent.SetNull();
	LOG("Cleared scene data.");
}

void SceneLoader::LoadJSON(const char * json)
{
	document.Parse(json);
	LOG("Loaded scene from json.");
}

char * SceneLoader::GetJSON() const
{
	StringBuffer buffer;
	buffer.Clear();

	Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return _strdup(buffer.GetString());
}

void SceneLoader::StartGameObject()
{
	currentObject.SetObject();
}

void SceneLoader::CreateComponentArray()
{
	Value components;
	components.SetArray();
	currentObject.AddMember("Components", components, document.GetAllocator());
}

void SceneLoader::FinishGameObject()
{
	document["Game Objects"].PushBack(currentObject, document.GetAllocator());
	currentObject.SetNull();
}

void SceneLoader::StartComponent()
{
	currentComponent.SetObject();
}

void SceneLoader::FinishComponent()
{
	currentObject["Components"].PushBack(currentComponent, document.GetAllocator());
	currentComponent.SetNull();
}

void SceneLoader::AddUnsignedInt(const char * name, unsigned int value)
{
	assert(name != nullptr);

	Value vName(name, document.GetAllocator());
	Value vVal(value);
	AddMemberToObjectOrComponent(vName, vVal);
}

unsigned int SceneLoader::GetUnsignedInt(const char * name, unsigned int defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentComponent.IsNull())
	{
		if (!currentComponent.HasMember(name))
			return defaultVal;

		Value & member = currentComponent[name];
		if (!member.IsNumber() || !member.IsUint())
			return defaultVal;

		return member.GetUint();
	}

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & member = currentObject[name];
	if (!member.IsNumber() || !member.IsUint())
		return defaultVal;

	return member.GetUint();
}

void SceneLoader::AddFloat(const char * name, float value)
{
	assert(name != nullptr);

	Value vName(name, document.GetAllocator());
	Value vVal(value);
	AddMemberToObjectOrComponent(vName, vVal);
}

float SceneLoader::GetFloat(const char * name, float defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentComponent.IsNull())
	{
		if (!currentComponent.HasMember(name))
			return defaultVal;

		Value & member = currentComponent[name];
		if (!member.IsNumber() || !member.IsDouble())
			return defaultVal;

		return (float)member.GetDouble();
	}

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & member = currentObject[name];
	if (!member.IsNumber() || !member.IsDouble())
		return defaultVal;

	return (float)member.GetDouble();
}

void SceneLoader::AddString(const char * name, const char * value)
{
	assert(name != nullptr);
	assert(value != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	Value vName(name, allocator);
	Value vVal(value, allocator);
	AddMemberToObjectOrComponent(vName, vVal);
}

const char * SceneLoader::GetString(const char * name, const char * defaultVal)
{
	assert(name != nullptr);
	assert(defaultVal != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentComponent.IsNull())
	{
		if (!currentComponent.HasMember(name))
			return defaultVal;

		Value & member = currentComponent[name];
		if (!member.IsString())
			return defaultVal;

		return member.GetString();
	}

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & member = currentObject[name];
	if (!member.IsString())
		return defaultVal;

	return member.GetString();
}

void SceneLoader::AddVec3f(const char * name, const float3 & value)
{
	assert(name != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	Value vName(name, allocator);
	Value vVal;
	vVal.SetArray();
	vVal.PushBack(value.x, allocator);
	vVal.PushBack(value.y, allocator);
	vVal.PushBack(value.z, allocator);
	AddMemberToObjectOrComponent(vName, vVal);
}

float3 SceneLoader::GetVec3f(const char * name, const float3 & defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentComponent.IsNull())
	{
		if (!currentComponent.HasMember(name))
			return defaultVal;

		Value & vector = currentComponent[name];
		if (!vector.IsArray() || vector.Size() != 3)
			return defaultVal;

		for (unsigned int i = 0; i < vector.Size(); ++i)
			if (!vector[i].IsFloat())
				return defaultVal;

		return float3(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat());
	}

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & vector = currentObject[name];
	if (!vector.IsArray() || vector.Size() != 3)
		return defaultVal;

	for (unsigned int i = 0; i < vector.Size(); ++i)
		if (!vector[i].IsFloat())
			return defaultVal;

	return float3(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat());
}

void SceneLoader::AddVec4f(const char * name, const float4 & value)
{
	assert(name != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	Value vName(name, allocator);
	Value vVal;
	vVal.SetArray();
	vVal.PushBack(value.x, allocator);
	vVal.PushBack(value.y, allocator);
	vVal.PushBack(value.z, allocator);
	vVal.PushBack(value.w, allocator);
	AddMemberToObjectOrComponent(vName, vVal);
}

float4 SceneLoader::GetVec4f(const char * name, const float4 & defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentComponent.IsNull())
	{
		if (!currentComponent.HasMember(name))
			return defaultVal;

		Value & vector = currentComponent[name];
		if (!vector.IsArray() || vector.Size() != 4)
			return defaultVal;

		for (unsigned int i = 0; i < vector.Size(); ++i)
			if (!vector[i].IsFloat())
				return defaultVal;

		return float4(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat(), vector[3].GetFloat());
	}

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & vector = currentObject[name];
	if (!vector.IsArray() || vector.Size() != 4)
		return defaultVal;

	for (unsigned int i = 0; i < vector.Size(); ++i)
		if (!vector[i].IsFloat())
			return defaultVal;

	return float4(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat(), vector[3].GetFloat());
}

bool SceneLoader::SetCurrentObject(unsigned int parentUID)
{
	Value & gameObjects = document["Game Objects"];
	assert(gameObjects.IsArray());

	for (SizeType i = 0; i < gameObjects.Size(); ++i)
	{
		Value & currentObject = gameObjects[i];
		assert(currentObject.HasMember("parentUID"));

		if (currentObject["parentUID"].GetUint() == parentUID)
		{
			this->currentObject = currentObject;
			gameObjects.Erase(gameObjects.Begin() + i);
			currentComponent.SetNull();
			return true;
		}
	}
	return false;
}

bool SceneLoader::SelectNextComponent()
{
	Value & components = currentObject["Components"];
	assert(components.IsArray());

	if (components.Size() == 0)
		return false;

	currentComponent = components[0];
	components.Erase(components.Begin());
	return true;
}

void SceneLoader::SaveJSONToFile(const char * filename)
{
	char* json = GetJSON();
	FILE* file = nullptr;
	fopen_s(&file, filename, "wt");
	if (!file)
	{
		LOG("Error saving scene. Can not create %s file.", filename);
		return;
	}
	fwrite(json, sizeof(char), strlen(json), file);
	fclose(file);
}

void SceneLoader::LoadJSONFromFile(const char * filename)
{
	char* json = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, filename, "rt");
	if (!file)
	{
		LOG("Error loading scene. File %s does not exist.", filename);
		return;
	}


	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);
	json = (char*)malloc(size + 1);
	fread(json, 1, size, file);
	json[size] = 0;
	fclose(file);

	LoadJSON(json);
}

void SceneLoader::AddMemberToObjectOrComponent(Value & name, Value & val)
{
	Document::AllocatorType& allocator = document.GetAllocator();

	if (!currentComponent.IsNull())
		currentComponent.AddMember(name, val, allocator);

	else if (!currentObject.IsNull())
		currentObject.AddMember(name, val, allocator);

	else
		LOG("Can not add value if Game Object is not specified.");
}
