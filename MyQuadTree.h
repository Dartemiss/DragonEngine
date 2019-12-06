#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"

const int BUCKET_CAPACITY = 1;

class GameObject;


struct Node
{
	//Pointer to childs	
	Node* children[4];

	std::vector<GameObject*> gameObjects;

	bool isLeaf = true;

	AABB* quadrant = nullptr;

	int level = 0;
	
};

class MyQuadTree
{
public:
	//Recursive
	MyQuadTree(const AABB &limits, int level);
	//Iterative
	MyQuadTree(AABB* limits);
	~MyQuadTree();

	//Recursive
	void Clear();
	bool Insert(GameObject* go);
	void Subdivide();
	bool IsWithinQuad(const AABB* go) const;

	void Draw() const;

	//Iterative
	void ClearIterative();
	bool InsertIterative(GameObject* go);
	bool InsertAfterSubdividing(Node* newNodes[], GameObject* go);
	bool IsWithinQuadrant(const AABB* quad, const AABB* go) const;
	void SubdivideIterative(Node* node, GameObject* go);
	void DrawIterative() const;
	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);

	//Limits of the quadtree
	AABB limits;
	
	//Objects on that tree/leaf
	std::vector<GameObject*> gameObjects;
	
	int levelOfDepth = 0;
	int maxHeight = 0;
	
	//Children
	MyQuadTree* topLeft = nullptr;
	MyQuadTree* topRight = nullptr;
	MyQuadTree* bottomLeft = nullptr;
	MyQuadTree* bottomRight = nullptr;
	

	std::vector<Node*> nodes;

	

};

#endif __MyQuadTree_H__