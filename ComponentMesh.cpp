#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"

ComponentMesh::ComponentMesh()
{
	myType = MESH;
}


ComponentMesh::~ComponentMesh()
{
	delete mesh;
	
}

void ComponentMesh::Update()
{

	return;
}

void ComponentMesh::LoadMesh(Mesh* loadedMesh)
{
	mesh = loadedMesh;
}

void ComponentMesh::Draw(const unsigned int program) const
{
	mesh->Draw(program);
}




