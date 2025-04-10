#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void computeMeshNormals(ofMesh& mesh);

    ofImage flowerImg;
    ofMesh mesh;
    ofEasyCam cam;

    ofLight directionalLight;
    ofLight ambientLight;
    ofMaterial material;

    int resolutionX = 100;
    int resolutionY = 100;
    float meshScale = 4.0;
    float heightScale = 100.0;
};
