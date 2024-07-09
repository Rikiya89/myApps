#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
        
        float noiseScale;
        float time;
        vector<ofVec2f> points;
        vector<float> sizes;
        vector<ofColor> colors;
};
