#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* go);
	ComponentMesh(GameObject* go, ComponentMesh* comp);
	~ComponentMesh();

	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	Mesh* mesh;
};

#endif __ComponentMesh_H__