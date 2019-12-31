#ifndef __Importer_H__
#define __Importer_H__

#include <string>

using namespace std;

class Importer
{
	virtual bool Import(const char* path, const char* file, std::string& output_file)
	{
		return false;
	}
	virtual bool Import(const char* file, const void* buffer, unsigned int size, std::string& output_file)
	{
		return false;
	}
};

#endif __Importer_H__