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
    int numSides = 16; // Higher symmetry for complex mandala
    
    float noiseScale = 0.004; // Subtle noise for smooth movement
    float noiseStrength = 0.6; // Strength for gentle motion
    ofColor baseColor;
    float layerRotationSpeeds[3]; // Different speeds for each mandala layer
};
