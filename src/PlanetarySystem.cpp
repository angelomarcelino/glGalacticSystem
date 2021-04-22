#include "../include/PlanetarySystem.h"

//Colour values
GLfloat color_white[3]     = {1.0f, 1.0f, 1.0f};
GLfloat color_green[3]     = {0.0f, 0.5f, 0.0f};
GLfloat color_blue[3]      = {0.0f, 0.0f, 1.0f};
GLfloat color_grey[3]      = {0.6f, 0.6f, 0.6f};
GLfloat color_purple[3]    = {0.4f, 0.0f, 0.4f};
GLfloat color_red[3]       = {0.6f, 0.0f, 0.0f};
GLfloat color_yellow[3]    = {1.0f, 0.8f, 0.0f};
GLfloat color_orange[3]    = {1.0f, 0.5f, 0.0f};
GLfloat color_lightblue[3] = {0.0f, 1.0f, 1.0f};

// Class Constructor
PlanetarySystem::PlanetarySystem() {
    planets = {
        // initial_pos, initial_vel, tilt, _rotation_speed, mass, size, color
        new Planet({0.0, 0.0}, {0.0, 0.0}, 0.0, 0.0, 0.0, 2.0, color_yellow),
        new Planet({0.0, 0.387}, {0.025, 0.0}, 0.0, 0.001, 3834600, 0.383, color_purple),
        new Planet({0.0, 1.0}, {0.07, 0.00}, 30.0, 0.01, 6.972E7, 1.0, color_green),
        new Planet({0.0, 1.52}, {0.02, 0.0}, 32.1, 0.015, 7460040, 0.832, color_red),
        new Planet({0.0, 2.2}, {0.05, 0.0}, 4.02, 0.0266, 74600400, 0.9, color_blue)
    };

    // Define moons
    planets[2]->has_moon = true;
    planets[4]->has_moon = true;

    // Set up planet select controls
    planet_select = new bool[planets.size()]{ false };

    camera = new SysCamera;
}

// Class Destructor
PlanetarySystem::~PlanetarySystem() {
    delete[] planet_select;
}

void PlanetarySystem::Init() {
    // Enable OpenGL stuff
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void PlanetarySystem::Run() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, near_plane, far_plane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (toggle_wire_frame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    for (int i = 0; i < planets.size(); i++) {
        if (planet_select[i]) {
            planets[i]->GetPositon(camera_pos);
        } 
    }
    
    camera->Update(keys_state, camera_pos);

    for (auto planet : planets) {

        if (planet->GetMass() == 0.0) {
            glDisable(GL_LIGHTING);
            planet->Update(toggle_gravity, keys_state["d"]);
            glEnable(GL_LIGHTING);        
        }

        planet->Update(toggle_gravity, keys_state["d"]);
    }

    // Toggle the display of the world's coordinates axis
    if (keys_state["s"]) {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-50.0, 0.0, 0.0);
        glVertex3f(50.0, 0.0, 0.0); // Red is X

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 50.0, 0.0); // Green is Y

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, -50.0);
        glVertex3f(0.0, 0.0, 50.0); // Blue is Z
        glEnd();
        glEnable(GL_LIGHTING);
    }

    glutSwapBuffers();
}

void PlanetarySystem::SelectPlanet(int planet_idx) {
    if (planet_idx > planets.size() - 1) {
        camera_pos[0] = 0;
        camera_pos[1] = 0;
        return;
    }

    planet_select = new bool[planets.size()]{ false };
    planet_select[planet_idx] = true;
}

void PlanetarySystem::KeyboardHandler(int key) {
    switch (key) {
        case GLUT_KEY_UP:
            keys_state["Up"] = true;
            glutPostRedisplay();
            break;

        case GLUT_KEY_DOWN:
            keys_state["Down"] = true;
            glutPostRedisplay();
            break;

        case GLUT_KEY_LEFT:
            keys_state["Left"] = true;
            glutPostRedisplay();
            break;

        case GLUT_KEY_RIGHT:
            keys_state["Right"] = true;
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

void PlanetarySystem::KeyboardHandler(unsigned char key) {
    switch (key) {
        case ' ':
            keys_state["Space"] = true;
            glutPostRedisplay();
            break;

        case 'w':
            toggle_wire_frame = !toggle_wire_frame;
            break;

        case 'g':
            toggle_gravity = !toggle_gravity;
            break;

        case 'q':
            keys_state["q"] = true;
            break;
        
        case 'e':
            keys_state["e"] = true;
            break;
        
        case 's':
            keys_state["s"] = !keys_state["s"];
            break;
        
        case 'd':
            keys_state["d"] = !keys_state["d"];
            break;

        case 'r':
            camera->CameraReset();
            break;

        case '1':
            SelectPlanet(1);
            keys_state["Space"] = true;
            break;

        case '2':
            SelectPlanet(2);
            keys_state["Space"] = true;
            break;

        case '3':
            SelectPlanet(3);
            keys_state["Space"] = true;
            break;

        case '4':
            SelectPlanet(4);
            keys_state["Space"] = true;
            break;

        case '5':
            SelectPlanet(5);
            keys_state["Space"] = true;
            break;

        case '6':
            SelectPlanet(6);
            keys_state["Space"] = true;
            break;

        case '7':
            SelectPlanet(7);
            keys_state["Space"] = true;
            break;

        case '8':
            SelectPlanet(8);
            keys_state["Space"] = true;
            break;
            
        case 27:  // Esc key
            exit(0);
            break;

        default:
            break;
    }
}