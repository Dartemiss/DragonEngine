#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Update();

	void LoadMeshes(const char* path);


	std::vector<Mesh*> meshes;

};

#endif __ComponentMesh_H__