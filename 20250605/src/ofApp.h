#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofEasyCam cam;
    float time;
    ofLight light;
    ofLight ambient;
};
