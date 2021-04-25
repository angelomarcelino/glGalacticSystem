#include <string>

#include "../include/PlanetarySystem.h"

#define FPS 30
#define REDISPLAY_INTERVAL 1000 / FPS

PlanetarySystem *planetary_system = new PlanetarySystem;

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10.0, 15.0, 10.0, 0.0, 0.0, 0., 0, 1., 0);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(REDISPLAY_INTERVAL, timer, 0);
}

int main(int argc, char **argv) {

    if(argc < 5) {
        std::cout << "Missing Arguments!\nUsage: " << argv[0] 
            << " random_sedd star_color_red star_color_green star_color_blue\n";

        exit(-1);
    }

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Planetary System");


    planetary_system->SetRandSeed(std::stoul(argv[1], nullptr, 0));
    planetary_system->SetStarColor(
        std::stof(argv[2]),
        std::stof(argv[3]),
        std::stof(argv[4])
    );

    planetary_system->Init();

    glutTimerFunc(REDISPLAY_INTERVAL, timer, 0);

    glutDisplayFunc([]() { planetary_system->Run(); });
    glutReshapeFunc(reshape);

    glutSpecialFunc([](int key, int x, int y) { planetary_system->KeyboardHandler(key); });
    glutKeyboardFunc([](unsigned char key, int x, int y) { planetary_system->KeyboardHandler(key); });

    glutMainLoop();

    delete planetary_system;
    return 0;
}