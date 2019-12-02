#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"

class GameObject;


struct Node
{
	//Pointer to childs
	Node* children[4];

	std::vector<GameObject*> gameObjects;

	int counter = 0;
	
};

class MyQuadTree
{
public:
	MyQuadTree();
	~MyQuadTree();

	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* go);
	//void CollectIntersect(std::vector<GameObject*>&, PRIMITIVE);


private:
	int bucket = 0;

};

#endif __MyQuadTree_H__