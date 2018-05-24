#pragma once
#include "Particle.h"
#define PI 3.141592

class Triangle
{
public:
	Particle * p[3];
	VECTOR3D normal;
	VECTOR3D repulsive;
	float D = 0;
	
	Triangle(Particle *p1, Particle *p2, Particle *p3)
	{
		p[0] = p1;
		p[1] = p2;
		p[2] = p3;
		normal = VECTOR3D(0, 1, 0);
	}

	Triangle() {}

	void calcTriangleNormal()
	{
		VECTOR3D pos1 = p[0]->getPos();
		VECTOR3D pos2 = p[1]->getPos();
		VECTOR3D pos3 = p[2]->getPos();

		VECTOR3D v1 = pos2 - pos1;
		VECTOR3D v2 = pos3 - pos1;

		// Face normal
		normal = v1.CrossProduct(v2);
	}

	/* plane vector equation : P¡¤N - D = 0*/
	void calcD()
	{
		D = normal.InnerProduct(p[0]->getPos());
	}

	void calcRepulsive()
	{
		repulsive = VECTOR3D(0, 0, 0);
		for (int i = 0; i < 3; i++)
		{
			repulsive += p[i]->getOldPos() - p[i]->getPos();
		}
	}
};

