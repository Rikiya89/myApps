#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(10, 10, 24);
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);  // Glowing effect

    gridSizeX = 36;
    gridSizeY = 20;
    spacing = 38.0f;
    time = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.012;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Soft fade background for motion trails
    ofSetColor(10, 10, 24, 28);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    cam.begin();
    ofPushMatrix();
    ofRotateYDeg(sin(time * 0.7) * 16.0);

    // Center the grid
    float centerX = 0;
    float centerY = 0;
    float centerZ = 0;

    for (int y = 0; y < gridSizeY; y++) {
        for (int x = 0; x < gridSizeX; x++) {
            float nx = x * spacing - (gridSizeX * spacing) / 2;
            float ny = y * spacing - (gridSizeY * spacing) / 2;

            float noiseVal = ofNoise(x * 0.14, y * 0.14, time * 0.9);
            float offsetZ = ofMap(noiseVal, 0, 1, -80, 80);

            float hue = fmod(noiseVal * 200 + time * 40 + x * 4 + y * 6, 255);
            float sat = 200 - noiseVal * 80;
            float bri = 180 + sin(time + x + y) * 45;
            ofColor col = ofColor::fromHsb(hue, sat, bri, 180);

            ofSetColor(col);
            ofDrawCircle(nx, ny, offsetZ, 10 + noiseVal * 16);
        }
    }

    ofPopMatrix();
    cam.end();
}
