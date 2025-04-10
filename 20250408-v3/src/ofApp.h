#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    vector<glm::vec3> points;
    int numPoints = 2500;
    float angleStep = 137.5;
    float spiralScale = 5.0;
    float time;

    ofEasyCam cam;
};
