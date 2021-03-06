/*
This source code is accompanying the Cloth Tutorial at the cg.alexandra.dk blog.

You may use the code in any way you see fit. Please leave a comment on the blog
or send me an email if the code or tutorial was somehow helpful.

Everything needed is defined in this file, it is probably best read from the
bottom and up, since dependancy is from the bottom and up

A short overview of this file is;
* includes
* physics constant

* class Vec3
* class Particle (with position Vec3)
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


// Just below are three global variables holding the actual animated stuff; Cloth and Ball 
Cloth cloth1(14, 10, 55, 45); // one Cloth object of the Cloth class
Vec3 ball_pos(7, -5, 0); // the center of our one ball
float ball_radius = 2; // the radius of our one ball



					   /***** Below are functions Init(), display(), reshape(), keyboard(), arrow_keys(), main() *****/

					   /* This is where all the standard Glut/OpenGL stuff is, and where the methods of Cloth are called;
					   addForce(), windForce(), timeStep(), ballCollision(), and drawShaded()*/


void init(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { -1.0,1.0,0.5,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

	glEnable(GL_LIGHT1);

	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}


float ball_time = 0; // counter for used to calculate the z position of the ball below

					 /* display method called each frame*/
void display(void)
{
	// calculating positions

	ball_time++;
	//////////////////////////////////////////////////////////////////////////////////////////ball_pos.f[2] = cos(ball_time / 50.0) * 7;

	cloth1.addForce(Vec3(0, -0.2, 0)*TIME_STEPSIZE2); // add gravity each frame, pointing down
	cloth1.windForce(Vec3(0.5, 0, 0.2)*TIME_STEPSIZE2); // generate some wind each frame
	cloth1.timeStep(); // calculate the particle positions of the next frame
	cloth1.ballCollision(ball_pos, ball_radius); // resolve collision with the ball



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

	glTranslatef(-6.5, 6, -9.0f); // move camera out and center on the cloth
	glRotatef(25, 0, 1, 0); // rotate a bit to see the cloth from the side
	cloth1.drawShaded(); // finally draw the cloth with smooth shading

	glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		glTranslatef(ball_pos.f[0], ball_pos.f[1], ball_pos.f[2]); // hence the translation of the sphere onto the ball position
		glColor3f(0.4f, 0.8f, 0.5f);
		glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
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
		glutReshapeWindow(1280, 720);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Cloth Draping Simulation");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	glutMainLoop();
}