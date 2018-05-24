#pragma once
#include "Tree.h"
#include "Triangle.h"
#include "VECTOR3D.h"

#include <vector>

typedef struct pair {
	VECTOR3D minimum; //point of minimum bound
	VECTOR3D maximum; //point of maximum bound

	/* initially, infinite bound */
	pair()
	{
		minimum = VECTOR3D(999, 999, 999);
		maximum = VECTOR3D(-999, -999, -999);
	}
}Pair;

/* resulsivly search for minimum bound position with given node */
void searchMin(Node *node, VECTOR3D& result, std::vector<Triangle>& T)
{
	if (node->data == -1) //not leaf node
	{
		searchMin(node->left, result, T);
		searchMin(node->right, result, T);
	}

	else
	{
		for (int i = 0; i < 3; i++)
		{
			result.x = min(result.x, T[node->data].p[i]->getPos().x);
			result.y = min(result.y, T[node->data].p[i]->getPos().y);
			result.z = min(result.z, T[node->data].p[i]->getPos().z);
			return;
		}
	}
}

/* resulsivly search for maximum bound position with given node */
void searchMax(Node *node, VECTOR3D& result, std::vector<Triangle>& T)
{
	if (node->data == -1) //not leaf node
	{
		searchMax(node->left, result, T);
		searchMax(node->right, result, T);
	}

	else
	{
		for (int i = 0; i < 3; i++)
		{
			result.x = max(result.x, T[node->data].p[i]->getPos().x);
			result.y = max(result.y, T[node->data].p[i]->getPos().y);
			result.z = max(result.z, T[node->data].p[i]->getPos().z);
			return;
		}
	}
}

/* for HierarchicalSelfCollision resolve */
Pair makeBoundingBoxforNode(Node* node, std::vector<Triangle>& T)
{
	Pair result;

	searchMax(node, result.maximum, T);
	searchMin(node, result.minimum, T);

	return result;
}

/* for RawSelfCollision resolve */
Pair makeBoundingBoxforTriangle(Triangle& T)
{
	Pair result;

	for (int i = 0; i < 3; i++)
	{
		result.minimum.x = min(result.minimum.x, T.p[i]->getPos().x);
		result.minimum.y = min(result.minimum.y, T.p[i]->getPos().y);
		result.minimum.z = min(result.minimum.z, T.p[i]->getPos().z);

		result.maximum.x = max(result.minimum.x, T.p[i]->getPos().x);
		result.maximum.y = max(result.minimum.y, T.p[i]->getPos().y);
		result.maximum.z = max(result.minimum.z, T.p[i]->getPos().z);
	}
	return result;
}

bool BoundingBoxIntersect(Pair B1, Pair B2)
{
	return B1.maximum.x < B2.minimum.x && B1.maximum.y < B2.minimum.y && B1.maximum.z < B2.minimum.z;
}

void resolveSelfCollision(Pair B1, Pair B2, Triangle& T1, Triangle& T2)
{

}