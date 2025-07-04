#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    float baseRadius;
    float spacing;
    int rings, sectors;
    float time;
    ofEasyCam cam;
};
