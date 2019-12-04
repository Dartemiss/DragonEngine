#include "SceneLoader.h"
#include "Globals.h"

#include "Dependencies/rapidjson/prettywriter.h"

using namespace rapidjson;

SceneLoader::SceneLoader()
{
	document.SetObject();
}

SceneLoader::~SceneLoader()
{
}

void SceneLoader::LoadJSON(const char * json)
{
	document.Parse(json);
	LOG("Loaded scene json.");
}

char * SceneLoader::GetJSON() const
{
	StringBuffer buffer;
	buffer.Clear();

	Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return strdup(buffer.GetString());
}

void SceneLoader::AddFloat(const char * name, const float f)
{
	assert(name != nullptr);

	Document::AllocatorType& allocator = document.GetAllocator();

	Value floatName(name, allocator);
	document.AddMember(floatName, f, allocator);
}

float SceneLoader::GetFloat(const char * name, const float defaultF)
{
	assert(name != nullptr);

	Value & currentObject = document;

	if (!currentObject.HasMember(name))
		return defaultF;

	Value & member = currentObject[name];
	if (!member.IsNumber() || !member.IsDouble())
		return defaultF;

	return member.GetDouble();
	return defaultF;
}
