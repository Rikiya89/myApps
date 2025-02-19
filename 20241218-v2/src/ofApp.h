// ofApp.h
#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    // Variables for Perlin noise and shapes
    float time;
    vector<ofVec2f> points;
    int numPoints;
    vector<float> radii;
    vector<ofColor> colors;
    float sacredRatio;
    float animationSpeed;
    vector<float> rotations;
    vector<float> pulseSpeeds;
};
