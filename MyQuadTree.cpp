#include "MyQuadTree.h"
#include "GameObject.h"



MyQuadTree::MyQuadTree(AABB limits)
{
	this->limits = limits;
	maxHeight = 5;
	levelOfDepth = 1;

}

MyQuadTree::~MyQuadTree()
{
}

//Recursive
bool MyQuadTree::Insert(GameObject* go)
{
	const AABB* boundingBox = go->globalBoundingBox;

	if (!limits.Contains(*boundingBox))
	{
		LOG("Cannot insert elemment, it's outside the QuadTree limits.");
		return false;
	}

	//If there is space in this quad tree and if doesn't have subdivisions, add the object here
	if(gameObjects.size() < TREE_CAPACITY && topLeft == nullptr)
	{
		gameObjects.push_back(go);
		LOG("Game Object added correctly")
		return true;
	}

	//Otherwise, subdivide and then add the point to the corresponding node
	if (topLeft == nullptr)
		Subdivide();

	//We have to add the points / data contained into this quad array to the new quads if we want that only
	//the last node holds the data 

	if (topLeft->Insert(go)) return true;
	if (topRight->Insert(go)) return true;
	if (bottomLeft->Insert(go)) return true;
	if (bottomRight->Insert(go)) return true;

	// Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
	return false;
}

void MyQuadTree::Subdivide()
{
	float3 center = limits.CenterPoint();
	AABB* topLeft = new AABB(float3(limits.minPoint.x, 0, center.z), float3(center.x,0,limits.maxPoint.z));
	AABB* topRight = new AABB(center, limits.maxPoint);
	AABB* bottomLeft = new AABB(limits.minPoint, center);
	AABB* bottomRight = new AABB(float3(center.x, 0, limits.minPoint.z), float3(limits.maxPoint.x, 0, center.z));

	return;
}

