/*
This source code is accompanying the Cloth Tutorial at the cg.alexandra.dk blog.

You may use the code in any way you see fit. Please leave a comment on the blog
or send me an email if the code or tutorial was somehow helpful.

Everything needed is defined in this file, it is probably best read from the
bottom and up, since dependancy is from the bottom and up

A short overview of this file is;
* includes
* physics constant

* class VECTOR3D
* class Particle (with position VECTOR3D)
* class Constraint (of two particles)
* class Cloth (with particles and constraints)

* Cloth object and ball (instance of Cloth Class)

* OpenGL/Glut methods, including display() and main() (calling methods on Cloth object)

Jesper Mosegaard, clothTutorial@jespermosegaard.dk

Tested on: Windows Vista / Visual Studio 2005
Linux (Red Hat) / GCC 4.1.2

History:
2 Jun 2009 - Initial version
6 Jan 2010 - Typo corrected in call of glutInitDisplayMode to enable depth-buffer (Thanks Martijn The)

*/


#ifdef _WIN32
#include <windows.h> 
#endif
#include <iostream>
#include "Cloth.h"
#include "Init.h"
#include "Matrix.h"
#define c_width 20
#define c_height 20

// Just below are three global variables holding the actual animated stuff; Cloth and Ball 
Cloth cloth1(c_width, c_height, 15, 15); // one Cloth object of the Cloth class
VECTOR3D ball_pos(c_width/2, -10, c_height / 2); // the center of our one ball
float ball_radius = 2.5; // the radius of our one ball


					   /***** Below are functions Init(), display(), reshape(), keyboard(), arrow_keys(), main() *****/

					   /* This is where all the standard Glut/OpenGL stuff is, and where the methods of Cloth are called;
					   addForce(), windForce(), timeStep(), ballCollision(), and drawShaded()*/


float ball_time = 0; // counter for used to calculate the z position of the ball below

void RenderPlane(GLfloat width)
{
	glColor3f(0.7f, 0.7f, 0.7f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex3f(-width, 0, width);
	glVertex3f(width, 0, width);
	glVertex3f(width, 0, -width);
	glVertex3f(-width, 0, -width);
	glEnd();
}

void RenderAxis()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(50.0f, 0.1f, 0.0f);
	glVertex3f(-50.0f, 0.1f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 50.0f, 0.0f);
	glVertex3f(0.0f, -50.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.1f, 50.0f);
	glVertex3f(0.0f, 0.1f, -50.0f);
	glEnd();
}
					 /* display method called each frame*/
void display(void)
{
	// calculating positions

	//ball_time++;
	//ball_pos.z = cos(ball_time / 50.0) * 7;
	GLfloat plane_y = -ball_radius -1;
	cloth1.addForce(VECTOR3D(0, -0.2, 0)*TIME_STEPSIZE); // gravity
	//cloth1.windForce(VECTOR3D(0.5, 0, 0.2)*TIME_STEPSIZE2); // generate some wind each frame
	cloth1.timeStep(); // calculate the particle positions of the next frame
	cloth1.HierachialSelfCollision();
	//cloth1.RawSelfCollision(); //resolve self collision
	cloth1.ballCollision(ball_pos, ball_radius); // resolve collision with the ball
	cloth1.planeCollision(ball_pos.y + plane_y); //resolve collision with the plane


	// drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING); // drawing some smooth shaded background - because I like it :)
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.8f, 1.0f);
	glVertex3f(-200.0f, -100.0f, -100.0f);
	glVertex3f(200.0f, -100.0f, -100.0f);
	glColor3f(0.4f, 0.4f, 0.8f);
	glVertex3f(200.0f, 100.0f, -100.0f);
	glVertex3f(-200.0f, 100.0f, -100.0f);
	glEnd();

	glEnable(GL_LIGHTING);


	gluLookAt(0, 15, 15, 0, 0, 0, 0, 1, -1);
	glTranslatef(-ball_pos.x, -ball_pos.y, -ball_pos.z);
	glRotatef(45, 0, 1, 0);
	cloth1.drawShaded(); // finally draw the cloth with smooth shading

	glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
		glColor3f(0.4f, 0.8f, 0.5f);
		glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly

		glTranslatef(0.0f, plane_y, 0.0f);
		RenderAxis();
		RenderPlane(30.0);
	glPopMatrix();
	


	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		gluPerspective(80, (float)w, 1.0, 5000.0);
	else
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case ' ':
		for (int i = 0; i < 3; i++)
		{
			cloth1.getParticle(0 + i, 0)->makemovable();
			cloth1.getParticle(cloth1.num_particles_width - 1 - i, 0)->makemovable();
			cloth1.getParticle(0, cloth1.num_particles_height - 1 - i)->makemovable();
			cloth1.getParticle(cloth1.num_particles_width - 1, cloth1.num_particles_height - 1 - i)->makemovable();
		}
		break;
	case 'b':
		cloth1.getParticle(cloth1.num_particles_width / 2, cloth1.num_particles_height / 2)->makemovable();
		break;
	default:
		break;
	}
}


void arrow_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
	case GLUT_KEY_UP:
		glutFullScreen();
		break;
	case GLUT_KEY_DOWN:
		glutReshapeWindow(800, 800);
		break;
	case GLUT_KEY_RIGHT:
		
		break;
	case GLUT_KEY_LEFT:
		
		break;

		glutPostRedisplay();
	default:
		break;
	}
}

int main(int argc, char** argv)
{

	Initialize(argc, argv);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	/*
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	*/
	

	glutMainLoop();
}