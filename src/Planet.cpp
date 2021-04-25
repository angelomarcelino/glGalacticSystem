#include "../include/Planet.h"

// Class Constructor
Planet::Planet(
    std::pair<GLdouble, GLdouble> initial_pos, 
    std::pair<GLdouble, GLdouble> initial_vel,
    GLfloat tilt, GLfloat _rotation_speed,
    GLdouble _mass, GLfloat _size, GLfloat* _color
) : grav_const(6.6742e-11), scaleG(10.0) {

    // Set up planet variables
    radius = 0.0;
    angle = 0.0;
    grav_accel = 0.0;
    scale = 1.0;
    size = _size;
    mass = _mass;
    
    x_pos_G = initial_pos.first;
    z_pos_G = initial_pos.second;

    x_vel = initial_vel.first;
    z_vel = initial_vel.second;

    color[0] = _color[0];
    color[1] = _color[1];
    color[2] = _color[2];

    // Axis rotation
    rotation_speed = _rotation_speed;
    x_angle = tilt;
    theta = 0.0f;

    // Moon variables
    has_moon = false;
    moon_angle = 0.0f;
    moon_x_pos = 0.0f;
    moon_y_pos = 0.0f;
    moon_z_pos = 0.0f;
}

// Class Destructor
Planet::~Planet() {}

void Planet::Update(bool grav_toggle, bool orbit_toggle) {
    GLdouble _x, _z;

    // if it isn't the star
    if (mass != 0) {
        // Toggles gravity on and off
        if (grav_toggle) {
            GravMath();
        }

        // Update positions to the scale
        _x = x_pos_G * scaleG;
        _z = z_pos_G * scaleG;
        
        DrawOrbit(_x, _z, orbit_toggle);
    
    } else {
        _x = 0.0;
        _z = 0.0;
    }
    
    SetPosition(_x, 0.0, _z);
    scale = 1.0f;

    Render();

    // If the planet has a moon, update its position in relation to the planet
    if (has_moon) {
        UpdateMoon(_x, 0.0, _z);
    }   
}

void Planet::GetPositon(GLdouble* camera_pos) {
    camera_pos[0] = x_pos_G * scaleG;
    camera_pos[1] = z_pos_G * scaleG;
}

GLdouble Planet::GetMass() {
    return mass;
}

void Planet::GravMath() {
    // Update the planet position in relation to the gravity acting on it as stated by F = GMm /r ^2
    
    // Find the distance between the planet and the origin
    radius = sqrt(pow(x_pos_G, 2) + pow(z_pos_G, 2));
    
    //Calculate its acceleration
    grav_accel = (grav_const * (mass / pow(radius, 2)));
	
    angle = atan2(x_pos_G, z_pos_G);										
	
    // Updates the velocities
    x_vel += (sin(angle) * grav_accel);
	z_vel += (cos(angle) * grav_accel);

    // Updates the position
	x_pos_G -= x_vel;
	z_pos_G -= z_vel;	
}

void Planet::AxisRotation(GLfloat theta) {
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

void Planet::Render() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glColor3f(color[0], color[1], color[2]);
    glTranslatef(x_pos, y_pos, z_pos);

    glRotatef(x_angle, 1.0, 0.0, 0.0);
    AxisRotation(theta); // Compute rotation on planet axis
    glScalef(scale, scale, scale); 

    glutSolidSphere(size, 24, 48);

    glPopMatrix();

    // Update the rotation value
    theta += rotation_speed;
    if (theta >= (2 * M_PI)) {
        theta = 0;
    }
}

void Planet::SetPosition(GLdouble _x_pos, GLdouble _y_pos, GLdouble _z_pos) {
    x_pos = _x_pos;
    y_pos = _y_pos;
    z_pos = _z_pos;
}

void Planet::SetColor(GLfloat red, GLfloat green, GLfloat blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
}

void Planet::UpdateMoon(GLdouble x, GLdouble y, GLdouble z) {
    // Set the oribt of the moon
    moon_x_pos = x + sin(((moon_angle * 180) / M_PI) * 2.0f) * 1.5f;
    moon_y_pos = y + sin(((moon_angle * 180) / M_PI) * 2.0f) * 1.5f;
    moon_z_pos = z + cos(((moon_angle * 180) / M_PI) * 2.0f) * 1.5f;

    moon_angle += 0.0008f;
    
    SetPosition(moon_x_pos, moon_y_pos, moon_z_pos);
    scale = 0.2f;

    // Render();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glColor3f(moon_color[0], moon_color[1], moon_color[2]);
    glTranslatef(x_pos, y_pos, z_pos);

    glRotatef(x_angle, 1.0, 0.0, 0.0);
    AxisRotation(theta); // Compute rotation on planet axis
    glScalef(scale, scale, scale); 

    glutSolidSphere(size, 24, 48);

    glPopMatrix();
    // Update the rotation value
    theta += 0.25 * rotation_speed;
    if (theta >= (2 * M_PI)) {
        theta = 0;
    }
}

void Planet::DrawOrbit(GLdouble _x_pos, GLdouble _z_pos, bool orbit_toggle) {

    orbit_points.push_back({_x_pos, _z_pos});

    if (orbit_points.size() > 2 and orbit_toggle) {
        for (auto point = orbit_points.begin();  (point + 1) != orbit_points.end(); point += 1) {
            auto start = *point, end = *(point + 1);

            glDisable(GL_LIGHTING);
            glBegin(GL_LINES);
            glColor3f(color[0], color[1], color[2]);
            glVertex3f(start.first, 0.0, start.second);
            glVertex3f(end.first, 0.0, end.second);
            glEnd();
            glEnable(GL_LIGHTING);
        }    
    }

    if (orbit_points.size() > 100 and _x_pos < 0 and  _z_pos > 0) {
        orbit_points.clear();
    }   
}