#ifndef __AABBTree_H__
#define __AABBTree_H__

#include "Globals.h"
#include "MathGeoLib/Geometry/AABB.h"
#include "GameObject.h"
#include <vector>
#include <set>
#include <map>


#define AABB_NULL_NODE 0xffffffff

struct NodeAABB
{
	AABB aabb;
	GameObject* go = nullptr;
	// tree links
	unsigned parentNodeIndex = AABB_NULL_NODE;
	unsigned leftNodeIndex = AABB_NULL_NODE;
	unsigned rightNodeIndex = AABB_NULL_NODE;
	// node linked list link
	unsigned nextNodeIndex = AABB_NULL_NODE;

	bool isLeaf() const { return leftNodeIndex == AABB_NULL_NODE; }

};

class AABBTree
{
public:
	AABBTree(unsigned initialSize);
	~AABBTree();


	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void UpdateObject(GameObject* go);

	void GetIntersection(std::set<GameObject*> &intersectionGO, AABB* bbox);
	
	void Draw() const;


	//Tree
	std::map<GameObject*, unsigned> objectNodeIndexMap;
	std::vector<NodeAABB> nodes;
	unsigned rootNodeIndex = AABB_NULL_NODE;
	unsigned allocatedNodeCount = 0;
	unsigned nextFreeNodeIndex = 0;
	unsigned nodeCapacity = 0;
	unsigned growthSize = 10;


private:
	AABB MergeAABB(const AABB &first, const AABB &second) const;
	unsigned AllocateNode();
	void DeallocateNode(unsigned nodeIndex);
	void InsertLeaf(unsigned leafNodeIndex);
	void FixUpwardsTree(unsigned treeNodeIndex);
	void RemoveLeaf(unsigned leafNodeIndex);
	void UpdateLeaf(unsigned leafNodeIndex, const AABB& newAaab);
};

#endif __AABBTree_H__