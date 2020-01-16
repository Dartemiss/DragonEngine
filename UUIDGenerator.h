#ifndef __UUIDGenerator_H__
#define __UUIDGenerator_H__

class UUIDGenerator
{
public:
	UUIDGenerator();
	~UUIDGenerator();

	unsigned int getUUID() const;
};

extern UUIDGenerator * UUIDGen;

#endif __UUIDGenerator_H__