#ifndef PLANETARYSYSTEM_H
#define PLANETARYSYSTEM_H

#include <GL/glut.h>

#include "SysCamera.h"
#include "Planet.h"

#include <iostream>
#include <map>

// Window globals
#define WIDTH		1280
#define HEIGHT		720

class PlanetarySystem {
    private:
        // Planets
        std::vector<Planet*> planets;

        SysCamera *camera;
        GLdouble camera_pos[2] = {0, 0};

        bool toggle_wire_frame = false;
        bool toggle_gravity = true;

        bool *planet_select;

        // Buttons        
        std::map<std::string, bool> keys_state = {
            {"Up", false},
            {"Down", false},
            {"Right", false},
            {"Left", false},
            {"q", false},
            {"w", false},
            {"s", false},
            {"d", true},
            {"Space", false}
        };

        // Lighting and materials variables
        GLfloat light_position[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat diffuse_material[4] = {0.5, 0.5, 0.5, 1.0};
        GLfloat mat_specular[4] = {1.0, 1.0, 1.0, 1.0};

        GLfloat fov = 60.0f;
        GLfloat far_plane = 100000.0f;
        GLfloat near_plane = 1.0f;

        void CheckControls();

    public:
        PlanetarySystem();
        ~PlanetarySystem();

        void Init();

        void Run();

        void SelectPlanet(int planet_idx);
        
        void KeyboardHandler(int key);
        void KeyboardHandler(unsigned char key);
};

#endif  // PLANETARYSYSTEM_H