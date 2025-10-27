#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

        // GUI
        ofxPanel gui;
        ofxFloatSlider radius;
        ofxIntSlider numCircles;

        // Drawing function for Flower of Life
        void drawFlowerOfLife(float x, float y, float radius, int numCircles);
};
