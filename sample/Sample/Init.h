#pragma once
#include <gl/glut.h>

void Lighting(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glFrontFace(GL_CCW);

	float light_pos[] = { 25.0, 50.0, 50.0, 0.0 }; //������ ��ġ
	float light_dir[] = { 0, -1, -1 }; //���߱��� ���ߴ� ��

	/* light ���� �� RGBA */
	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 }; //�ֺ���
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 }; //���ݻ�
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //���ݻ�. ����

	

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);

	/* material�� ���� �� RGBA */

	float noMat[] = { 0, 0, 0, 1 }; //��ü �߱�
	float matSpec[] = { 0.3, 0.3, 0.3, 1.0 }; //�ݻ籤. �Ͼ�� ���̶����� �κ�. �ݼ��� ��ä
	float matShininess = 100; //����, �ݻ���. ǥ���� �Ų��� ����
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
	glEnable(GL_LIGHT0);
}

void Initialize(int argc, char** argv)
{
	glutInit(&argc, argv);

	// ������ ����
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Cloth simulation");

	// depth test
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);

	Lighting();
}