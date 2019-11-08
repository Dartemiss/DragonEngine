#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "glew.h"
#include <string>
#include <fstream>
#include <streambuf>
#include "SDL.h"
#include "glew.h"
#include "assert.h"



ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init()
{
	return true;
}

update_status ModuleProgram::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
	return true;
}

unsigned int ModuleProgram::createProgram(unsigned int vShader, unsigned int fShader)
{
	unsigned int program = glCreateProgram();
	
	if (vShader != NULL) 
	{
		LOG("Attaching Vertex Shader");
		glAttachShader(program, vShader);
	}

	if(fShader != NULL)
	{
		LOG("Attaching Fragment Shader");
		glAttachShader(program, fShader);
	}

	LOG("Linking program");
	glLinkProgram(program);

	LOG("Deleting Vertex Shader");
	glDeleteShader(vShader);

	LOG("Deleting Fragment Shader");
	glDeleteShader(fShader);

	//Now we can delete shaders
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		LOG("ERROR::PROGRAM::CREATION_FAILED\n");
		LOG("ERROR: %s\n", infoLog);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	
	return program;
}


unsigned int ModuleProgram::createVertexShader(char * filename)
{
	assert(filename != NULL);
	char* data = readFile(filename);
	unsigned int shaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderId,1,&data,NULL);
	glCompileShader(shaderId);
	delete data;

	//glGetShaderiv(shaderId, GL_COMPILE_STATUS,NULL);
	

	return shaderId;
}

unsigned int ModuleProgram::createFragmentShader(char * filename)
{
	assert(filename != NULL);
	char* data = readFile(filename);
	unsigned int shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderId, 1, &data, NULL);
	glCompileShader(shaderId);
	delete data;
	//glGetShaderiv(shaderId, GL_COMPILE_STATUS, NULL);


	return shaderId;
}



char* ModuleProgram::readFile(char* file_name) const
{
	char* result = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		result = (char*)malloc(size + 1);
		fread(result, 1, size, file);
		result[size] = 0;
		fclose(file);
	}
	
	return result;
}




