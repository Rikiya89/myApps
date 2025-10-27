#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    ofEasyCam cam;
    ofMesh mesh;
    vector<ofVec3f> points;
    vector<ofVec3f> velocities;
    vector<float> noiseSeeds;
    vector<vector<ofVec3f>> trails;
    vector<float> particleLife;
    vector<float> particleSize;
    
    float time;
    int numPoints;
    float noiseScale;
    float speed;
    bool showWireframe;
    int trailLength;
    float globalPulse;
    
    void generatePoints();
    void updatePoints();
    void createMesh();
    void drawTrails();
    void drawParticles();
    void drawConnections();
};