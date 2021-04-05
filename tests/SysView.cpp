#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int day = 0;
static GLfloat year = 0;

void drawAxis(int l) {
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void display(void) {
	GLfloat position[] = {0., 0., 0., 1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAxis(30);

	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//Drawing the sun
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(year, 0.0, 1.0, 0.0);	 //This will self-rotate the sun
	glutSolidSphere(1.0, 20, 16);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	//Drawing mercurius
	glPushMatrix();
	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);  
	glutSolidSphere(1.0, 20, 16);
	glPopMatrix();

	//Drawing the earth1
	glPushMatrix();
	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(5.0, 0.0, 0.0);  
	glutSolidSphere(1.0, 20, 16);
	glPopMatrix();

	glutSwapBuffers();
}

void spinDisplay(void) {
	year = year + 1;
	if (year > 360.0) {
		year = year - 360.0;
	}
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0., 0, 1., 0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'y':
			glutIdleFunc(spinDisplay);
			break;
		case 'Y':
			glutIdleFunc(NULL);
			break;
		default:
			break;
	}
}

int main(int argc, char **argv) {
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(900, 500);
	glutInitWindowPosition(200, 700);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
