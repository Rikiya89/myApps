#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofImage image;
    ofMesh staticMesh;
    ofMesh animatedMesh;
    int width, height;
    ofEasyCam cam;
    float time;
};
