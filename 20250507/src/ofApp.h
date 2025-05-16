#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;

private:
    static const int NUM_POINTS = 600;
    static const int NUM_PARTICLES = 300;
    static const float BASE_RADIUS;

    ofEasyCam cam;
    ofVboMesh meshPrimary;
    ofVboMesh meshSecondary;
    std::vector<glm::vec3> basePos;

    std::vector<glm::vec3> particles; // Floating stars

    float rotationOffset;
};
