#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

static int dots[100][2];
static int gxOff[3] = {0, 0, 0};
static int year = 0;

class StarSystem {
   public:
	StarSystem(uint32_t x, uint32_t y, uint32_t z, double nSec, double secSize) {
		nLehmer = (z & 0xFFFF) << 16 | (y & 0xFFFF) << 8 | (x & 0xFFFF);

		starExists = (rndInt(0, 100) == 1);
		if (!starExists) return;

		starRadius = rndDouble(0.03, secSize / 5);

		for (int i = 0; i < 3; i++) {
			int sig = (rndInt(1, 2) == 1) ? 1 : -1;
			starOffset[i] = sig * rndDouble(0.01, secSize / 8);
		}

		starCoord[0] = (((int)x - nSec / 2) * secSize) + starOffset[0];
		starCoord[1] = (((int)y - nSec / 2) * secSize) + starOffset[1];
		starCoord[2] = (((int)z - nSec / 2) * secSize) + starOffset[2];
		//cout << starCoord[0] << " " << starCoord[1] << " " << starCoord[2] << endl;
		//cout << (int)z << " ";
	}

   public:
	bool starExists = false;
	double starRadius = 0.0;
	double starCoord[3] = {0.0, 0.0, 0.0};
	double starOffset[3] = {0.0, 0.0, 0.0};

   private:
	uint32_t nLehmer = 0;
	uint32_t Lehmer32() {
		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;

		return m2;
	}

	int rndInt(int min, int max) { return (Lehmer32() % (max - min)) + min; }

	double rndDouble(double min, double max) {
		return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}
};

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
					StarSystem SysSector(x + gxOff[0],
										 y + gxOff[1],
										 z + gxOff[2], nSector, secSize);

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
