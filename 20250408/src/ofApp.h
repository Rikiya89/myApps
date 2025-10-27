#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    int numRings = 3;
    int petalsPerRing = 36;
    float baseRadius = 200;
    float angleOffset = 0;

    std::vector<std::vector<glm::vec3>> ringPetalPositions;
};
