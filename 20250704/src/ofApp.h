// ofApp.h
#pragma once

#include "ofMain.h"
#include <vector>

struct Sparkle {
    ofPoint pos;
    float life; // normalized (0 to 1)
    float size;
    float angle;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    std::vector<ofPoint> points;
    std::vector<Sparkle> sparkles;
    float time;
    ofFbo trailFbo;
};
