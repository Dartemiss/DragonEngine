#include "ModuleFilesystem.h"

#include "SDL/SDL.h"
#include <string>
#include <filesystem>

using namespace std;
using namespace std::tr2::sys;

ModuleFilesystem::ModuleFilesystem()
{
}


ModuleFilesystem::~ModuleFilesystem()
{
}

bool ModuleFilesystem::Load(const char * path, const char * file, char ** buffer) const
{
	string filename = path; filename += file;

	SDL_RWops *rw = SDL_RWFromFile(filename.c_str(), "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	
	*buffer = res;
	return 1;
}

bool ModuleFilesystem::Save(const char * path, const char * file, const char * buffer, unsigned int size, bool append) const
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
	char** buffer = new char*;
	if (!Load("", source, buffer))
		return NULL;

	size_t len = SDL_strlen(*buffer);
	return Save("", destination, *buffer, len, false);
}
