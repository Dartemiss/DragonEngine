#include "Globals.h"
#include "Application.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "glew.h"
#include "SDL.h"
#include "glew.h"
#include "include/Math/float4x4.h"
#include "assert.h"
#include <string>
#include <streambuf>
#include <fstream>


bool ModuleProgram::Init()
{

	//Grid shader
	unsigned int vs2 = App->program->createVertexShader("../Shaders/Grid.vs");
	unsigned int fs2 = App->program->createFragmentShader("../Shaders/Grid.fs");

	gridProg = App->program->createProgram(vs2, fs2);

	//Default shader
	unsigned int vs = App->program->createVertexShader("../Shaders/VertexShader.vs");
	unsigned int fs = App->program->createFragmentShader("../Shaders/Model.fs");

	defaultProg = App->program->createProgram(vs, fs);

	SetUpUniformsBuffer();

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
	glDeleteProgram(defaultProg);
	glDeleteProgram(gridProg);

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

	
	return program;
}

void ModuleProgram::setBool(const std::string & name, bool value, unsigned int prog) const
{
	glUniform1i(glGetUniformLocation(prog, name.c_str()), (int)value);
}

void ModuleProgram::setInt(const std::string & name, int value, unsigned int prog) const
{
	glUniform1i(glGetUniformLocation(prog, name.c_str()), value);
}

void ModuleProgram::setFloat(const std::string & name, float value, unsigned int prog) const
{
	glUniform1f(glGetUniformLocation(prog, name.c_str()), value);
}

void ModuleProgram::SetUpUniformsBuffer()
{
	unsigned int uniformBlockIndexGrid = glGetUniformBlockIndex(gridProg, "Matrices");
	glUniformBlockBinding(gridProg, uniformBlockIndexGrid, 0);

	unsigned int uniformBlockIndexDefault = glGetUniformBlockIndex(defaultProg, "Matrices");
	glUniformBlockBinding(defaultProg, uniformBlockIndexDefault, 0);


	glGenBuffers(1, &uniformsBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, uniformsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float4x4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformsBuffer, 0, 2 * sizeof(float4x4));

}


unsigned int ModuleProgram::createVertexShader(char * filename)
{
	assert(filename != NULL);
	char* data = readFile(filename);
	unsigned int shaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderId,1,&data,NULL);
	glCompileShader(shaderId);
	delete data;

	GLint success = GL_FALSE;
	int logLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> vertexShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &vertexShaderError[0]);
		LOG("ERROR: Shader coudn't be compiled : %s\n", &vertexShaderError[0]);
	}
	

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

	GLint success = GL_FALSE;
	int logLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> fragmentShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &fragmentShaderError[0]);
		LOG("ERROR: Shader coudn't be compiled : %s\n", &fragmentShaderError[0]);
	}

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




