#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    int numPoints;
    float baseScale;
    float phi;
    float tau;
    float goldenAngle;
    ofPoint center;
    float rotationSpeed;
    float breathingSpeed;
    float noiseScale;
};
