#ifndef __MyQuadTree_H__
#define __MyQuadTree_H__

#include "Globals.h"
#include <vector>
#include "include/Geometry/AABB.h"
#include "GameObject.h"
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