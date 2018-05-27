#pragma once
#include "Particle.h"
#define elasticity 0.5 //0~1
#define CONSTRAINT_ITERATIONS 10 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)

class Constraint
{
public:
	Particle *p1, *p2; // the two particles that are connected through this constraint
	float rest_distance; // original distance

	Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2)
	{
		VECTOR3D vec = p1->getPos() - p2->getPos();
		rest_distance = vec.Magnitude();
	}

	/* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
	the method is called by Cloth.time_step() many times per frame*/
	void satisfyConstraint()
	{
		VECTOR3D p1_to_p2 = p2->getPos() - p1->getPos(); // vector from p1 to p2
		float current_distance = p1_to_p2.Magnitude(); // current distance between p1 and p2
		VECTOR3D correctionVector = p1_to_p2.Normalize() *(current_distance - rest_distance) * elasticity; // The offset vector that could move p1 into a distance of rest_distance to p2
		p1->offsetPos(correctionVector); // restore
		p2->offsetPos(-correctionVector); //restore
	}
};