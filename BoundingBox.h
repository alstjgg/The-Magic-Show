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
		minimum = VECTOR3D(999999, 999999, 999999);
		maximum = VECTOR3D(-999999, -999999, -999999);
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
	임의로 bounding box의 높이를 설정해줌 
								형진: 		이거 왜 하는거야????
	if (result.maximum.x == result.minimum.x)
		result.maximum.x += (result.maximum.y - result.minimum.y);
	if (result.maximum.y == result.minimum.y)
		result.maximum.y += (result.maximum.z - result.minimum.z);
	if (result.maximum.z == result.minimum.z)
		result.maximum.z += (result.maximum.x - result.minimum.x);
	*/
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
	임의로 bounding box의 높이를 설정해줌 
	if (result.maximum.x == result.minimum.x)
		result.maximum.x += (result.maximum.y - result.minimum.y);
	if (result.maximum.y == result.minimum.y)
		result.maximum.y += (result.maximum.z - result.minimum.z);
	if (result.maximum.z == result.minimum.z)
		result.maximum.z += (result.maximum.x - result.minimum.x);
	*/
	return result;
}

bool BoundingBoxIntersect(Pair B1, Pair B2)
{	
	bool noOverlap = (B1.maximum.x < B2.minimum.x) || (B1.minimum.x > B2.maximum.x) ||
					 (B1.maximum.y < B2.minimum.y) || (B1.minimum.y > B2.maximum.y) ||
		             (B1.maximum.z < B2.minimum.z) || (B1.minimum.z > B2.minimum.z);
	return !(noOverlap);
}

