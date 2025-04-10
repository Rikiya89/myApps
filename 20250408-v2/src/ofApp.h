#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    vector<glm::vec3> points;
    int numPoints;
    float radius;

    ofEasyCam cam;
};
