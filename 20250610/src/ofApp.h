#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();               // Initialize window and variables
    void update();              // Update animation state
    void draw();                // Draw the generative art
    void mouseMoved(int x, int y); // Handle mouse movement

    float time;                 // Animation time counter
    float spacing;              // Distance between each grid point
    int cols, rows;             // Grid size
    float phi;                  // Golden ratio constant (for aesthetics)

    ofVec2f mousePos;           // Current mouse position
    float influenceRadius;      // How far the mouse affects the field
};
