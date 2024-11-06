#pragma once
#include "ofMain.h"
#include "Particle.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);

    vector<Particle> particles;
    ofFbo trailLayer1, trailLayer2;
    ofColor bgColor1, bgColor2;
    float time;
    bool repel;
};
