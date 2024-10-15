#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    ofxBox2d box2d;  // Box2D world for physics
    vector<shared_ptr<ofxBox2dCircle>> particles; // Vector for particles
    vector<ofVec2f> noiseOffsets; // Offsets for Perlin noise
    
    float gravity = 9.8; // Gravity strength
};
