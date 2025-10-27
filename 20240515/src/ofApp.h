#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseMoved(int x, int y );

    ofImage image;
    ofMesh mesh;
    ofEasyCam cam;
    ofLight light;
};
