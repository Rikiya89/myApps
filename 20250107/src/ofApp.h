#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void drawSacredPattern(float x, float y, float radius, int layers);

    float time;
};
