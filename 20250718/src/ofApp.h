#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    int mode = 0;                    // which visual to draw
    float time = 0;                  // global time

    // common
    ofFbo       fbo;                 // for trails
    vector<ofColor> palette;         // pastel palette

    // mode 1: particle system
    struct Particle {
        ofVec2f pos;
        ofVec2f vel;
        ofColor color;
    };
    vector<Particle> particles;

    // mode 3: sacred geometry palette
    vector<ofColor> sacredPalette;
};
