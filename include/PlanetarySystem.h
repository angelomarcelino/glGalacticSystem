#ifndef PLANETARYSYSTEM_H
#define PLANETARYSYSTEM_H

#include "SysCamera.h"
#include "Planet.h"

#include <iostream>
#include <map>

#define WIDTH  1280
#define HEIGHT 720

class PlanetarySystem {
    private:
        // Star
        GLfloat star_color[3];

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

        uint32_t random_seed;

        // Background dots
        int bg_dots[100][2];

        void CheckControls();
        
        // Random Number Generatior
        uint32_t Lehmer32();
	    int RandInt(int min, int max);
	    double RandDouble(double min, double max);
        
        int RandSign();
        GLfloat* RndColor();
        GLfloat* RndStarColor();

        // Background Stars
        void DrawStarDots();

    public:
        PlanetarySystem();
        ~PlanetarySystem();

        void Init();

        void Run();

        void SelectPlanet(int planet_idx);

        void SetRandSeed(uint32_t _random_seed);
        void SetStarColor(GLfloat red, GLfloat green, GLfloat blue);
        
        void KeyboardHandler(int key);
        void KeyboardHandler(unsigned char key);
};

#endif  // PLANETARYSYSTEM_H