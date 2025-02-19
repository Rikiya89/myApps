// ofApp.cpp
#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowShape(1280, 960);
    ofBackground(0);
}

void ofApp::update() {
    // Updates for animation logic can be added here.
}

void ofApp::draw() {
    float time = ofGetElapsedTimef();

    for (int x = 0; x < resolution; x++) {
        for (int y = 0; y < resolution; y++) {
            float posX = ofMap(x, 0, resolution, 0, ofGetWidth());
            float posY = ofMap(y, 0, resolution, 0, ofGetHeight());

            // Adding wave motion based on time and position
            float waveOffset = sin((x + time * animationSpeed) * 0.1) * waveAmplitude;

            // Adding rotational distortion
            float angle = ofMap(ofNoise(x * 0.1, y * 0.1, time * rotationSpeed), 0, 1, -PI, PI);
            float rotatedX = cos(angle) * (posX + waveOffset - ofGetWidth() / 2) - sin(angle) * (posY - ofGetHeight() / 2) + ofGetWidth() / 2;
            float rotatedY = sin(angle) * (posX + waveOffset - ofGetWidth() / 2) + cos(angle) * (posY - ofGetHeight() / 2) + ofGetHeight() / 2;

            // Adding pulse effect for size
            float noiseValue = ofNoise(x * 0.1, y * 0.1, time * timeStep);
            float pulse = sin(time * pulseFrequency + x * 0.1 + y * 0.1) * 0.5 + 0.5;
            float size = ofMap(noiseValue * pulse, 0, 1, 1, 10);

            // Adding dynamic shading with gradient shift
            float gradient = sin(time * gradientSpeed + x * 0.02) * 0.5 + 0.5;
            ofSetColor(baseColor.r * noiseValue * gradient, baseColor.g * noiseValue * gradient, baseColor.b * noiseValue * gradient);
            ofDrawCircle(rotatedX, rotatedY, size);
        }
    }
}
