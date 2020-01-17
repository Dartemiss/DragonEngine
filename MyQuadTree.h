#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include <set>
#include "MathGeoLib/Geometry/AABB.h"

const int BUCKET_CAPACITY = 2;

class GameObject;

struct Node
{
	//Pointer to childs
	Node* parent = nullptr;
	Node* children[4];

	std::vector<GameObject*> gameObjects;

	bool isLeaf = true;

	AABB* quadrant = nullptr;

	int level = 0;
	unsigned int index = 0;
	
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
	bool InsertIterative(const std::vector<Node*> &posibleNodes, GameObject* go);
	bool IsWithinQuadrant(const AABB* quad, const AABB* go) const;
	void SubdivideIterative(Node* node, GameObject* go);
	void DrawIterative() const;
	bool GameObjectIsRepeated(const std::vector<GameObject*> &gameObjects, GameObject* go);
	void GetIntersection(std::set<GameObject*> &intersectionGO, AABB* bbox);

	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);

	//Limits of the quadtree
	AABB limits;
	
	//Objects on that tree/leaf
	std::vector<GameObject*> gameObjects;
	
	int levelOfDepth = 0;
	int maxHeight = 0;
	unsigned int currentMaxIndex = 0;
	
	//Children
	MyQuadTree* topLeft = nullptr;
	MyQuadTree* topRight = nullptr;
	MyQuadTree* bottomLeft = nullptr;
	MyQuadTree* bottomRight = nullptr;
	

	std::vector<Node*> nodes;

	

};

#endif __MyQuadTree_H__