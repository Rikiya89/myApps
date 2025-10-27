#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

    public:
        void setup() override;
        void update() override;
        void draw() override;
        void keyPressed(int key) override;

        vector<glm::vec3> points;
        vector<glm::vec3> originalPoints;
        ofEasyCam cam;
        ofMesh mesh;
};
