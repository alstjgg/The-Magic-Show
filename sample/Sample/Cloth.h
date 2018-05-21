#pragma once
#include <vector>
#include "Constraint.h"
#include "Triangle.h"
#include <gl/GL.h>
#include <gl/glut.h> 


class Cloth
{
public:

	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
							  // total number of particles is num_particles_width*num_particles_height

	std::vector<Particle> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // all constraints between particles as part of this cloth
	std::vector<Triangle> triangles; //  there are 2 * (num_particles_width-1) * (num_particles_height-1) triangles

	Particle* getParticle(int x, int z) 
	{ return &particles[z*num_particles_width + x]; }
	
	void makeConstraint(Particle *p1, Particle *p2) 
	{ constraints.push_back(Constraint(p1, p2)); }

	void makeTriangle(Particle *p1, Particle *p2, Particle *p3)
	{ triangles.push_back(Triangle(p1, p2, p3)); }

	void addWindForcesForTriangle(Triangle T, const VECTOR3D direction)
	{
		VECTOR3D normal = T.normal;
		VECTOR3D d = normal.Normalize();
		VECTOR3D force = normal*(d.InnerProduct(direction));
		T.p[0]->addForce(force);
		T.p[1]->addForce(force);
		T.p[2]->addForce(force);
	}

	void drawTriangle(Triangle T, const VECTOR3D color)
	{
		glColor3fv((GLfloat*)&color);

		glNormal3fv((GLfloat *) &(T.p[0]->getNormal().Normalize()));
		glVertex3fv((GLfloat *) &(T.p[0]->getPos()));

		glNormal3fv((GLfloat *) &(T.p[1]->getNormal().Normalize()));
		glVertex3fv((GLfloat *) &(T.p[1]->getPos()));

		glNormal3fv((GLfloat *) &(T.p[2]->getNormal().Normalize()));
		glVertex3fv((GLfloat *) &(T.p[2]->getPos()));
	}

	Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
	{
		particles.resize(num_particles_width*num_particles_height);

		//particle »ý¼º
		for (int x = 0; x < num_particles_width; x++)
		{
			for (int z = 0; z < num_particles_height; z++)
			{
				VECTOR3D pos = VECTOR3D(width * (x / (float)num_particles_width), 0, height * (z / (float)num_particles_height));
				particles[z*num_particles_width + x] = Particle(pos);
			}
		}

		// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
		for (int x = 0; x < num_particles_width; x++)
		{
			for (int z = 0; z < num_particles_height; z++)
			{
				if (x < num_particles_width - 1) makeConstraint(getParticle(x, z), getParticle(x + 1, z));
				if (z < num_particles_height - 1) makeConstraint(getParticle(x, z), getParticle(x, z + 1));
				if (x < num_particles_width - 1 && z < num_particles_height - 1) makeConstraint(getParticle(x, z), getParticle(x + 1, z + 1));
				if (x < num_particles_width - 1 && z < num_particles_height - 1) makeConstraint(getParticle(x + 1, z), getParticle(x, z + 1));
			}
		}

		// Connecting secondary neighbors with constraints (distance 2 and sqrt(8) in the grid)
		for (int x = 0; x < num_particles_width-1; x++)
		{
			for (int z = 0; z < num_particles_height-1; z++)
			{
				if (x < num_particles_width - 2) makeConstraint(getParticle(x, z), getParticle(x + 2, z));
				if (z < num_particles_height - 2) makeConstraint(getParticle(x, z), getParticle(x, z + 2));
				if (x < num_particles_width - 2 && z < num_particles_height - 2) makeConstraint(getParticle(x, z), getParticle(x + 2, z + 2));
				if (x < num_particles_width - 2 && z < num_particles_height - 2) makeConstraint(getParticle(x + 2, z), getParticle(x, z + 2));
			}
		}

		// Make triangle
		/*
			(x,y)	*--* (x+1,y)
					| /|
					|/ |
			(x,y+1) *--* (x+1,y+1)
		*/
		for (int x = 0; x < num_particles_width - 1; x++)
		{
			for (int z = 0; z < num_particles_height - 1; z++)
			{
				makeTriangle(getParticle(x, z), getParticle(x, z + 1), getParticle(x + 1, z));
				makeTriangle(getParticle(x + 1, z + 1), getParticle(x + 1, z), getParticle(x, z + 1));
			}
		}

		// Make unmovable particles
		for (int i = 0; i < 3; i++)
		{
			getParticle(0 + i, 0)->makeUnmovable();
			getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();
			getParticle(0, num_particles_height - 1 - i)->makeUnmovable();
			getParticle(num_particles_width - 1, num_particles_height - 1 - i)->makeUnmovable();
			getParticle(num_particles_width / 2, num_particles_height / 2)->makeUnmovable();
		}
		
		//getParticle(num_particles_width / 2, num_particles_height / 2)->makeUnmovable();
	}

