#include "../include/SysCamera.h"

#include <iostream>

// Class Constructor
SysCamera::SysCamera() {
    x_pos = 20.0f;
    y_pos = 25.0f;
    z_pos = 20.0f;

    x_rot = -90.0f;
    y_rot = 0.0f;
    angle = 0.0f;

    radius = 10.0f;
    camera_speed = 1.0f;

    x_lookat = 0.0f;
    y_lookat = 0.0f;
    z_lookat = 0.0f;

    camera_lock = false;
}

// Class Destructor
SysCamera::~SysCamera() {}

void SysCamera::Update(std::map<std::string, bool> &keys_state, GLdouble* camera_pos) {
    // If spacebar is pressed, lock the camera to the currently selected planet. If toggled off, reset the camera
    if (keys_state["Space"]) {
        if (!camera_lock) {
            camera_lock = true;
        } else {
            camera_lock = false;
            CameraReset();
        }

        keys_state["Space"] = false;
    }

    // Lock the camera to the currently selected planet
    if (camera_lock and camera_pos[0] != 0 and camera_pos[1] != 0) {
        x_pos = camera_pos[0] * 1.5;
        z_pos = camera_pos[1] * 1.5;

        x_lookat = camera_pos[0];
        z_lookat = camera_pos[1];

        y_pos = 4.0f;

    } else if (keys_state["Up"] and y_pos > 0) {  // Otherwise update the camera from button pressed
        y_pos -= camera_speed;
    } else if (keys_state["Down"] ) {
        y_pos += camera_speed;
    } else if (keys_state["Left"]) {
        z_pos += camera_speed;
    } else if (keys_state["Right"] and z_pos > 0) {
        z_pos -= camera_speed;
    } else if (keys_state["q"]) {
        x_pos += camera_speed;
    } else if (keys_state["e"] and x_pos > 0) {
        x_pos -= camera_speed;
    }

    // Reset keys pressed
    keys_state["Up"] = false;
    keys_state["Down"] = false;
    keys_state["Left"] = false;
    keys_state["Right"] = false;
    keys_state["q"] = false;
    keys_state["e"] = false;

    // std::cout << "Camera positon: (" << x_pos << ", " << y_pos << ", " << z_pos << ")\n";

    // Update camera
    gluLookAt(x_pos, y_pos, z_pos, x_lookat, y_lookat, z_lookat, 0.0f, 1.0f, 0.0f);
}

void SysCamera::CameraReset() {
    x_pos = 20.0f;
    y_pos = 25.0f;
    z_pos = 20.0f;

    x_rot = -90.0f;
    y_rot = 0.0f;
    angle = 0.0f;

    radius = 10.0f;
    camera_speed = 3.0f;

    x_lookat = 0.0f;
    y_lookat = 0.0f;
    z_lookat = 0.0f;

    camera_lock = false;
}