void resolveSelfCollision(Pair B1, Pair B2, Triangle& T1, Triangle& T2)
{
	/* collision plane : xz 
	float correction = (B1.maximum.y - B2.minimum.y) / 2;
	VECTOR3D correctionVector(0, correction, 0);
	for (int i = 0; i < 3; i++)
	{
		T1.p[i]->offsetPos(-correctionVector);
		T2.p[i]->offsetPos(correctionVector);
	}
	*/
	// we must decide collision plane
	
	// so we have to find old boundingbox 
	pair B1_old;
	for (int i = 0; i < 3; i++)
	{
		B1_old.minimum.x = min(B1_old.minimum.x, T1.p[0]->getOldPos().x);
		B1_old.minimum.y = min(B1_old.minimum.y, T1.p[i]->getOldPos().y);
		B1_old.minimum.z = min(B1_old.minimum.z, T1.p[i]->getOldPos().z);

		B1_old.maximum.x = max(B1_old.maximum.x, T1.p[i]->getOldPos().x);
		B1_old.maximum.y = max(B1_old.maximum.y, T1.p[i]->getOldPos().y);
		B1_old.maximum.z = max(B1_old.maximum.z, T1.p[i]->getOldPos().z);
	}
	
	pair B2_old;
	for (int i = 0; i < 3; i++)
	{
		B2_old.minimum.x = min(B2_old.minimum.x, T2.p[0]->getOldPos().x);
		B2_old.minimum.y = min(B2_old.minimum.y, T2.p[i]->getOldPos().y);
		B2_old.minimum.z = min(B2_old.minimum.z, T2.p[i]->getOldPos().z);

		B2_old.maximum.x = max(B2_old.maximum.x, T2.p[i]->getOldPos().x);
		B2_old.maximum.y = max(B2_old.maximum.y, T2.p[i]->getOldPos().y);
		B2_old.maximum.z = max(B2_old.maximum.z, T2.p[i]->getOldPos().z);
	}
	
	//yz plane collision?
	float x_dist, x_mov, x_time,x_collisionPlane;
	if (B1_old.maximum.x < B2_old.minimum.x) {
		x_dist = B2_old.minimum.x - B1_old.maximum.x;
		x_mov = (B1.maximum.x - B1_old.maximum.x) - (B2.minimum.x - B2_old.minimum.x);
		x_time = x_dist / x_mov;
		x_collisionPlane = B1_old.maximum.x + (B1.maximum.x - B1_old.maximum.x)*x_time;
	}
	else { // B1_old.minimum.x > B2.old.maximum.x
		x_dist = B1_old.minimum.x - B2_old.maximum.x;
		x_mov = (B2.maximum.x - B2_old.maximum.x) - (B1.minimum.x - B1_old.minimum.x);
		x_time = x_dist / x_mov;
		x_collisionPlane = B2_old.maximum.x + (B2.maximum.x - B2_old.maximum.x)*x_time;
	}
	
	//xz plane collision?
	float y_dist, y_mov, y_time,y_collisionPlane;
	if (B1_old.maximum.y < B2_old.minimum.y) {
		y_dist = B2_old.minimum.y - B1_old.maximum.y;
		y_mov = (B1.maximum.y - B1_old.maximum.y) - (B2.minimum.y - B2_old.minimum.y);
		y_time = y_dist / y_mov;
		y_collisionPlane = B1_old.maximum.y + (B1.maximum.y - B1_old.maximum.y)*y_time;
	}
	else { // B1_old.minimum.y > B2.old.maximum.y
		y_dist = B1_old.minimum.y - B2_old.maximum.y;
		y_mov = (B2.maximum.y - B2_old.maximum.y) - (B1.minimum.y - B1_old.minimum.y);
		y_time = y_dist / y_mov;
		y_collisionPlane = B2_old.maximum.y + (B2.maximum.y - B2_old.maximum.y)*y_time;
	}

	//xy plane collision?
	float z_dist, z_mov, z_time,z_collisionPlane;
	if (B1_old.maximum.z < B2_old.minimum.z) {
		z_dist = B2_old.minimum.z - B1_old.maximum.z;
		z_mov = (B1.maximum.z - B1_old.maximum.z) - (B2.minimum.z - B2_old.minimum.z);
		z_time = z_dist / z_mov;
		z_collisionPlane = B1_old.maximum.z + (B1.maximum.z - B1_old.maximum.z)*z_time;
	}
	else { // B1_old.minimum.z > B2.old.maximum.z
		z_dist = B1_old.minimum.z - B2_old.maximum.z;
		z_mov = (B2.maximum.z - B2_old.maximum.z) - (B1.minimum.z - B1_old.minimum.z);
		z_time = z_dist / z_mov;\
		z_collisionPlane = B2_old.maximum.z + (B2.maximum.z - B2_old.maximum.z)*z_time;
	}
	
	
	float min_time = min(x_time, min(y_time, z_time));
	VECTOR3D collisionPlane(x_collisionPlane*(min_time == x_time), y_collisionPlane*(min_time == y_time), z_collisionPlane*(min_time == z_time));
	//collisionPlane vector3D contain info about axis and which point should it cross
	VECTOR3D NormalizedCollisionPlane = collisionPlane.Normalize();

	for (int i = 0; i < 3; i++) {

		if ((T1.p[i]->getOldPos().InnerProduct(NormalizedCollisionPlane) - collisionPlane.Magnitude()) *
			(T1.p[i]->getPos().InnerProduct(NormalizedCollisionPlane) - collisionPlane.Magnitude()) < 0) {//collisionplane is between old_pos and pos 

			

			T1.p[i]->setPos(T1.p[i]->getPos() - collisionPlane.Normalize() * T1.p[i]->getPos().InnerProduct(collisionPlane.Normalize()) + collisionPlane);
		}


		if ((T2.p[i]->getOldPos().InnerProduct(NormalizedCollisionPlane) - collisionPlane.Magnitude()) *
			(T2.p[i]->getPos().InnerProduct(NormalizedCollisionPlane) - collisionPlane.Magnitude()) < 0) {//collisionplane is between old_pos and pos 

			T2.p[i]->setPos(T2.p[i]->getPos() - collisionPlane.Normalize() * T2.p[i]->getPos().InnerProduct(collisionPlane.Normalize()) + collisionPlane);
		}
	}
	
}