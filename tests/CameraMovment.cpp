#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/Camera.h"
#include "../include/StarSystem.h"

#include <iostream>
using namespace std;

int offset = 0;

Camera cam(
    new Vec3(0.0, 0.0, 3.0), // Eye
    new Vec3(0.0, 0.0, 0.0), // Center
    new Vec3(0.0, 1.0, 0.0), // Up
    0.3 // Movement Speed
);

void drawAxis(int l) {
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-l, 0.0, 0.0);
	glVertex3f(l, 0.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, -l, 0.0);
	glVertex3f(0.0, l, 0.0);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 0.0, -l);
	glVertex3f(0.0, 0.0, l);
	glEnd();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display(void) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3D rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam.pos.x, cam.pos.y, cam.pos.z,
			  cam.center.x, cam.center.y, cam.center.z,
			  cam.up.x, cam.up.y, cam.up.z);

	drawAxis(3.0);

	glColor3f(1.0, 1.0, 0.0);

	glPushMatrix();
	{  // Procedural stars
		double secSize = 0.3;
		double secLim = 20;
		int nSector = (secLim / secSize);


		for (int i = 0; i < nSector; i++)
			for (int j = 0; j < nSector; j++)
				for (int k = 0; k < nSector; k++) {
					StarSystem SysSector(i,
										 j,
										 k + offset, nSector, secSize);

					if (SysSector.starExists) {
						glPushMatrix();
						glTranslatef((i - nSector / 2) * secSize,
									 (j - nSector / 2) * secSize,
									 (k + offset - nSector / 2) * secSize);
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
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			cam.forward();
			offset -= 1;
			break;
		case 'a':
			cam.left();
			break;
		case 's':
			cam.backward();
			offset += 1;
			break;
		case 'd':
			cam.right();
			break;
		case 'o':
			cam.pitch(true, 1.0);
			break;
		case 'l':
			cam.pitch(false, 1.0);
			break;
		case 'k':
			cam.yaw(true, 1.0);
			break;
		case ';':
			cam.yaw(false, 1.0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

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
