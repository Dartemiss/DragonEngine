#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"

class GameObject;


struct Node
{
	//Pointer to first child
	Node* firstChild = nullptr;

	int counter = 0;
	
};

class MyQuadTree
{
public:
	MyQuadTree();
	~MyQuadTree();

	void Create(const std::vector<AABB>& aabbs);
	void Clear();
	void Insert(GameObject* go);
	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);
};

#endif __MyQuadTree_H__