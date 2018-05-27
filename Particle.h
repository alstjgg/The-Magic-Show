#pragma once
#include "VECTOR3D.h"

/* Some physics constants */
#define SELF_FRICTION 0.07  //damping
#define TIME_STEPSIZE 0.2 // 한 프레임당 얼마만큼의 시간변화를 표현할 것인가


/* The particle class represents a particle of mass that can move around in 3D space*/
class Particle
{
public:
	bool movable; // can the particle move or not ? used to pin parts of the cloth

	float mass;
	VECTOR3D pos; // the current position of the particle in 3D space
	VECTOR3D old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
	VECTOR3D acceleration; // a vector representing the current acceleration of the particle
	VECTOR3D accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading

	Particle(VECTOR3D pos) : 
		pos(pos), old_pos(pos), acceleration(VECTOR3D(0, 0, 0)), mass(1), movable(true), accumulated_normal(VECTOR3D(0, 0, 0)) {}
	
	Particle() {}

	void addForce(VECTOR3D f)
	{
		acceleration += f / mass;
	}

	/* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
	The method is called by Cloth.time_step()
	Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
	void timeStep()
	{
		if (movable)
		{
			VECTOR3D temp = pos;
			pos = pos + (pos - old_pos) * (1-SELF_FRICTION) + acceleration * TIME_STEPSIZE;
			old_pos = temp;
			acceleration = VECTOR3D(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
		}
	}

	VECTOR3D& getPos() { return pos; }
	VECTOR3D& getOldPos() { return old_pos; }

	void resetAcceleration() { acceleration = VECTOR3D(0, 0, 0); }

	void offsetPos(const VECTOR3D v) { if (movable) pos += v; }

	void setPos(VECTOR3D v) { pos = v; }

	void makeUnmovable() { movable = false; }

	void makemovable() { movable = true; }

	void addToNormal(VECTOR3D normal)
	{
		accumulated_normal += normal.Normalize();
	}

	VECTOR3D& getNormal() { return accumulated_normal; } // notice, the normal is not unit length

	void resetNormal() { accumulated_normal = VECTOR3D(0, 0, 0); }

};
