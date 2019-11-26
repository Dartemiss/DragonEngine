#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"
#include "include/Geometry/AABB.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Update();

	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	Mesh* mesh;

	

private:
	void DrawAABB();

};

#endif __ComponentMesh_H__