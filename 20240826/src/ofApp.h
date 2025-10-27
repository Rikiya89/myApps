#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();

    vector<ofPoint> points;
    vector<ofPoint> velocities;
    vector<ofColor> colors;
    int numPoints;
    float maxRadius;
    float rotationSpeed;
    ofColor gradientStart, gradientEnd;

    void drawGradientBackground();
    ofColor generateRandomColor();
    void drawSymmetry(int numSymmetry);
    void drawParticleTrail(ofPoint point, ofColor color);

    ofPoint noiseOffset; // Perlin noise offset for movement

    // Interaction variables
    bool attractMode = true;
    float attractStrength = 0.1;
    float repelStrength = 0.2;

    void toggleAttractMode(); // Toggle between attraction and repulsion

    // New variables for enhancements
    float colorShiftSpeed;
    ofColor baseColor;
};
