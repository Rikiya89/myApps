#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    float time;
    int numRings;
    int circlesPerRing;
    float baseRadius;

    vector<ofVec2f> particles;
};
