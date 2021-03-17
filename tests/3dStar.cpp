#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

static int year = 0, day = 0, stars = 100;
static double xmov = 0, ymov = 0, zmov = 0;
static GLdouble star[100][3];
static int dots[100][2];


GLdouble double_rand( GLdouble min, GLdouble max ){
    GLdouble scale = rand() / (GLdouble) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_SMOOTH);

}

void display(void) {
    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 2D rendering
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, w, 0, h, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPushMatrix();{ // Star dots
        glColor3f( 1.0, 1.0, 1.0 );
        glPointSize( 3 );
        glBegin( GL_POINTS );
        for( int i = 1; i < 100; i++ )
        {
            int x = dots[i][0] % w;
            int y = dots[i][1] % h;
            glVertex2i( x, y );
        }
        glEnd();
    }
    glPopMatrix();

    // 3D rendering
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();{  // Axis Lines 
        glBegin(GL_LINES);
        glColor3f (1.0, 1.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);

        glColor3f (1.0, 1.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);

        glColor3f (1.0, 1.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 10.0);
        glEnd();
    }
    glPopMatrix();

    glTranslatef(xmov, 0.0, zmov);// Movement Matrix
    glColor3f(1.0, 1.0, 0.0);
    glRotatef((GLfloat)year, 1.0, 0.0, 0.0);
    for (int i = 0; i < stars; i++) {
        // translate star
        glTranslatef(star[i][0], star[i][1], star[i][2]);
        // draw star
        glPushMatrix();
        glutSolidSphere(0.1, 20, 16);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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

   srand((unsigned int)time(NULL));
   for (int i = 0; i < stars; i++) {
     for (int j = 0; j < 3; j++) {
         star[i][j] = double_rand(-1.0, 1.0);
        }
      printf("%f --- %f --- %f\n", star[i][0], star[i][1], star[i][2]);
   }
    for (int i = 0; i < 100; i++) 
     for (int j = 0; j < 2; j++) 
        dots[i][j] = rand();

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (1200, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
