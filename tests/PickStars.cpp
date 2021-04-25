#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

#include "../include/Camera.h"
#include "../include/StarSystem.h"
using namespace std;

double secSize = 0.3;
double secLim = 5;
int nSector = (secLim / secSize);

Camera cam(
	new Vec3(0.0, 0.0, 3.0),  // Eye
	new Vec3(0.0, 0.0, 0.0),  // Center
	new Vec3(0.0, 1.0, 0.0),  // Up
	secSize					  // Movement Speed
);

Vec3 toCoord(Vec3 ints, Vec3 offset, int nSec, double secSize) {
	//return (i + offset - nSec / 2) * secSize;
	double x = (ints.x + offset.x - nSec / 2) * secSize;
	double y = (ints.y + offset.y - nSec / 2) * secSize;
	double z = (ints.z + offset.z - nSec - 10) * secSize;
	return Vec3(x, y, z);
}

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

void drawStars(GLenum mode) {
	for (int i = 0; i < nSector; i++)
		for (int j = 0; j < nSector; j++)
			for (int k = 0; k < nSector * 2; k++) {
				StarSystem SysSector(i + (uint32_t)cam.offset.x,
									 j + (uint32_t)cam.offset.y,
									 k + (uint32_t)cam.offset.z,
									 nSector, secSize);

				if (SysSector.starExists) {
					glColor3f(SysSector.starColor[0], SysSector.starColor[1], SysSector.starColor[2]);
					Vec3 coords = toCoord(Vec3(i, j, k), cam.offset, nSector, secSize);
					glPushMatrix();
					glTranslatef(coords.x, coords.y, coords.z);
					//glutWireCube(secSize);	// setor
					glPopMatrix();
					glPushMatrix();
					// translate star
					if (mode == GL_SELECT)
						glLoadName(SysSector.getLehmer());
					glTranslatef(coords.x + SysSector.starOffset[0],
								 coords.y + SysSector.starOffset[1],
								 coords.z + SysSector.starOffset[2]);
					// draw star
					glutSolidSphere(SysSector.starRadius, 20, 16);
					glPopMatrix();
				}
			}
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
		drawStars(GL_RENDER);
	}
	glPopMatrix();

	/* Matrix cube
	Vec3 coords = toCoord(Vec3(0,0,0), cam.offset, nSector, secSize);
	cout << coords << endl;
	glPushMatrix();
	glTranslatef(coords.x, coords.y, coords.z);
	//glutWireCube(secLim);	// setor
	glPopMatrix(); */

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.5, 200.0);
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

void processHits(GLint hits, GLuint buffer[]) {
	unsigned int i, j;
	GLuint names, *ptr;

	printf("hits = %d\n", hits);
	ptr = (GLuint *)buffer;
	for (i = 0; i < hits; i++) { /* for each hit  */
		names = *ptr;
		printf(" number of names for hit = %d\n", names);
		ptr++;
		printf("  z1 is %g;", (float)*ptr / 0x7fffffff);
		ptr++;
		printf(" z2 is %g\n", (float)*ptr / 0x7fffffff);
		ptr++;
		printf("   the name is ");
		for (j = 0; j < names; j++) { /* for each name */
			cout << *ptr;
			ptr++;
		}
		printf("\n");
	}
}

#define BUFSIZE 512

void onMouse(int button, int state, int x, int y) {
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	/*  create 5x5 pixel picking region near cursor location */
	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y),
				  5.0, 5.0, viewport);
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.5, 200.0);
	drawStars(GL_SELECT);
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(720, 640);
	glutInitWindowPosition(500, 200);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(onMouse);
	glutMainLoop();
	return 0;
}
