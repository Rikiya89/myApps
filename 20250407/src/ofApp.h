#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofEasyCam cam;
    float angle = 0.0;

    vector<glm::vec3> spherePositions;
};
