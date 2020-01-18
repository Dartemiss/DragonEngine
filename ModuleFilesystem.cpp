#include "ModuleFilesystem.h"

#include "SDL/SDL.h"
#include <string>
#include <filesystem>
#include "SceneImporter.h"

using namespace std;
using namespace std::tr2::sys;

ModuleFilesystem::ModuleFilesystem()
{
}


ModuleFilesystem::~ModuleFilesystem()
{
}

bool ModuleFilesystem::Init()
{
	LOG("Import all files on directory assets into own binary file");
	//Import all files on directory assets into own binary file
	string path = "../Assets";
	for(const auto& entry : recursive_directory_iterator(path))
	{
		//debug this pls
		string fullPath(entry.path().string());
		string fileExt(fullPath);
		size_t dotFound = fileExt.find_last_of(".");
		fileExt.erase(0, dotFound + 1);
		if(fileExt == "fbx")
		{
			string myPath(fullPath);
			string file = ComputeName(fullPath);


			size_t sizeFile = myPath.size() - file.size();
			myPath = myPath.substr(0, sizeFile);

			string s;
			//If artist put right paths of textures then import everything fromo here 
			
		}

	}

	return true;
}

std::string ModuleFilesystem::ComputeName(const std::string & path) const
{
	size_t simpleRightSlash = path.find_last_of('/');
	if (string::npos != simpleRightSlash)
	{
		LOG("Directory with simpleRightSlashes.")
			return path.substr(path.find_last_of('/') + 1, path.size() - 1);
	}
	size_t simpleLeftSlash = path.find_last_of('\\');
	if (string::npos != simpleLeftSlash)
	{
		LOG("Directory with simpleLeftSlashes.")
			return path.substr(path.find_last_of('\\') + 1, path.size() - 1);
	}
	size_t doubleRightSlash = path.find_last_of("//");
	if (string::npos != doubleRightSlash)
	{
		LOG("Directory with doubleRightSlashes.")
			return path.substr(path.find_last_of("//") + 1, path.size() - 1);
	}

	size_t doubleLeftSlash = path.find_last_of("\\\\");
	if (string::npos != doubleLeftSlash)
	{
		LOG("Directory with doubleLeftSlashes.")
			return path.substr(path.find_last_of("\\\\") + 1, path.size() - 1);
	}

	return path;
}


char* ModuleFilesystem::Load(const char * path, const char * file) const
{
	//Remember to delete memory after using it for avoiding memory leaks
	string filename = path; filename += file;

	SDL_RWops *rw = SDL_RWFromFile(filename.c_str(), "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	//Replace malloc for new, does the same job but better (free doesn't call destructors)
	char* res = new char[res_size + 1];

	Sint64 nb_read_total = 0; 
	Sint64 nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		delete[] res;
		return NULL;
	}

	res[nb_read_total] = '\0';
	
	return res;
}

bool ModuleFilesystem::Save(const char * path, const char * file, const void * buffer, unsigned int size, bool append) const
{
	string filename = path; filename += file;

	SDL_RWops *rw;
	if (append)
		rw = SDL_RWFromFile(filename.c_str(), "a");
	else
		rw = SDL_RWFromFile(filename.c_str(), "w");

	if (rw == NULL) return NULL;

	Sint64 nb_write_total = SDL_RWwrite(rw, buffer, 1, size);
	SDL_RWclose(rw);

	if (nb_write_total != size)
		return NULL;


	return 1;
}

bool ModuleFilesystem::Remove(const char * file)
{
	return remove(file) == 0;
}

bool ModuleFilesystem::Exists(const char * file) const
{
	SDL_RWops *rw = SDL_RWFromFile(file, "rb");
	return rw != NULL;
}

bool ModuleFilesystem::MakeDirectory(const char * directory)
{
	return create_directory(directory);
}

bool ModuleFilesystem::IsDirectory(const char * directory) const
{
	return is_directory(directory);
}

bool ModuleFilesystem::Copy(const char * source, const char * destination)
{
	char* buffer = Load("", source);
	if(buffer == NULL)
		return NULL;

	size_t len = SDL_strlen(buffer);
	bool succes = Save("", destination, buffer, len, false);
	delete[] buffer;
	return succes;
}
