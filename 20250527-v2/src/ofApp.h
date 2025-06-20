#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofEasyCam cam;
    ofMesh mesh;
    int gridSize;
    float spacing;
    float time;
};
