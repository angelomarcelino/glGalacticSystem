#ifndef SPHERE_H
#define SPHERE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include <vector>

class SolidSphere {
   protected:
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<GLushort> indices;

   private:
        void AxisRotation(GLfloat theta);

   public:
        SolidSphere();
	    SolidSphere(float radius, unsigned int rings, unsigned int sectors);
	    ~SolidSphere();

	    void Draw(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat ay, GLfloat az, GLfloat* color, GLfloat theta, GLfloat scale);
};

#endif  // SPHERE_H