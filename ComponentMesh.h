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

	void LoadMesh(Mesh* myMesh);
	void Draw(const unsigned int program) const;

	float IsIntersectedByRay(const float3 &origin, const LineSegment &ray);

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	Mesh* mesh;
	void ProcessMeshData(const MeshData & data, Mesh & mesh);
	void DrawInspector();
};

#endif __ComponentMesh_H__