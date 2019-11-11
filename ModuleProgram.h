#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "Globals.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned int createVertexShader(char* filename);
	unsigned int createFragmentShader(char* filename);
	unsigned int createProgram(unsigned int vShader, unsigned int fShader);

	void setBool(const std::string &name, bool value, unsigned int prog) const;
	void setInt(const std::string &name, int value, unsigned int prog) const;
	void setFloat(const std::string &name, float value, unsigned int prog) const;

	char* readFile(char* filename) const;
};

#endif // __ModuleProgram_H__
