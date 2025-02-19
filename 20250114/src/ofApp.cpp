// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0); // Set background color to black
    ofSetCircleResolution(100); // Smooth circles
    ofSetFrameRate(60); // Frame rate for smooth animation
}

//--------------------------------------------------------------
void ofApp::update(){
    // No updates needed for this static example
}

//--------------------------------------------------------------
void ofApp::draw(){
    float time = ofGetElapsedTimef(); // Get elapsed time for animation

    for (int x = 0; x < ofGetWidth(); x += 20) {
        for (int y = 0; y < ofGetHeight(); y += 20) {
            float noiseValue = ofNoise(x * 0.01, y * 0.01, time * 0.1); // Generate noise value
            float radius = ofMap(noiseValue, 0, 1, 2, 20); // Map noise to circle radius
            int alpha = ofMap(noiseValue, 0, 1, 50, 255); // Map noise to alpha value for transparency
            ofSetColor(255, alpha); // Set grayscale color with transparency
            ofDrawCircle(x, y, radius); // Draw the circle

            // Add connecting lines for more dynamic visuals
            if (x > 20 && y > 20) {
                ofSetLineWidth(1.5);
                ofSetColor(200, 100); // Slightly brighter lines for better visibility
                ofDrawLine(x, y, x - 20, y - 20);
                ofDrawLine(x, y, x - 20, y);
                ofDrawLine(x, y, x, y - 20);
            }

            // Add rotating arcs for more detail
            float angle = ofMap(noiseValue, 0, 1, 0, TWO_PI);
            ofSetLineWidth(1.0);
            ofSetColor(255, 150);
            ofPushMatrix();
            ofTranslate(x, y);
            ofRotateRad(angle);
            ofNoFill();

            // Create an arc using ofPath
            ofPath arcPath;
            arcPath.setCircleResolution(100);
            arcPath.setStrokeColor(ofColor(255, 150));
            arcPath.setFilled(false);
            arcPath.setStrokeWidth(1.0);
            arcPath.arc(0, 0, radius * 2, radius * 2, 0, 180);
            arcPath.draw();

            // Add radiating lines from the center of the arc
            ofSetColor(255, 100);
            for (float angle = 0; angle < 360; angle += 45) {
                float radian = ofDegToRad(angle);
                float x1 = cos(radian) * radius * 1.5;
                float y1 = sin(radian) * radius * 1.5;
                float x2 = cos(radian) * radius * 2.5;
                float y2 = sin(radian) * radius * 2.5;
                ofDrawLine(x1, y1, x2, y2);
            }

            // Add sacred geometry-inspired patterns
            ofSetColor(255, 50);
            for (int i = 0; i < 6; i++) {
                float innerRadius = radius * 1.2;
                float outerRadius = radius * 1.8;
                float angle1 = i * TWO_PI / 6;
                float angle2 = (i + 1) * TWO_PI / 6;
                float x1 = cos(angle1) * innerRadius;
                float y1 = sin(angle1) * innerRadius;
                float x2 = cos(angle2) * outerRadius;
                float y2 = sin(angle2) * outerRadius;
                ofDrawLine(x1, y1, x2, y2);
            }

            ofPopMatrix();
        }
    }
}
