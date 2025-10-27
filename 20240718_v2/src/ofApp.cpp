#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);
    ofSetLineWidth(2);
}

void ofApp::update(){

}


void ofApp::draw(){
    float time = ofGetElapsedTimef();
    int numLines = 300;
    float noiseScale = 0.01;
    
    // Draw a subtle background grid
    ofSetColor(10, 10, 10);
    for(int x = 0; x < ofGetWidth(); x += 20) {
        ofDrawLine(x, 0, x, ofGetHeight());
    }
    for(int y = 0; y < ofGetHeight(); y += 20) {
        ofDrawLine(0, y, ofGetWidth(), y);
    }

    // Draw dynamic lines in black and white
    for(int i = 0; i < numLines; i++) {
        float brightness = ofMap(sin(time + i * 0.1), -1, 1, 100, 255);
        ofSetColor(brightness);

        float x1 = ofNoise(i * noiseScale, time * 0.1) * ofGetWidth();
        float y1 = ofNoise(i * noiseScale + 1000, time * 0.1) * ofGetHeight();
        float x2 = ofNoise(i * noiseScale + 2000, time * 0.1) * ofGetWidth();
        float y2 = ofNoise(i * noiseScale + 3000, time * 0.1) * ofGetHeight();
        
        float lineWidth = ofMap(ofNoise(i * noiseScale, time * 0.05), 0, 1, 1, 5);
        ofSetLineWidth(lineWidth);
        
        ofDrawLine(x1, y1, x2, y2);
    }
}
