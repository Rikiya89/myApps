// ofApp.cpp
#include "ofApp.h"

void ofApp::setup() {
    ofBackgroundGradient(ofColor(0), ofColor(32), OF_GRADIENT_CIRCULAR); // Add a subtle gradient background
    ofSetFrameRate(60);
    numShapes = 600; // Increase the number of shapes for more detail
    timeOffset = 0.0;
}

void ofApp::update() {
    timeOffset += 0.01; // Smooth animation speed
}

void ofApp::draw() {
    // Dynamic circular patterns with gradient colors
    for (int i = 0; i < numShapes; i++) {
        float angle = i * TWO_PI / numShapes + timeOffset;
        float radius = 240 + sin(timeOffset + i * 0.1) * 140;
        float x = ofGetWidth() / 2 + cos(angle) * radius;
        float y = ofGetHeight() / 2 + sin(angle) * radius;

        float size = ofRandom(3, 8) + sin(timeOffset + i * 0.15) * 2;
        ofSetColor(200 + sin(i * 0.1) * 55, 150 + cos(i * 0.1) * 55, 255, 180); // Gradient glow
        ofDrawCircle(x, y, size);
    }

    // Enhanced mandala with intricate layered petals
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotateDeg(ofGetFrameNum() * 0.25);

    for (int i = 0; i < 24; i++) {
        float innerRadius = 60 + sin(timeOffset * 2) * 40;
        float outerRadius = 180 + cos(timeOffset * 2) * 60;
        float angle1 = i * TWO_PI / 24;
        float angle2 = (i + 1) * TWO_PI / 24;

        ofSetColor(180 + i * 3, 120 + i * 5, 200 + i * 4); // Intricate gradient
        ofDrawLine(cos(angle1) * innerRadius, sin(angle1) * innerRadius,
                   cos(angle2) * outerRadius, sin(angle2) * outerRadius);

        float petalRadius = 100 + sin(timeOffset + i * 0.5) * 30;
        ofSetColor(200, 150, 255, 200);
        ofDrawCircle(cos(angle1) * petalRadius, sin(angle1) * petalRadius, 12);
    }
    ofPopMatrix();

    // Add glowing aura layers for depth
    for (int i = 0; i < 10; i++) {
        float alpha = 25 + sin(timeOffset + i * 0.6) * 30;
        ofSetColor(255, 180, 255, alpha);
        ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 220 + i * 50);
    }

    // Radiating subtle background lines for harmony
    for (int i = 0; i < 36; i++) {
        float angle = i * TWO_PI / 36;
        float x1 = ofGetWidth() / 2 + cos(angle) * 300;
        float y1 = ofGetHeight() / 2 + sin(angle) * 300;
        float x2 = ofGetWidth() / 2 + cos(angle) * 600;
        float y2 = ofGetHeight() / 2 + sin(angle) * 600;

        ofSetColor(100, 50, 150, 30);
        ofDrawLine(x1, y1, x2, y2);
    }
}
