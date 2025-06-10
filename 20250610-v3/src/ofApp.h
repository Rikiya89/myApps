#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void drawFlowerOfLife(float cx, float cy, int layers, float baseRadius);

    float time;
};
