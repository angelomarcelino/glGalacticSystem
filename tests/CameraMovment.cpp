#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/Camera.h"

#include <iostream>
using namespace std;

Camera cam(
    new Vec3(0.0, 0.0, 5.0), // Eye
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
	gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
			  cam.center.x, cam.center.y, cam.center.z,
			  cam.up.x, cam.up.y, cam.up.z);

	drawAxis(3.0);

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
			break;
		case 'a':
			cam.left();
			break;
		case 's':
			cam.backward();
			break;
		case 'd':
			cam.right();
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
