#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void drawFlowerOfLife(float centerX, float centerY, float radius, int layers, float timeOffset, float alphaScale, float scaleMult, float rotationMult);

    float baseRotationAngle;
    float scaleFactor;
    vector<glm::vec2> stars;

    vector<float> wiggleOffsets;
};
