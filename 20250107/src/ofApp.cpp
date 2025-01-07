#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(10, 10, 30);  // Dark background
    ofSetCircleResolution(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2); // Center the pattern
    drawSacredPattern(0, 0, 200, 6); // Draw pattern
}

//--------------------------------------------------------------
void ofApp::drawSacredPattern(float x, float y, float radius, int layers) {
    float angleStep = 360.0 / layers;
    float offset = sin(time) * 20; // Animation effect

    for (int i = 0; i < layers; i++) {
        float angle = ofDegToRad(i * angleStep);
        float xPos = x + cos(angle) * (radius + offset);
        float yPos = y + sin(angle) * (radius + offset);

        ofSetColor(127 + 127 * sin(time + i), 127 + 127 * cos(time + i * 0.5), 255, 180);
        ofNoFill();
        ofDrawCircle(xPos, yPos, radius * 0.5); // Circles in layers
        ofFill();
        ofDrawCircle(xPos, yPos, 5); // Center points
    }

    // Recursive pattern
    if (radius > 20) {
        for (int i = 0; i < layers; i++) {
            float angle = ofDegToRad(i * angleStep);
            float xPos = x + cos(angle) * (radius / 2);
            float yPos = y + sin(angle) * (radius / 2);

            drawSacredPattern(xPos, yPos, radius / 2, layers);
        }
    }
}
