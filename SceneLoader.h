#ifndef __SceneLoader_H__
#define __SceneLoader_H__

#include "Dependencies/rapidjson/document.h"
#include "include/Math/float3.h"
#include "include/Math/float4.h"

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	void ClearScene();
	void LoadJSON(const char * json);
	char * GetJSON() const;

	void StartGameObject();
	void FinishGameObject();

	void AddUnsignedInt(const char * name, unsigned int value);
	unsigned int GetUnsignedInt(const char * name, unsigned int defaultVal);
	void AddFloat(const char * name, float value);
	float GetFloat(const char * name, float defaultVal);
	void AddString(const char * name, const char * value);
	const char * GetString(const char * name, const char * defaultVal);
	
	void AddVec3f(const char * name, const float3 & value);
	float3 GetVec3f(const char * name, const float3 & defaultVal);
	void AddVec4f(const char * name, const float4 & value);
	float4 GetVec4f(const char * name, const float4 & defaultVal);

	void SetCurrentObject(unsigned int UID);

	//TODO: Save current scene temporal
	//TODO: Load temporal scene

private:
	rapidjson::Document document;
	rapidjson::Value currentObject;
};

#endif __SceneLoader_H__