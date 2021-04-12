#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH	1280
#define HEIGHT	720

static int day = 0;
static GLfloat year = 0;

GLfloat diffuseMaterial[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_specular[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_position[4] = {0.0, 0.0, 0.0, 1.0};

void drawAxis(int l) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-l, 0.0, 0.0);
	glVertex3f(l, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -l, 0.0);
	glVertex3f(0.0, l, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -l);
	glVertex3f(0.0, 0.0, l);
	glEnd();
	glEnable(GL_LIGHTING);
}

void init(void) {
	
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawAxis(30);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//Drawing the sun
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRotatef(year, 0.0, 1.0, 0.0);
	glColor4f(1.0, 1.0, 0.0, 1.0);
	glutSolidSphere(1.0, 20, 16);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	//Drawing mercurius
	glPushMatrix();
	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);  
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glutSolidSphere(0.3, 20, 16);
	glPopMatrix();

	//Drawing the earth1
	glPushMatrix();
	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(5.0, 0.0, 0.0);  
	glColor4f(1.0, 0.0, 1.0, 1.0);
	glutSolidSphere(0.7, 20, 16);
	glPopMatrix();

	glutSwapBuffers();
}

void spinDisplay(void) {
	year = year + 0.01;
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
		
		case 27:
			exit(0);
			break;

		default:
			break;
	}
}

int main(int argc, char **argv) {
	srand((unsigned int)time(NULL));
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(argv[0]);
	
	init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();
	
	return 0;
}
