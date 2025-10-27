#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableLighting();
    ofSetCircleResolution(64);
    ofEnableBlendMode(OF_BLENDMODE_ADD);  // Additive blending for glowing effect
}

void ofApp::update() {}

void ofApp::draw() {
    ofBackground(0);
    cam.begin();

    float time = ofGetElapsedTimef() * timeSpeed;

    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                float nx = x * spacing;
                float ny = y * spacing;
                float nz = z * spacing;

                float noiseVal = ofNoise(x * noiseScale, y * noiseScale, z * noiseScale, time);
                float offset = ofMap(noiseVal, 0, 1, -25, 25);
                float radius = baseRadius + ofMap(noiseVal, 0, 1, 0, 10);

                // Gradient brightness
                float brightness = ofMap(noiseVal, 0, 1, 180, 255);
                ofSetColor(brightness);

                ofPushMatrix();
                ofTranslate(
                    nx - gridSize * spacing / 2,
                    ny - gridSize * spacing / 2,
                    nz - gridSize * spacing / 2 + offset
                );

                // Slow subtle rotation
                float rot = time * 10 + (x + y + z) * 2.0;
                ofRotateDeg(rot, 0.3, 0.5, 0.2);

                ofDrawSphere(radius);
                ofPopMatrix();
            }
        }
    }

    cam.end();
}
