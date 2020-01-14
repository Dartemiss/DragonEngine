#include "MyQuadTree.h"
#include "GameObject.h"
#include "debugdraw.h"
#include <stack>
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
		//LOG("Cannot insert elemment, it's outside the QuadTree limits.");
		return false;
	}

	//If there is space in this quad tree and if doesn't have subdivisions, add the object here
	if(gameObjects.size() < BUCKET_CAPACITY && topLeft == nullptr)
	{
		gameObjects.push_back(go);
		return true;
	}

	//Otherwise, subdivide and then add the point to the corresponding node
	if (topLeft == nullptr)
		Subdivide();

	if (levelOfDepth >= 5)
	{
		gameObjects.push_back(go);
		return true;
	}

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
		//LOG("GameObject is on the left outside of quadtree.");
		return false;
	}
	else if(limits.minPoint.z > go->maxPoint.z)
	{
		//LOG("GameObject is on the top outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.x < go->minPoint.x)
	{
		//LOG("GameObject is on the right outside of quadtree.");
		return false;
	}
	else if(limits.maxPoint.z < go->minPoint.z)
	{
		//LOG("GameObject is on the bottom outside of quadtree.");
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

bool MyQuadTree::InsertIterative(const std::vector<Node*> &posibleNodes, GameObject * go)
{
	assert(go != nullptr);

	AABB* boundingBox = go->globalBoundingBox;
	
	for(auto node : posibleNodes)
	{

		if(node->isLeaf && IsWithinQuadrant(node->quadrant, boundingBox))
		{
			
			if(node->gameObjects.size() < BUCKET_CAPACITY)
			{
				node->gameObjects.push_back(go);
			}
			else if (!GameObjectIsRepeated(node->gameObjects, go))
			{
				SubdivideIterative(node, go);
				break;
			}

		}
	}
	
	return true;
}


bool MyQuadTree::IsWithinQuadrant(const AABB* quad, const AABB* go) const
{
	if (quad->minPoint.x > go->maxPoint.x)
	{
		//LOG("GameObject is on the left outside of quadtree.");
		return false;
	}
	else if (quad->minPoint.z > go->maxPoint.z)
	{
		//LOG("GameObject is on the top outside of quadtree.");
		return false;
	}
	else if (quad->maxPoint.x < go->minPoint.x)
	{
		//LOG("GameObject is on the right outside of quadtree.");
		return false;
	}
	else if (quad->maxPoint.z < go->minPoint.z)
	{
		//LOG("GameObject is on the bottom outside of quadtree.");
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

	++currentMaxIndex;

	node->isLeaf = false;
	//Top Left
	node->children[0] = new Node();
	node->children[0]->index = currentMaxIndex;
	++currentMaxIndex;
	node->children[0]->parent = node;
	node->children[0]->quadrant = new AABB(node->quadrant->minPoint, center);
	node->children[0]->level = node->level + 1;
	//Top Right
	node->children[1] = new Node();
	node->children[1]->index = currentMaxIndex;
	++currentMaxIndex;
	node->children[1]->parent = node;
	node->children[1]->quadrant = new AABB(float3(center.x, 0, node->quadrant->minPoint.z), float3(node->quadrant->maxPoint.x, 0, center.z));
	node->children[1]->level = node->level + 1;
	//Bottom Left
	node->children[2] = new Node();
	node->children[2]->index = currentMaxIndex;
	++currentMaxIndex;
	node->children[2]->parent = node;
	node->children[2]->quadrant = new AABB(float3(node->quadrant->minPoint.x, 0, center.z), float3(center.x, 0, node->quadrant->maxPoint.z));
	node->children[2]->level = node->level + 1;
	//BottomRight
	node->children[3] = new Node();
	node->children[3]->index = currentMaxIndex;
	++currentMaxIndex;
	node->children[3]->parent = node;
	node->children[3]->quadrant = new AABB(center, node->quadrant->maxPoint);
	node->children[3]->level = node->level + 1;

	nodes.push_back(node->children[0]);
	nodes.push_back(node->children[1]);
	nodes.push_back(node->children[2]);
	nodes.push_back(node->children[3]);

	std::vector<Node*> posibleNodes;

	for(int i = 0; i < 4; ++i)
	{
		posibleNodes.push_back(node->children[i]);
	}
	
	for(auto gameObject: node->gameObjects)
	{
		InsertIterative(posibleNodes,gameObject);
	}

	node->gameObjects.clear();

	//Try to insert again
	InsertIterative(posibleNodes,go);

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

bool MyQuadTree::GameObjectIsRepeated(const std::vector<GameObject*>& allGameObjects, GameObject * go)
{
	for(auto game : allGameObjects)
	{
		if (game == go)
			return true;
	}

	return false;
}

void MyQuadTree::GetIntersection(std::set<GameObject*>& intersectionGO, AABB* bbox)
{
	if(nodes[0]->isLeaf)
	{
		for(auto go : nodes[0]->gameObjects)
		{
			intersectionGO.insert(go);
		}

		return;
	}

	Node* current = nullptr;
	Node* previous = nullptr;

	std::stack<int> indexes;
	std::stack<Node*> stackOfNodes;
	current = nodes[0];
	indexes.push(0);
	stackOfNodes.push(current);
	int times = 0;
	while(true)
	{
		++times;
		if(bbox->Intersects(*current->quadrant))
		{
			if(current->isLeaf)
			{
				for(auto go: current->gameObjects)
				{
					intersectionGO.insert(go);
				}
				

				int index = indexes.top() + 1;
				indexes.pop();
				indexes.push(index);

				current = previous->children[index];

				if(index == 3)
				{
					indexes.pop();
					stackOfNodes.pop();
					if (!stackOfNodes.empty())
						previous = stackOfNodes.top();
				}
			}
			else
			{
				indexes.push(0);
				previous = current;
				stackOfNodes.push(previous);
				current = previous->children[0];
			}

			

		}
		else
		{
			int index = indexes.top() + 1;
			indexes.pop();
			indexes.push(index);
			current = previous->children[index];

			if (index == 3)
			{
				indexes.pop();
				stackOfNodes.pop();
				if(!stackOfNodes.empty())
					previous = stackOfNodes.top();
			}

		}
	

		if (indexes.size() == 0 || stackOfNodes.size() == 0)
		{
			//LOG("Times checked: %d", times);
			return;
		}
			

	}

	return;
}

