#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void drawRibbon(const vector<glm::vec3>& path);

    int numRibbons;
    int numPoints;
    float time;

    ofEasyCam cam; // << Add this line!
};
