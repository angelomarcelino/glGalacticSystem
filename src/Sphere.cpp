#include "../include/Sphere.h"

// Class Constructor
SolidSphere::SolidSphere() {}

SolidSphere::SolidSphere(float radius, unsigned int rings, unsigned int sectors) {
    const float R = 1.0 / (float)(rings - 1);
    const float S = 1.0 / (float)(sectors - 1);

    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);

    auto v_it = vertices.begin();
    auto n_it = normals.begin();

    for (int r = 0; r < rings; r++) {
        for (int s = 0; s < sectors; s++) {
            const float y = sin(-M_PI_2 + M_PI * r * R);
            const float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            const float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            *v_it++ = x * radius;
            *v_it++ = y * radius;
            *v_it++ = z * radius;

            *n_it++ = x;
            *n_it++ = y;
            *n_it++ = z;
        }
    }

    indices.resize(rings * sectors * 4);
    auto idx_it = indices.begin();

    for (int r = 0; r < rings - 1; r++) {
        for (int s = 0; s < sectors - 1; s++) {
            *idx_it++ = r * sectors + s;
            *idx_it++ = r * sectors + (s + 1);
            *idx_it++ = (r + 1) * sectors + (s + 1);
            *idx_it++ = (r + 1) * sectors + s;
        }
    }
}

// Class Destructor
SolidSphere::~SolidSphere() {}

void SolidSphere::Draw(GLfloat x, GLfloat y, GLfloat z, GLfloat ax, GLfloat ay, GLfloat az, GLfloat* color, GLfloat theta, GLfloat scale) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glColor3f(color[0], color[1], color[2]);
    glTranslatef(x, y, z);

    glRotatef(ax, 1.0, 0.0, 0.0);
    AxisRotation(theta); // Compute rotation on planet axis
    glScalef(scale, scale, scale); 

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
        
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

void SolidSphere::AxisRotation(GLfloat theta) {
    // Set the up vector
    GLfloat x = 0.0f, y = 1.0f, z = 0.0f;

    // Normalizing the vector
    GLfloat n = sqrt(x * x + y * y + z * z);
    if (n != 1) {
        GLfloat inv_n = 1 / n;

        x *= inv_n;
        y *= inv_n;
        z *= inv_n;
    }

    // Creates the rotation matrix using the up vector and theta
    GLfloat rotation_mat[4][4] = {
        { powf(x, 2) + ((powf(y, 2) + powf(z, 2)) * cosf(theta)), ((x * y) * (1.0f - cosf(theta))) - (z * sinf(theta)), ((x * z) * (1.0f - cosf(theta))) + (y * sinf(theta)), 0 },
		{ ((x * y) * (1.0f - cosf(theta))) + (z * sinf(theta)), powf(y, 2) + ((powf(x, 2) + powf(z, 2)) * cosf(theta)), ((y * z) * (1.0f - cosf(theta))) - (x * sinf(theta)), 0 },
		{ ((x * z) * (1.0f - cosf(theta))) - (y * sinf(theta)), ((y * z) * (1.0f - cosf(theta))) + (x * sinf(theta)), powf(z, 2) + ((powf(x, 2) + powf(y, 2)) * cosf(theta)), 0 },
		{ 0, 0, 0, 1 }
    };

    glMultMatrixf(*rotation_mat);
}
