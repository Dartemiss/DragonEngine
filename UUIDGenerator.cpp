#include "UUIDGenerator.h"
#include "Globals.h"

#include "PCG/pcg_basic.h"

UUIDGenerator::UUIDGenerator()
{
	LOG("UUID Generator initialized.");
}

UUIDGenerator::~UUIDGenerator()
{
}

unsigned int UUIDGenerator::getUUID() const
{
	unsigned int UUID = pcg32_random();
	while (UUID == 0)
		UUID = pcg32_random();
	LOG("Generated UUID: %u.", UUID);
	return UUID;
}
