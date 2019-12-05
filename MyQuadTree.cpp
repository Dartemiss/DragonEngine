#include "MyQuadTree.h"
#include "GameObject.h"
#include "debugdraw.h"


MyQuadTree::MyQuadTree(AABB limits, int level)
{
	this->limits = limits;
	maxHeight = 5;
	levelOfDepth = level;

}

MyQuadTree::~MyQuadTree()
{
}

void MyQuadTree::Clear()
{
	if (topLeft != nullptr)
		topLeft->Clear();

	if (topRight != nullptr)
		topRight->Clear();

	if (bottomLeft != nullptr)
		bottomLeft->Clear();

	if (bottomRight != nullptr)
		bottomRight->Clear();

	delete topLeft;
	delete topRight;
	delete bottomLeft;
	delete bottomRight;

	gameObjects.clear();

	topLeft = nullptr;
	topRight = nullptr;
	bottomLeft = nullptr;
	bottomRight = nullptr;

}

//Recursive
bool MyQuadTree::Insert(GameObject* go)
{
	AABB* boundingBox = go->globalBoundingBox;

	if (!IsWithinQuad(boundingBox))
	{
		LOG("Cannot insert elemment, it's outside the QuadTree limits.");
		return false;
	}

	//If there is space in this quad tree and if doesn't have subdivisions, add the object here
	if(gameObjects.size() < BUCKET_CAPACITY && topLeft == nullptr)
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
	//Z axis is pointing to negative (top)
	topLeft = new MyQuadTree(AABB(limits.minPoint, center), levelOfDepth + 1);
	topRight = new MyQuadTree(AABB(float3(center.x, 0, limits.minPoint.z), float3(limits.maxPoint.x, 0, center.z)), levelOfDepth + 1);
	bottomLeft = new MyQuadTree(AABB(float3(limits.minPoint.x, 0, center.z), float3(center.x,0,limits.maxPoint.z)), levelOfDepth + 1);
	bottomRight = new MyQuadTree(AABB(center, limits.maxPoint), levelOfDepth + 1);

	for(auto go : gameObjects)
	{
		topLeft->Insert(go);
		topRight->Insert(go);
		bottomLeft->Insert(go);
		bottomRight->Insert(go);
	}

	gameObjects.clear();

	return;
}

bool MyQuadTree::IsWithinQuad(AABB* go) const
{
	if(limits.minPoint.x > go->minPoint.x)
	{
		LOG("GameObject is on the left outside of quadtree.");
		return false;
	}
	else if(limits.minPoint.z > go->minPoint.z)
	{
		LOG("GameObject is on the top outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.x < go->maxPoint.x)
	{
		LOG("GameObject is on the right outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.z < go->maxPoint.z)
	{
		LOG("GameObject is on the bottom outside of quadtree.");
		return false;
	}
	
	return true;
}

void MyQuadTree::Draw() const
{
	dd::aabb(limits.minPoint,limits.maxPoint,float3(1.0f,0.5f,0.5f));

	//Draw all childs
	if (topLeft != nullptr)
		topLeft->Draw();

	if (topRight != nullptr)
		topRight->Draw();

	if (bottomLeft != nullptr)
		bottomLeft->Draw();

	if (bottomRight != nullptr)
		bottomRight->Draw();

}

