#ifndef __SceneLoader_H__
#define __SceneLoader_H__

#include "Dependencies/rapidjson/document.h"

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();

	void LoadJSON(const char * json);
	char * GetJSON() const;

	void AddFloat(const char * name, const float f);
	float GetFloat(const char * name, const float defaultF);

private:
	rapidjson::Document document;
};

#endif __SceneLoader_H__