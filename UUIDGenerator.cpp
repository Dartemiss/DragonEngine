#include "UUIDGenerator.h"
#include "Globals.h"

#include "Dependencies/PCG/pcg_basic.h"

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
	while (UUID == 0)
		UUID = pcg32_random();
	LOG("Generated UUID: %d.", UUID);
	return UUID;
}
