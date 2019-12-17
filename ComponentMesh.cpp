#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "SceneLoader.h"
#include "Dependencies/MathGeoLib/include/Geometry/LineSegment.h"
#include "Dependencies/MathGeoLib/include/Geometry/Triangle.h"

ComponentMesh::ComponentMesh(GameObject* go)
{
	myGameObject = go;
	myType = MESH;
}

ComponentMesh::ComponentMesh(GameObject * go, ComponentMesh * comp)
{
	myGameObject = go;
	myType = MESH;
	mesh = comp->mesh;
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

float ComponentMesh::IsIntersectedByRay(const float3 &origin ,const LineSegment & ray)
{
	float minDist = -1.0f;

	for(int i = 0; i < mesh->indices.size();i += 3)
	{

		Triangle tri = Triangle(mesh->vertices[mesh->indices[i]].Position, mesh->vertices[mesh->indices[i+1]].Position, mesh->vertices[mesh->indices[i+2]].Position);
		if(tri.Intersects(tri))
		{
			minDist = tri.Distance(origin);
		}
	}


	return minDist;
}

void ComponentMesh::OnSave(SceneLoader & loader)
{
	loader.AddUnsignedInt("Type", myType);
	//TODO implement save
}

void ComponentMesh::OnLoad(SceneLoader & loader)
{
	//TODO implement load
}




