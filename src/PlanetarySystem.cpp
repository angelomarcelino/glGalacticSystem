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

// Stable initial conditions for the planets
GLdouble initial_pos_z[8] = {
    0.3870,
    0.7230,
    1.0000,
    1.5200,
    3.2000,
    4.5800,
    6.7500,
    10.250
};

GLdouble initial_vel_x[8] = { 
    0.025,
    0.068,
    0.070,
    0.020,
    0.130,
    0.350,
    0.120,
    0.195
};

GLdouble masses[8] = {
    3.835E6,    
    5.682E7,    
    6.972E7,    
    7.460E6,    
    7.460E8,    
    6.637E9,
    1.011E9,
    5.192E9     
};

// Initial camera positions
GLfloat initial_camera_pos[9][3] = {
    {15,  12,   9},
    { 5,   5,   5},
    { 9,  10,  15},
    {10,  10,  20},
    {40,  30,  30},
    {40,  40,  50},
    {80,  70,  80},
    {80,  80, 100},
    {70, 107, 160}
};

// Class Constructor
PlanetarySystem::PlanetarySystem() {

}

// Class Destructor
PlanetarySystem::~PlanetarySystem() {
    delete[] planet_select;
    delete camera;
}

void PlanetarySystem::SetRandSeed(uint32_t _random_seed) {
    random_seed = _random_seed;
}

void PlanetarySystem::SetStarColor(GLfloat red, GLfloat green, GLfloat blue) {
    star_color[0] = red;
    star_color[1] = green;
    star_color[2] = blue;
}

uint32_t PlanetarySystem::Lehmer32() {
    random_seed += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)random_seed * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;

	return m2;
}

int PlanetarySystem::RandInt(int min, int max) { 
    return (Lehmer32() % (max - min)) + min; 
}

double PlanetarySystem::RandDouble(double min, double max) {
    return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) / 2 + min;
}

int PlanetarySystem::RandSign() {
    return RandDouble(0.0, 1.0) < 0.4 ? -1 : 1;
};

GLfloat* PlanetarySystem::RndColor() {
    GLfloat *color = new GLfloat[3]{
        (GLfloat)RandDouble(0.0, 1.0),
        (GLfloat)RandDouble(0.0, 1.0),
        (GLfloat)RandDouble(0.0, 1.0)
    };

    return color;
}

GLfloat* PlanetarySystem::RndStarColor() {
    GLfloat *color = new GLfloat[3]{
        1.0,
        (GLfloat)RandDouble(0.00, 1.0),
        (GLfloat)RandDouble(0.35, 1.0)
    };

    return color;
}

void PlanetarySystem::Init() {

    // Generate a random number of planets
    int n_planets = RandInt(0, 8);

    // Add star
    GLfloat star_size = RandInt(2, 5); 

    planets.push_back(new Planet(
        {0.0, 0.0}, {0.0, 0.0}, 
        0.0, 0.0, 0.0, 
        RandDouble(1.5, 2.3), 
        star_color
    ));

    // Add planets with random characteristics
    if(n_planets > 0) {
        GLfloat *planet_color = NULL;

        for (int i = 0; i < n_planets; i++) {
            planet_color = RndColor();

            planets.push_back(new Planet(
                {0.0, RandSign() * initial_pos_z[i]},
                {RandSign() * initial_vel_x[i], 0.0},
                (GLfloat)RandDouble(0.0, 50.0),
                (GLfloat)RandDouble(0.001, 0.05),
                masses[i],
                (GLfloat)RandDouble(i, initial_pos_z[i]),
                planet_color    
            ));   

            // 40% possibility of having a moon
            if((i > 3) and (RandDouble(0.0, 1.0) < 0.4))
                planets[i]->has_moon = true; 
        }

        delete planet_color;
    }

    // Set up planet camera select controls
    planet_select = new bool[planets.size()]{ false };

    camera = new SysCamera(initial_camera_pos[n_planets]);

    // Background
    for (int i = 0; i < 100; i++) {
		bg_dots[i][0] = RandInt(0, RAND_MAX);
		bg_dots[i][1] = RandInt(0, RAND_MAX);
	}

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

void PlanetarySystem::DrawStarDots() {
    // 2D rendering
    glDisable(GL_LIGHTING);

    int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	// 2D rendering
	glDisable(GL_DEPTH_TEST);
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
			int x = bg_dots[i][0] % w;
			int y = bg_dots[i][1] % h;
			glVertex2i(x, y);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void PlanetarySystem::Run() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    DrawStarDots();

	// 3D rendering
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, near_plane, far_plane);

    //glMatrixMode(GL_MODELVIEW); //Dá ruim se descomentar
    //glLoadIdentity();

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