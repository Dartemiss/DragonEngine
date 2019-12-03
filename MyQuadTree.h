#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"

const int TREE_CAPACITY = 4;

class GameObject;

/*
struct Node
{
	//Pointer to childs	
	Node* children[4];

	std::vector<GameObject*> gameObjects;

	int counter = 0;
	
};
*/
class MyQuadTree
{
public:
	MyQuadTree(AABB limits);
	~MyQuadTree();

	void Clear();
	bool Insert(GameObject* go);
	void Subdivide();
	void ExpandingLimits(AABB* box);

	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);

	//Limits of the quadtree
	AABB limits;
	
	//Objects on that tree/leaf
	std::vector<GameObject*> gameObjects;
	
	
	std::vector<MyQuadTree*> nodes;
	int bucketOLimit = 2;
	int levelOfDepth = 0;
	int maxHeight = 0;
	
	//Children
	MyQuadTree* topLeft;
	MyQuadTree* topRight;
	MyQuadTree* bottomLeft;
	MyQuadTree* bottomRight;
	
};

#endif __MyQuadTree_H__