#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;

    void regeneratePoints();               // rebuild the point cloud

    ofEasyCam          cam;
    std::vector<glm::vec3> points;

    // GUI parameters
    ofxPanel           gui;
    ofParameter<int>   numPoints;
    ofParameter<float> noiseScale;
    ofParameter<float> noiseSpeed;
    ofParameter<float> connectionDist;
    ofParameter<float> pointSize;

    float              t = 0.f;            // time accumulator
};