	/* drawing the cloth as a smooth shaded OpenGL triangular mesh
	Called from the display() method
	*/
	void drawShaded()
	{
		/* reset vertex normals (which where written to last frame) */
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i].resetNormal();
		}

		/* set vertex normal */
		for (int i = 0; i < triangles.size(); i++)
		{
			triangles[i].calcTriangleNormal();
			triangles[i].p[0]->addToNormal(triangles[i].normal);
			triangles[i].p[1]->addToNormal(triangles[i].normal);
			triangles[i].p[2]->addToNormal(triangles[i].normal);
		}
		
		/* finally, draw triangles */
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < triangles.size(); i=i+2)
		{
			/*
			VECTOR3D color(0, 0, 0);
			if (i % 4) // red and white color is interleaved according to which column number
				color = VECTOR3D(0.6f, 0.2f, 0.2f);
			else
				color = VECTOR3D(1.0f, 1.0f, 1.0f);
			*/
			
			VECTOR3D color(0.6, 0.2, 0.2);
			drawTriangle(triangles[i], color);
			drawTriangle(triangles[i + 1], color);
		}
		glEnd();
	}

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	void timeStep()
	{
		for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
		{
			for (int j = 0; j < constraints.size(); j++)
			{
				constraints[j].satisfyConstraint();
			}
		}

		for (int i = 0; i < particles.size(); i++)
		{
			particles[i].timeStep(); // calculate the position of each particle at the next time step.
		}
	}

	/* add force to particle */
	void addForce(const VECTOR3D direction)
	{
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i].addForce(direction); // add the forces to each particle
		}

	}

	/* add force to Triangle */
	void windForce(const VECTOR3D direction)
	{
		for (int i = 0; i < triangles.size(); i++)
			addWindForcesForTriangle(triangles[i], direction);
	}

	void ballCollision(const VECTOR3D center, const float radius)
	{
		for (int i = 0; i < particles.size(); i++)
		{
			VECTOR3D v = particles[i].getPos() - center;
			float l = v.Magnitude();
			if (v.Magnitude() < radius) // if the particle is inside the ball
			{
				particles[i].offsetPos(v.Normalize()*(radius - l)); // project the particle to the surface of the ball
			}
		}
	}

	void planeCollision(const float position)
	{
		for (int i = 0; i < particles.size(); i++)
		{
			float diffy = particles[i].getPos().y - (position+0.2);
			
			if (diffy < 0)
			{
				VECTOR3D correction = VECTOR3D(0, 1, 0);
				particles[i].offsetPos(correction*(-diffy));
			}
		}
	}

	bool neighborTriangle(Triangle T1, Triangle T2)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (T1.p[i] == T2.p[j])
					return true;
			
		return false;
	}

	bool IntersectPoint(Particle *p1, Particle *p2, Triangle T, VECTOR3D &point)
	{
		VECTOR3D pos1 = p1->getPos();
		VECTOR3D pos2 = p2->getPos();
		VECTOR3D normal = T.normal;
		T.calcD();

		float val1 = pos1.InnerProduct(normal) - T.D;
		float val2 = pos2.InnerProduct(normal) - T.D;

		if (val1 * val2 < 0)
		{
			setIntersectPoint(p1, p2, T, point);
			return true;
		}
		else if (val1*val2 == 0)
		{
			point = (val1 == 0) ? pos1 : pos2;
			return true;
		}
		return false;
	}

	void setIntersectPoint(Particle *p1, Particle *p2, Triangle T, VECTOR3D &point)
	{
		VECTOR3D pos1 = p1->getPos();
		VECTOR3D pos2 = p2->getPos();
		VECTOR3D dir = pos2 - pos1;
		dir = dir.Normalize();
		float first = T.D - pos1.InnerProduct(T.normal);
		float second = T.normal.InnerProduct(dir);

		point = pos1 + dir * (first / second);
	}

	bool PointinPolygon(VECTOR3D &p, Triangle T)
	{
		VECTOR3D vertex[3];
		for (int i = 0; i < 3; i++)
			vertex[i] = T.p[i]->getPos() - p; //vector from p to triangle vertex
	
		float angle = 0;
		for (int i = 0; i < 3; i++)
			angle += acos(vertex[i % 3].InnerProduct(vertex[(i + 1) % 3]) / (vertex[i % 3].Magnitude() * vertex[(i+1) % 3].Magnitude()));
		
		if (angle >= 2 * PI)
		{
			return true;
		}
		return false;
	}

	/* check whether triangle T1 intersect with triangle T2 */
	bool PolygonIntersect(Triangle T1, Triangle T2)
	{
		for (int i = 0; i < 3; i++)
		{
			VECTOR3D point;
			if (IntersectPoint(T2.p[i % 3], T2.p[(i + 1) % 3], T1, point))
				return PointinPolygon(point, T1);
		}
		return false;
	}

	/* unoptimized collision detection. time complexity = O(n^2) */
	void RawSelfCollision()
	{
		for (int i = 0; i < triangles.size(); i++)
		{
			for (int j = 0; j < triangles.size(); j++)
			{
				if (!(neighborTriangle(triangles[i], triangles[j])) && PolygonIntersect(triangles[i], triangles[j]))
				{
					for (int k = 0; k < 3; k++)
					{
						triangles[i].calcRepulsive();
						triangles[j].calcRepulsive();
						triangles[i].p[k]->offsetPos(triangles[i].repulsive * 1);
						triangles[j].p[k]->offsetPos(triangles[j].repulsive * 1);
					
						/*
						TO DO
						collisino resolve...
						*/
					}
					return;
				}
			}
		}
	}
};