#include "Globals.h"
#include "LogStream.hpp"
#include <string>

using namespace Assimp;

class myStream :
	public LogStream
{
public:
	// Constructor
	myStream()
	{
		// empty
	}

	// Destructor
	~myStream()
	{
		// empty
	}
	// Write womethink using your own functionality
	void write(const char* message)
	{
		std::string str = message;
		std::size_t found = str.find_first_of("%\\");
		while (found != std::string::npos)
		{
			str[found] = '%\\';
			found = str.find_first_of("%\\", found + 1);
		}


		LOG(str.c_str());
	}
};