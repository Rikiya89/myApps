#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    int numShapes;    // Number of shapes to generate
    float timeOffset; // Used for dynamic movement
};
