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
	ModuleProgram() = default;
	~ModuleProgram() = default;

	bool Init();
	bool CleanUp();

	void SetUpUniformsBuffer();

	//Programs
	unsigned int defaultProg = 0;
	unsigned int skyboxProg = 0;
	unsigned int uniformsBuffer = 0;

	unsigned int flatLighting = 0;
	unsigned int gouraudLighting = 0;
	unsigned int phongLighting = 0;
	unsigned int blinnLighting = 0;

	unsigned int uber = 0;

private:
	unsigned int createProgramWithShaders(const char * vertexShader, const char * fragmentShader) const;
	unsigned int createShader(const char * filename, unsigned int shaderType) const;
	unsigned int createProgram(unsigned int vShader, unsigned int fShader) const;

	char* readFile(const char* filename) const;
};

#endif // __ModuleProgram_H__
