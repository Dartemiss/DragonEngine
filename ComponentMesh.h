#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"
#include "MathGeoLib/Geometry/AABB.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* go);
	ComponentMesh(GameObject* go, ComponentMesh* comp);
	~ComponentMesh();

	void Update();

	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	float IsIntersectedByRay(const float3 &origin, const LineSegment &ray);

	//Saving and loading
	void OnSave(SceneLoader & loader);
	void OnLoad(SceneLoader & loader);

	Mesh* mesh;
};

#endif __ComponentMesh_H__