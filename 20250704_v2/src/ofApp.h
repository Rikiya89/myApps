// ofApp.h
#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

private:
    ofEasyCam                       cam;
    vector<ofVboMesh>               layers;
    vector<vector<glm::vec3>>       baseDirs;
    vector<float>                   baseRadii;
    vector<float>                   noiseScales;
    vector<float>                   pulseSpeeds;
    vector<ofColor>                 chakraColors;
    float                           startTime;
};
