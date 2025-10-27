// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(1280, 960);
    ofBackground(0);
    ofSetLineWidth(2);
    
    cols = 100;
    rows = 100;
    scale = 0.02;
}

//--------------------------------------------------------------
void ofApp::update(){
    // Update logic can be added here if needed
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int x = 0; x < cols; x++){
        for(int y = 0; y < rows; y++){
            float noiseVal = ofNoise(x * scale, y * scale, ofGetElapsedTimef() * 0.1);
            float angle = noiseVal * TWO_PI;
            ofVec2f dir = ofVec2f(cos(angle), sin(angle));
            ofVec2f pos = ofVec2f(x * 8, y * 8);
            
            float alpha = ofMap(noiseVal, 0, 1, 80, 255);
            float lineWidth = ofMap(noiseVal, 0, 1, 1, 3);
            
            // Enhanced flow with elegant curving motion
            float radius = ofMap(noiseVal, 0, 1, 5, 12);
            ofVec2f endPos = pos + dir * radius;
            
            ofSetColor(255, alpha);
            ofSetLineWidth(lineWidth);
            ofDrawLine(pos, endPos);
            
            // Glow effect for ethereal aesthetics
            ofSetColor(255, alpha * 0.3);
            ofDrawCircle(endPos, lineWidth * 2);
            
            // Creating interconnected sacred-geometry-inspired patterns
            if ((x + y) % 10 == 0) {
                ofSetColor(255, 80);
                ofNoFill();
                ofDrawCircle(pos, radius * 2);
                ofFill();
            }
        }
    }
}
