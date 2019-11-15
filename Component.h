#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include "GameObject.h"

class Component
{
public:

	Component()
	{
	}

	virtual void Enable()
	{
		return;
	}

	virtual void Disable()
	{
		return;
	}

	virtual void Update()
	{

		return;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	ComponentType myType = TRANSFORM;
	bool isActive = false;

	//GameObject Parent: IMPORTANT, THIS POINTER MUST BE SYNC WITH PARENTS CHILDREN, BE CAREFUL
	GameObject* componentParent;
};

#endif __Component_H__