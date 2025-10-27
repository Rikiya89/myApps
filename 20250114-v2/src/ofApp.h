#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void drawSacredGeometry();
    void drawMandala();  // Add this declaration
    void drawCosmicParticles();

    ofEasyCam cam;
    std::vector<glm::vec3> particlePositions;
    std::vector<glm::vec3> particleVelocities;
    float angle;
};
