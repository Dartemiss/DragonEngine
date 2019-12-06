#include "SceneLoader.h"
#include "Globals.h"

#include "Dependencies/rapidjson/prettywriter.h"
#include <fstream>

using namespace rapidjson;

SceneLoader::SceneLoader()
{
	document.SetObject();
	Value gameObjects;
	gameObjects.SetArray();
	document.AddMember("Game Objects", gameObjects, document.GetAllocator());
	currentObject.SetNull();
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

void SceneLoader::FinishGameObject()
{
	document["Game Objects"].PushBack(currentObject, document.GetAllocator());
}

void SceneLoader::AddUnsignedInt(const char * name, unsigned int value)
{
	assert(name != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	if (currentObject.IsNull())
	{
		LOG("Can not add value if Game Object is not specified.");
		return;
	}

	Value vName(name, allocator);
	currentObject.AddMember(vName, value, allocator);
}

unsigned int SceneLoader::GetUnsignedInt(const char * name, unsigned int defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

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

	Document::AllocatorType& allocator = document.GetAllocator();

	if (currentObject.IsNull())
	{
		LOG("Can not add value if Game Object is not specified.");
		return;
	}

	Value vName(name, allocator);
	currentObject.AddMember(vName, value, allocator);
}

float SceneLoader::GetFloat(const char * name, float defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

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

	if (currentObject.IsNull())
	{
		LOG("Can not add value if Game Object is not specified.");
		return;
	}

	Value vName(name, allocator);
	Value vVal(value, allocator);
	currentObject.AddMember(vName, vVal, allocator);
}

const char * SceneLoader::GetString(const char * name, const char * defaultVal)
{
	assert(name != nullptr);
	assert(defaultVal != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

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

	if (currentObject.IsNull())
	{
		LOG("Can not add value if Game Object is not specified.");
		return;
	}

	Value vName(name, allocator);
	Value vVal;
	vVal.SetArray();
	vVal.PushBack(value.x, allocator);
	vVal.PushBack(value.y, allocator);
	vVal.PushBack(value.z, allocator);
	currentObject.AddMember(vName, vVal, allocator);
}

float3 SceneLoader::GetVec3f(const char * name, const float3 & defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & vector = currentObject[name];
	if (!vector.IsArray() || vector.Size() != 3)
		return defaultVal;

	for (int i = 0; i < vector.Size(); ++i)
		if (!vector[i].IsFloat())
			return defaultVal;

	return float3(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat());
}

void SceneLoader::AddVec4f(const char * name, const float4 & value)
{
	assert(name != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	if (currentObject.IsNull())
	{
		LOG("Can not add value if Game Object is not specified.");
		return;
	}

	Value vName(name, allocator);
	Value vVal;
	vVal.SetArray();
	vVal.PushBack(value.x, allocator);
	vVal.PushBack(value.y, allocator);
	vVal.PushBack(value.z, allocator);
	vVal.PushBack(value.w, allocator);
	currentObject.AddMember(vName, vVal, allocator);
}

float4 SceneLoader::GetVec4f(const char * name, const float4 & defaultVal)
{
	assert(name != nullptr);

	if (currentObject.IsNull())
		return defaultVal;

	if (!currentObject.HasMember(name))
		return defaultVal;

	Value & vector = currentObject[name];
	if (!vector.IsArray() || vector.Size() != 4)
		return defaultVal;

	for (int i = 0; i < vector.Size(); ++i)
		if (!vector[i].IsFloat())
			return defaultVal;

	return float4(vector[0].GetFloat(), vector[1].GetFloat(), vector[2].GetFloat(), vector[3].GetFloat());
}

void SceneLoader::SetCurrentObject(unsigned int UID)
{
	Value & gameObjects = document["Game Objects"];

	for (SizeType i = 0; i < gameObjects.Size(); ++i)
	{
		Value & currentObject = gameObjects[i];
		assert(currentObject.HasMember("UID"));

		if (currentObject["UID"].GetUint() == UID)
		{
			this->currentObject = currentObject;
			return;
		}
	}
	LOG("Could not find GameObject with UID %d", UID);
}

void SceneLoader::SaveSceneForPlay()
{
	SaveJSONToFile("scene_temporal.json");
}

void SceneLoader::LoadSceneForStop()
{
	//TODO load gameobjects and components

	LoadJSONFromFile("scene_temporal.json");
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
