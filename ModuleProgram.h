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
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned int createProgramWithShaders(char * vertexShader, char * fragmentShader);
	unsigned int createVertexShader(char* filename);
	unsigned int createFragmentShader(char* filename);
	unsigned int createProgram(const unsigned int vShader, const unsigned int fShader);

	void setBool(const std::string &name, bool value, unsigned int prog) const;
	void setInt(const std::string &name, int value, unsigned int prog) const;
	void setFloat(const std::string &name, float value, unsigned int prog) const;

	void SetUpUniformsBuffer();

	char* readFile(char* filename) const;

	//Programs
	unsigned int gridProg;
	unsigned int defaultProg;
	unsigned int uniformsBuffer;


	unsigned int flatLighting;
	unsigned int gouraudLighting;
	unsigned int phongLighting;
	unsigned int blinnLighting;
	unsigned int blinnTextures;
};

#endif // __ModuleProgram_H__
