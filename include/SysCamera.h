#ifndef SYSCAMERA_H
#define SYSCAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include <map>

class SysCamera {
    private:
        GLfloat x_pos, y_pos, z_pos;
        GLfloat x_rot, y_rot, angle;
        GLfloat camera_speed;

        GLfloat x_lookat, y_lookat, z_lookat; 

        GLfloat radius;

        bool camera_lock;

    public:
        SysCamera();
        ~SysCamera();

        void Update(std::map<std::string, bool> &keys_state, GLdouble* planet_position);
        void CameraReset();
    
};

#endif // SYSCAMERA_H