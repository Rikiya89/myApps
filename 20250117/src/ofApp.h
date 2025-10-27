#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    // Frame buffer for "persistent" drawing
    ofFbo fbo;

    // Particle data
    int numParticles;
    vector<ofVec2f> positions;
    vector<ofVec2f> velocities;

    // Noise parameters
    float noiseScale;
    float noiseSpeed;
    float noiseStrength;
};
