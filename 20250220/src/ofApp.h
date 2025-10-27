#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    // Particle properties for swirling motion
    vector<ofVec2f> positions;
    vector<float> angles;      // Current angle for each particle (in radians)
    vector<float> radii;       // Distance from the center for each particle
    vector<ofColor> colors;    // Particle colors
    vector<float> noiseSeeds;  // Unique noise seed for organic evolution

    // Extra animated parameters for each particle
    vector<float> rotations;      // Additional rotation offset
    vector<float> scaleOffsets;   // For subtle scaling animation
    
    int numParticles = 200;
    float time = 0.0;
    float noiseSpeed = 0.0005; // Controls noise evolution
    float gradientOffset = 0.0;
    
    // FBOs for feedback and bloom effects
    ofFbo mainFbo;
    ofFbo bloomFbo;
    
    // Helper function to draw a dynamic, animated star shape with noise deformation
    void drawComplexShape(const ofVec2f &pos, float radius, const ofColor &col, float rotation, float scale);
};
