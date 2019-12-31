#ifndef __ModuleFilesystem_H__
#define __ModuleFilesystem_H__

#include "Module.h"

class ModuleFilesystem : public Module
{
public:
	ModuleFilesystem();
	~ModuleFilesystem();

	bool Load(const char* path, const char* file, char** buffer) const;
	bool Save(const char* path, const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);
	bool Exists(const char* file) const;
	bool MakeDirectory(const char* directory);
	bool IsDirectory(const char* directory) const;
	bool Copy(const char* source, const char* destination);
};

#endif __ModuleFilesystem_H__