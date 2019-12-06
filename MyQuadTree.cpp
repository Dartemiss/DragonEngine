#include "MyQuadTree.h"
#include "GameObject.h"
#include "debugdraw.h"
#include <assert.h>

//Recursive Constructor
MyQuadTree::MyQuadTree(const AABB &limits, int level)
{
	this->limits = limits;
	maxHeight = 5;
	levelOfDepth = level;

}

MyQuadTree::MyQuadTree(AABB* limits)
{
	assert(limits != nullptr);

	Node* root = new Node();
	root->quadrant = limits;

	nodes.push_back(root);
}

MyQuadTree::~MyQuadTree()
{
	Clear();
	ClearIterative();
	if(nodes.size() == 1)
	{
		delete nodes[0]->quadrant;
		delete nodes[0];
	}
		
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
	assert(go != nullptr);

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

bool MyQuadTree::IsWithinQuad(const AABB* go) const
{
	assert(go != nullptr);

	if(limits.minPoint.x > go->maxPoint.x)
	{
		LOG("GameObject is on the left outside of quadtree.");
		return false;
	}
	else if(limits.minPoint.z > go->maxPoint.z)
	{
		LOG("GameObject is on the top outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.x < go->minPoint.x)
	{
		LOG("GameObject is on the right outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.z < go->minPoint.z)
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

void MyQuadTree::ClearIterative()
{
	Node* root = new Node();
	root->quadrant = nodes[0]->quadrant;

	nodes[0]->gameObjects.clear();
	delete nodes[0];

	for(unsigned int i = 1; i < nodes.size();++i)
	{
		nodes[i]->gameObjects.clear();
		delete nodes[i]->quadrant;
		delete nodes[i];
	}

	nodes.clear();
	nodes.push_back(root);

	return;
}

bool MyQuadTree::InsertIterative(GameObject * go)
{
	assert(go != nullptr);

	AABB* boundingBox = go->globalBoundingBox;
	
	for(auto node : nodes)
	{

		if(node->isLeaf && IsWithinQuadrant(node->quadrant, boundingBox))
		{
			if(node->gameObjects.size() < BUCKET_CAPACITY)
			{
				node->gameObjects.push_back(go);
			}
			else
			{
				SubdivideIterative(node, go);
				break;
			}

		}
	}
	
	return true;
}

bool MyQuadTree::InsertAfterSubdividing(Node* newNodes[], GameObject * go)
{
	assert(go != nullptr);

	AABB* boundingBox = go->globalBoundingBox;

	for (int i = 0; i< 4; ++i)
	{
		if (newNodes[i]->isLeaf && IsWithinQuadrant(newNodes[i]->quadrant, boundingBox))
		{
			if (newNodes[i]->gameObjects.size() < BUCKET_CAPACITY)
			{
				newNodes[i]->gameObjects.push_back(go);
			}
			else
			{
				SubdivideIterative(newNodes[i], go);
				break;
			}

			++newNodes;
		}
	}

	return true;
}

bool MyQuadTree::IsWithinQuadrant(const AABB* quad, const AABB* go) const
{
	if (quad->minPoint.x > go->maxPoint.x)
	{
		LOG("GameObject is on the left outside of quadtree.");
		return false;
	}
	else if (quad->minPoint.z > go->maxPoint.z)
	{
		LOG("GameObject is on the top outside of quadtree.");
		return false;
	}
	else if (quad->maxPoint.x < go->minPoint.x)
	{
		LOG("GameObject is on the right outside of quadtree.");
		return false;
	}
	else if (quad->maxPoint.z < go->minPoint.z)
	{
		LOG("GameObject is on the bottom outside of quadtree.");
		return false;
	}

	return true;
}

void MyQuadTree::SubdivideIterative(Node* node, GameObject* go)
{
	if(node->level >= 5)
	{
		node->gameObjects.push_back(go);
		return;
	}

	float3 center = node->quadrant->CenterPoint();
	//Z axis is pointing to negative (top)

	node->isLeaf = false;
	//Top Left
	node->children[0] = new Node();
	node->children[0]->quadrant = new AABB(node->quadrant->minPoint, center);
	node->children[0]->level = node->level + 1;
	//Top Right
	node->children[1] = new Node();
	node->children[1]->quadrant = new AABB(float3(center.x, 0, node->quadrant->minPoint.z), float3(node->quadrant->maxPoint.x, 0, center.z));
	node->children[1]->level = node->level + 1;
	//Bottom Left
	node->children[2] = new Node();
	node->children[2]->quadrant = new AABB(float3(node->quadrant->minPoint.x, 0, center.z), float3(center.x, 0, node->quadrant->maxPoint.z));
	node->children[2]->level = node->level + 1;
	//BottomRight
	node->children[3] = new Node();
	node->children[3]->quadrant = new AABB(center, node->quadrant->maxPoint);
	node->children[3]->level = node->level + 1;

	
	for(auto gameObject: node->gameObjects)
	{
		InsertAfterSubdividing(node->children, gameObject);
	}

	node->gameObjects.clear();

	//Try to insert again
	InsertAfterSubdividing(node->children,go);

	nodes.push_back(node->children[0]);
	nodes.push_back(node->children[1]);
	nodes.push_back(node->children[2]);
	nodes.push_back(node->children[3]);

	return;
}

void MyQuadTree::DrawIterative() const
{
	for(auto node : nodes)
	{
		dd::aabb(node->quadrant->minPoint, node->quadrant->maxPoint, float3(1.0f, 0.5f, 0.5f));
	}

	return;
}

