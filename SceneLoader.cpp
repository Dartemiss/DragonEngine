#include "SceneLoader.h"
#include "Globals.h"

#include "Dependencies/rapidjson/prettywriter.h"

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
		LOG("Can not add float if Game Object is not specified.");
		return;
	}

	Value intName(name, allocator);
	currentObject.AddMember(intName, value, allocator);
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
		LOG("Can not add float if Game Object is not specified.");
		return;
	}

	Value floatName(name, allocator);
	currentObject.AddMember(floatName, value, allocator);
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

void SceneLoader::SetCurrentObject(unsigned int UID)
{
	Value & gameObjects = document["Game Objects"];

	for (SizeType i = 0; i < gameObjects.Size(); i++)
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
