#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    
private:
    // -----------------------------------------
    // 1) Spirograph + Polygon Parameters
    float R;                // Radius of fixed circle for spirograph
    float r;                // Radius of rolling circle
    float d;                // Offset from rolling circle center
    float thetaStep;        // Spirograph step
    float time;             // Time for noise-driven animation
    const int NUM_LAYERS = 5;
    const int SIDES_PER_LAYER = 6;
    float RADIUS_STEP;      // Distance between concentric polygons
    std::vector<float> layerRotationOffsets; // each layer’s rotation offset

    // -----------------------------------------
    // 2) Main FBO for Drawing Scene
    ofFbo mainFBO;

    // -----------------------------------------
    // 3) FBOs for Blur Passes
    ofFbo blurFbo1;
    ofFbo blurFbo2;

    // -----------------------------------------
    // 4) Shaders for Horizontal & Vertical Blur
    ofShader blurX;
    ofShader blurY;

    // -----------------------------------------
    // 5) Parameters
    float blurAmount;   // How strong the blur is
    float globalScale;  // Uniform scale for the entire scene

    // -----------------------------------------
    // Helper Functions
    void drawScene();
    void drawSpirograph();
    void drawPolygons();
    void applyBlur();
};
