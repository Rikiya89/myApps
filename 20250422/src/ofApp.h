#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    vector<ofVec3f> points;
    float noiseScale;
    float time;
    ofEasyCam cam;

    void drawConnections();
    void drawSpiral();
};
