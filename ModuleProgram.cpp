#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "SDL/SDL.h"
#include "GL/glew.h"
#include "MathGeoLib/Math/float4x4.h"
#include "assert.h"
#include <string>
#include <streambuf>
#include <fstream>


bool ModuleProgram::Init()
{
	//Lighting shaders
	flatLighting = createProgramWithShaders("../Shaders/flat.vs", "../Shaders/flat.fs");
	gouraudLighting = createProgramWithShaders("../Shaders/Gouraud.vs", "../Shaders/Gouraud.fs");
	phongLighting = createProgramWithShaders("../Shaders/Phong.vs", "../Shaders/Phong.fs");
	blinnLighting = createProgramWithShaders("../Shaders/Blinn.vs", "../Shaders/Blinn.fs");

	uber = createProgramWithShaders("../Shaders/UberShader.vs", "../Shaders/UberShader.fs");

	//Skybox shader
	skyboxProg = createProgramWithShaders("../Shaders/Skybox.vs", "../Shaders/Skybox.fs");

	//Default shader
	defaultProg = createProgramWithShaders("../Shaders/VertexShader.vs", "../Shaders/Model.fs");

	SetUpUniformsBuffer();

	return true;
}

bool ModuleProgram::CleanUp()
{
	glDeleteProgram(defaultProg);

	//TODO: Randomly this line crashes on close
	glDeleteProgram(flatLighting);
	glDeleteProgram(gouraudLighting);
	glDeleteProgram(phongLighting);
	glDeleteProgram(blinnLighting);
	
	return true;
}

void ModuleProgram::SetUpUniformsBuffer()
{
	unsigned int uniformBlockIndexDefault = glGetUniformBlockIndex(defaultProg, "Matrices");
	glUniformBlockBinding(defaultProg, uniformBlockIndexDefault, 0);

	glGenBuffers(1, &uniformsBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float4x4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformsBuffer, 0, 2 * sizeof(float4x4));
}

unsigned int ModuleProgram::createProgramWithShaders(const char * vertexFilename, const char * fragmentFilename) const
{
	LOG("Compiling Vertex Shader from %s", vertexFilename);
	unsigned int vertexShader = createShader(vertexFilename, GL_VERTEX_SHADER);
	LOG("Compiling Fragment Shader from %s", fragmentFilename);
	unsigned int fragmentShader = createShader(fragmentFilename, GL_FRAGMENT_SHADER);

	return createProgram(vertexShader, fragmentShader);
}

unsigned int ModuleProgram::createProgram(unsigned int vShader, unsigned int fShader) const
{
	unsigned int program = glCreateProgram();

	if (vShader != NULL)
	{
		LOG("Attaching Vertex Shader");
		glAttachShader(program, vShader);
	}

	if (fShader != NULL)
	{
		LOG("Attaching Fragment Shader");
		glAttachShader(program, fShader);
	}

	LOG("Linking program");
	glLinkProgram(program);

	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		LOG("\nERROR::PROGRAM::CREATION_FAILED");
		LOG("ERROR: %s\n", infoLog);
	}

	//Now we can delete shaders
	LOG("Deleting Vertex Shader");
	glDeleteShader(vShader);

	LOG("Deleting Fragment Shader\n");
	glDeleteShader(fShader);

	return program;
}

unsigned int ModuleProgram::createShader(const char * filename, unsigned int shaderType) const
{
	assert(filename != nullptr);

	char* data = readFile(filename);
	unsigned int shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &data, NULL);
	glCompileShader(shaderId);
	delete data;

	GLint success = GL_FALSE;
	int logLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> shaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &shaderError[0]);
		LOG("ERROR: Shader with path %s coudn't be compiled : %s\n", filename, &shaderError[0]);
	}

	return shaderId;
}

char* ModuleProgram::readFile(const char* file_name) const
{
	assert(file_name != nullptr);

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
