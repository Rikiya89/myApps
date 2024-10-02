#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();
        
        void drawPattern(float x, float y, float radius, int depth);
        ofColor getGradientColor(float t, int variant);
        void drawParticles();
        ofColor getBackgroundColor();

        float time;
        int numRows, numCols;
        float baseRadius;
        float waveSpeed, waveFrequency;

        float rotationSpeed;

        // Particle system
        vector<ofVec2f> particles;
        vector<ofVec2f> velocities;
        vector<ofColor> particleColors;

        // Glow and trails
        float trailFadeSpeed;
};
