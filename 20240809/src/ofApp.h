#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    ofImage roseImage;
    ofMesh mesh;
    float time;
    
    ofEasyCam cam; // Easy camera for interactive view
    ofLight light; // Light to enhance visual depth

    // GUI parameters
    ofxPanel gui;
    ofParameter<float> waveFrequency;
    ofParameter<float> waveAmplitude;
    ofParameter<float> rotationSpeed;
    ofParameter<bool> showWireframe;
};
