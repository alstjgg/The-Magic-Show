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

	/* 삼각형의 법선벡터가 한 축과 평행하게 놓이는 경우,
	해당 축 방향으로 bounding box의 높이를 만들 수 없으므로
	임의로 bounding box의 높이를 설정해줌 */
	if (result.maximum.x == result.minimum.x)
		result.maximum.x += (result.maximum.y - result.minimum.y);
	if (result.maximum.y == result.minimum.y)
		result.maximum.y += (result.maximum.z - result.minimum.z);
	if (result.maximum.z == result.minimum.z)
		result.maximum.z += (result.maximum.x - result.minimum.x);

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

		result.maximum.x = max(result.maximum.x, T.p[i]->getPos().x);
		result.maximum.y = max(result.maximum.y, T.p[i]->getPos().y);
		result.maximum.z = max(result.maximum.z, T.p[i]->getPos().z);
	}

	/* 삼각형의 법선벡터가 한 축과 평행하게 놓이는 경우, 
	해당 축 방향으로 bounding box의 높이를 만들 수 없으므로 
	임의로 bounding box의 높이를 설정해줌 */
	if (result.maximum.x == result.minimum.x)
		result.maximum.x += (result.maximum.y - result.minimum.y);
	if (result.maximum.y == result.minimum.y)
		result.maximum.y += (result.maximum.z - result.minimum.z);
	if (result.maximum.z == result.minimum.z)
		result.maximum.z += (result.maximum.x - result.minimum.x);

	return result;
}

bool BoundingBoxIntersect(Pair B1, Pair B2)
{
	return (B1.maximum.x > B2.minimum.x) && (B1.maximum.y > B2.minimum.y) && (B1.maximum.z > B2.minimum.z)
		&& (B1.minimum.x < B2.maximum.x) && (B1.minimum.y < B2.maximum.y) && (B1.minimum.z < B2.maximum.z);
}

void resolveSelfCollision(Pair B1, Pair B2, Triangle& T1, Triangle& T2)
{
	/* collision plane : xz */
	float correction = (B1.maximum.y - B2.minimum.y) / 2;
	VECTOR3D correctionVector(0, correction, 0);
	for (int i = 0; i < 3; i++)
	{
		T1.p[i]->offsetPos(-correctionVector);
		T2.p[i]->offsetPos(correctionVector);
	}
}