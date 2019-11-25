#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"

ComponentMesh::ComponentMesh()
{
	myType = MESH;
}


ComponentMesh::~ComponentMesh()
{
	for (auto mesh : meshes)
	{
		delete mesh;
	}

	delete boundingBox;
}

void ComponentMesh::Update()
{
	DrawAABB();

	return;
}

void ComponentMesh::LoadMeshes(const char * path)
{
	App->modelLoader->loadModel(path, meshes);
	App->modelLoader->emptyScene();
	LOG("GameObject have %d number of meshes.", meshes.size());
}

void ComponentMesh::ComputeBoundingBox()
{
	float3 min = float3::zero;
	float3 max = float3::zero;

	for (auto mesh : meshes)
	{
		for (auto vertex : mesh->vertices)
		{
			//Min vertex
			if (vertex.Position.x < min.x)
				min.x = vertex.Position.x;

			if (vertex.Position.y < min.y)
				min.y = vertex.Position.y;

			if (vertex.Position.z < min.y)
				min.z = vertex.Position.z;

			//Max vertex
			if (vertex.Position.x > max.x)
				max.x = vertex.Position.x;

			if (vertex.Position.y > max.y)
				max.y = vertex.Position.y;

			if (vertex.Position.z > max.z)
				max.z = vertex.Position.z;
		}
	}

	boundingBox = new AABB(min,max);

}

void ComponentMesh::DrawAABB()
{
}
