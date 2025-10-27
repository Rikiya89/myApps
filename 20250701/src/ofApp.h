#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void drawPointCloud();

    ofEasyCam cam;
    vector<ofVec3f> points;
    int numPoints = 5000;
    float time;
};
