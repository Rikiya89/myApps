#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    int gridSize = 10;
    float spacing = 60;
    float noiseScale = 0.12;
    float timeSpeed = 0.3;
    float baseRadius = 5;

    ofEasyCam cam;
};
