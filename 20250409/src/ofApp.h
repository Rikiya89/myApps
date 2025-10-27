#pragma once
#include "ofMain.h"

class Node {
public:
    glm::vec3 position;
    glm::vec3 velocity;
};

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    vector<Node> nodes;
    int numNodes = 160;
    float connectDistance = 140.0;

    vector<glm::vec3> stars;
    float time;

    float orbitRadius = 600;
    float orbitSpeed = 0.2;
};
