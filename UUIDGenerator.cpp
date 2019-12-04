#include "UUIDGenerator.h"
#include "Globals.h"

#include "Dependencies/PCG/pcg_basic.h"
#include <ctime>

UUIDGenerator::UUIDGenerator()
{
	LOG("UUID Generator initialized.");
}

UUIDGenerator::~UUIDGenerator()
{
}

unsigned int UUIDGenerator::getUUID()
{
	unsigned int UUID = pcg32_random();
	LOG("Generated UUID: %d.", UUID);
	return UUID;
}
