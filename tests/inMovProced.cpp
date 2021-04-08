#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#include "../include/StarSystem.h"

using namespace std;

static int dots[100][2];
static int gxOff[3] = {0, 0, 0};
static int year = 0;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display(void) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	{  // Star dots
		glColor3f(1.0, 1.0, 1.0);
		glPointSize(3);
		glBegin(GL_POINTS);
		for (int i = 1; i < 100; i++) {
			int x = dots[i][0] % w;
			int y = dots[i][1] % h;
			glVertex2i(x, y);
		}
		glEnd();
	}
	glPopMatrix();

	// 3D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	{  // Axis Lines
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-10.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, -10.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, -10.0);
		glVertex3f(0.0, 0.0, 10.0);
		glEnd();
	}
	glPopMatrix();

	//glTranslatef(gxOff[0], 0.0, gxOff[2]);	// Movement Matrix
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);

	glPushMatrix();
	{  // Procedural stars
		double secSize = 0.3;
		double secLim = 3;
		int nSector = (secLim / secSize);

		int xyz[3] = {0, 0, 0};

		for (int x = 0; x < nSector; x++)
			for (int y = 0; y < nSector; y++)
				for (int z = 0; z < nSector; z++) {
					StarSystem SysSector(x + gxOff[0], y + gxOff[1], z + gxOff[2], nSector, secSize);

					if (SysSector.starExists) {
						glPushMatrix();
						glTranslatef((x + gxOff[0] - nSector / 2) * secSize,
									 (y + gxOff[1] - nSector / 2) * secSize,
									 (z + gxOff[2] - nSector / 2) * secSize);
						glutWireCube(secSize);
						glPopMatrix();
						glPushMatrix();
						// translate star
						glTranslatef(SysSector.starCoord[0], SysSector.starCoord[1], SysSector.starCoord[2]);
						// draw star
						glutSolidSphere(SysSector.starRadius, 20, 16);
						glPopMatrix();
					}
				}
	}

	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glutWireCube(3.0);
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'd':
			gxOff[0] += 1;
			glutPostRedisplay();
			break;
		case 'a':
			gxOff[0] -= 1;
			glutPostRedisplay();
			break;
		case 'w':
			gxOff[2] += 1;
			glutPostRedisplay();
			break;
		case 's':
			gxOff[2] += -1;
			glutPostRedisplay();
			break;
		case 'y':
			year = (year + 5) % 360;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

int main(int argc, char **argv) {
	for (int i = 0; i < 100; i++) {
		dots[i][0] = rand();
		dots[i][1] = rand();
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 700);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
