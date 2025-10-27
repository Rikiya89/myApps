// ofApp.h
#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mouseMoved(int x, int y);
    
private:
    // Animation variables
    float time;
    float rotationSpeed;
    float pulseSpeed;
    
    // Visual parameters
    int numLayers;
    int pointsPerLayer;
    float baseRadius;
    float maxRadius;
    
    // Colors
    ofColor bgColor;
    ofColor primaryColor;
    ofColor secondaryColor;
    
    // Interaction
    ofVec2f mousePos;
    float mouseInfluence;
    
    // Helper functions
    void drawSacredGeometry(float x, float y, float radius, int points, float rotation);
    void drawFlower(float x, float y, float radius, int petals);
    ofColor lerpColor(ofColor c1, ofColor c2, float amt);
};
