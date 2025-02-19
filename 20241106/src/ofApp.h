#pragma once
#include "ofMain.h"

// Define a structure for Particle
struct Particle {
    ofVec2f position;
    ofVec2f velocity;
    ofColor color;
    float size;
    float opacity;
};

// Define the ofApp class
class ofApp : public ofBaseApp {
public:
    // Core openFrameworks methods
    void setup();
    void update();
    void draw();

private:
    // Variables
    ofImage dahlia;                  // The image to base the particles on
    std::vector<Particle> particles; // Vector of particles
    float noiseScale = 0.003;        // Scale for noise movement (adjusted for smoother motion)
    float speed = 0.8;               // Speed multiplier for particles
};
