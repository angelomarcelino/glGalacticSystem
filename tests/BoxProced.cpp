#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
using namespace std;

static GLdouble star[100][3];
static int dots[100][2];
static double xmov = 0, zmov = 0;
static int year = 0;

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

double rndDouble(double min, double max) {
	return ((double)rand() / (double)(0x7FFFFFFF)) * (max - min) + min;
}

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

	/*glPushMatrix();
	{  // Axis Lines
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, -1.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, -1.0);
		glVertex3f(0.0, 0.0, 10.0);
		glEnd();
	}
	glPopMatrix();*/

	glTranslatef(xmov, 0.0, zmov);	// Movement Matrix
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);

	int lim = (3 / 0.3) / 2;
	int cont = 0;
	for (int x = -lim; x <= lim; x++) {
		for (int y = -lim; y <= lim; y++) {
			for (int z = -lim; z <= lim; z++) {
				bool isStar = false;
				int nSeed = z << 16 | y << 16 | abs(x);

				nLehmer = nSeed;
				isStar = Lehmer32() % 256 < 32;

				if (isStar) {
					star[cont][0] = (x * 0.3) + rndDouble(-0.09, 0.09);
					star[cont][1] = (y * 0.3) + rndDouble(-0.09, 0.09);
					star[cont][2] = (z * 0.3) + rndDouble(-0.09, 0.09);
					cont++;
				}
			}
		}
	}

	glPushMatrix();
	{  // Procedural stars
		for (int i = 0; i < cont; i++) {
			cout << star[i][0] << " " << star[i][1] << " " << star[i][2] << endl;
			glPushMatrix();
			// translate star
			glTranslatef(star[i][0], star[i][1], star[i][2]);
			// draw star
			glutSolidSphere(0.05, 20, 16);
			glPopMatrix();
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
			xmov += 0.05;
			glutPostRedisplay();
			break;
		case 'a':
			xmov -= 0.05;
			glutPostRedisplay();
			break;
		case 'w':
			zmov += 0.05;
			glutPostRedisplay();
			break;
		case 's':
			zmov += -0.05;
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
