#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    numRibbons = 18;
    numPoints = 160;
    time = 0.0f;

    cam.setDistance(850);  // Starting camera distance
    cam.setNearClip(0.1f);
    cam.setFarClip(4000.f);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.016;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    cam.begin(); // << Use ofEasyCam

    // Draw ribbons
    for (int i = 0; i < numRibbons; ++i) {
        float phi = ofMap(i, 0, numRibbons, 0, TWO_PI);

        vector<glm::vec3> path;
        for (int j = 0; j < numPoints; ++j) {
            float t = (float)j / (numPoints-1);
            float angle = t * TWO_PI * 2.0 + phi;
            float spiral = 160 + 60 * ofSignedNoise(i*0.13, j*0.13, time * 0.7 + i*0.06);
            float x = spiral * cos(angle) + 65 * sin(time + phi + t * 2.4);
            float y = spiral * sin(angle) + 40 * sin(time * 0.7 + phi * 1.1 + t * 2.2);
            float z = 80 * ofSignedNoise(j*0.06, i*0.07, time * 0.7) + 120 * t - 60;

            path.push_back(glm::vec3(x, y, z));
        }
        drawRibbon(path);
    }

    cam.end();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::drawRibbon(const vector<glm::vec3>& path) {
    float thickness = 8.5;
    ofSetPolyMode(OF_POLY_WINDING_ODD);

    for (size_t i = 1; i < path.size(); ++i) {
        float alpha = ofMap(i, 0, path.size(), 170, 24);
        float shade = ofMap(i, 0, path.size(), 255, 32);

        ofSetColor(shade, alpha);

        glm::vec3 p0 = path[i-1];
        glm::vec3 p1 = path[i];
        glm::vec3 dir = glm::normalize(p1 - p0);
        glm::vec3 side = glm::cross(dir, glm::vec3(0,1,0));
        side = glm::normalize(side) * thickness;

        ofDrawTriangle(p0 + side, p0 - side, p1 + side);
        ofDrawTriangle(p0 - side, p1 + side, p1 - side);
    }
}
