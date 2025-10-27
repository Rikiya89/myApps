#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);
    void drawTorus(float radius, float tubeRadius, int sides, int rings);

    ofEasyCam cam;
    float time;

    int numTori;
    vector<ofVec3f> positions;
    vector<float> baseRotations;
};
