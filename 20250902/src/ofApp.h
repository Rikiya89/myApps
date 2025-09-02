// 3D generative sacred geometry in black & white
#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;

private:
    // Scene
    ofEasyCam cam;
    ofIcoSpherePrimitive ico;

    // Geometry data
    std::vector<glm::vec3> spherePoints;   // Fibonacci sphere points
    std::vector<glm::vec3> circleAxes;     // Great-circle axes
    std::vector<glm::ivec2> edges;         // Indices into spherePoints

    // Parameters
    int    numPoints = 420;    // Fibonacci points
    int    numCircles = 28;    // Great circles around sphere
    float  baseRadius = 320.0f;
    float  t = 0.0f;
    bool   paused = false;
    bool   invert = false;     // Optional invert (still black/white)

    // Helpers
    void buildFibonacciSphere(int n, float radius);
    void buildGreatCircleAxes(int n);
    void buildEdges();
    void drawGreatCircles(float radius, float phase);
    void drawFibonacciWeb(float radius, float phase);
    void drawPlatonicCore(float radius, float phase);
};
