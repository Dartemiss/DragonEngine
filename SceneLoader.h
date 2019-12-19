#ifndef __SceneLoader_H__
#define __SceneLoader_H__

#include "Rapidjson/document.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float4.h"

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	void ClearScene();
	void LoadJSON(const char * json);
	char * GetJSON() const;

	//Call each time a Game Object is processed in this order
	void StartGameObject();
	void CreateComponentArray();
	void FinishGameObject();

	//Call each time a component is added to a Game Object
	void StartComponent();
	void FinishComponent();

	//Add and Get functions for each type. Requires call to StartGameObject before use.
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

	//Prepare to read Game Object with requested UID.
	bool SetCurrentObject(unsigned int parentUID);
	//Prepare to read next Component
	bool SelectNextComponent();

	void SaveJSONToFile(const char * filename);
	void LoadJSONFromFile(const char * filename);

private:
	rapidjson::Document document;
	rapidjson::Value currentObject;
	rapidjson::Value currentComponent;

	void AddMemberToObjectOrComponent(rapidjson::Value & name, rapidjson::Value & val);
};

#endif __SceneLoader_H__