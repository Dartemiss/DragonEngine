#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"

const int BUCKET_CAPACITY = 2;

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
	MyQuadTree(AABB limits, int level);
	~MyQuadTree();

	void Clear();
	bool Insert(GameObject* go);
	void Subdivide();
	void ExpandingLimits(AABB* box);
	bool IsWithinQuad(AABB* go) const;

	void Draw() const;

	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);

	//Limits of the quadtree
	AABB limits;
	
	//Objects on that tree/leaf
	std::vector<GameObject*> gameObjects;
	
	std::vector<MyQuadTree*> nodes;
	int levelOfDepth = 0;
	int maxHeight = 0;
	
	//Children
	MyQuadTree* topLeft = nullptr;
	MyQuadTree* topRight = nullptr;
	MyQuadTree* bottomLeft = nullptr;
	MyQuadTree* bottomRight = nullptr;
	

};

#endif __MyQuadTree_H__