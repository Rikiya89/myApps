// ofApp.h
#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    vector<ofVec2f> particles;
    vector<ofVec2f> velocities;
    int numParticles;
    float noiseScale;
    float lineAlpha;
    float pointSize;
    float maxLineDistance; // Maximum distance for lines
    int maxConnections; // Maximum number of connections per particle
    float gradientAlpha; // Alpha for gradient effect
    float pulseSpeed; // Speed for gradient pulsing
};
