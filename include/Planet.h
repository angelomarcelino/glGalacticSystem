#ifndef PLANET_H
#define PLANET_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include <vector>

#include "Sphere.h"

class Planet {
    private:
        SolidSphere* sphere;
        
        const GLdouble grav_const;
        const GLdouble scaleG;

        // Planet variables
        GLfloat x_angle, y_angle, z_angle;

        GLdouble x_pos, y_pos, z_pos;
        GLdouble x_pos_G, z_pos_G;

        GLdouble mass, radius, angle;
        
        GLdouble grav_accel, x_vel, z_vel;

        // Used for axis rotation
        GLfloat theta, rotation_speed;

        // Size scaling
        GLfloat scale;

        // RGB color values
        GLfloat color[3];

        // Moon variables
        GLfloat moon_x_pos, moon_y_pos, moon_z_pos, moon_angle;
        GLfloat moon_color[3] = { 0.4, 0.4, 0.4 };

        // Orbit line variables
        std::vector<std::pair<GLdouble, GLdouble>> orbit_points;
        int last_insertion = 0;
        int orbit_increment = 2;

        void GravMath();
        void Render();
        void SetPosition(GLdouble _x_pos, GLdouble _y_pos, GLdouble _z_pos);
        void SetColor(GLfloat red, GLfloat green, GLfloat blue);
        void UpdateMoon(GLdouble x, GLdouble y, GLdouble z);
        void DrawOrbit(GLdouble _x_pos, GLdouble _z_pos, bool orbit_toggle);

    public:
        Planet(
            std::pair<GLdouble, GLdouble> initial_pos, 
            std::pair<GLdouble, GLdouble> initial_vel,
            GLfloat tilt, GLfloat _rotation_speed,
            GLdouble _mass, GLfloat size, GLfloat* _color
        );
        ~Planet();

        void Update(bool grav_toggle, bool orbit_toggle);
        
        void GetPositon(GLdouble* camera_pos);
        
        GLdouble GetMass();
        
        bool has_moon;
};

#endif  // PLANET_H