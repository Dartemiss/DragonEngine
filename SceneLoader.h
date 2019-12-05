#ifndef __SceneLoader_H__
#define __SceneLoader_H__

#include "Dependencies/rapidjson/document.h"

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

	void SetCurrentObject(unsigned int UID);

private:
	rapidjson::Document document;
	rapidjson::Value currentObject;
};

#endif __SceneLoader_H__