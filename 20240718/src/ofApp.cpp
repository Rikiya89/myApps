#include "ofApp.h"


void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
}


void ofApp::update(){

}


void ofApp::draw(){
    ofColor gradientStart(0, 0, 0);
    ofColor gradientEnd(50, 50, 50);
    ofBackgroundGradient(gradientStart, gradientEnd, OF_GRADIENT_CIRCULAR);

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    int numLayers = 15;
    int numShapes = 300;
    float baseRadius = 20;
    float time = ofGetElapsedTimef() * 0.1;

    for (int j = 0; j < numLayers; j++) {
        float layerRadius = baseRadius + j * 30;
        for (int i = 0; i < numShapes; i++) {
            float angle = ofMap(i, 0, numShapes, 0, TWO_PI);
            float offsetX = layerRadius * cos(angle + time * (j + 1) * 0.05);
            float offsetY = layerRadius * sin(angle + time * (j + 1) * 0.05);
            
            ofPushMatrix();
            ofTranslate(offsetX, offsetY);
            ofRotateDeg(ofRadToDeg(angle + time * 20));
            
            ofColor color;
            color.setHsb(ofMap(i + j * numShapes, 0, numShapes * numLayers, 0, 255), 255, 255, ofMap(j, 0, numLayers, 50, 255));
            ofSetColor(color);
            
            float size = ofMap(sin(time * (j + 1) + i), -1, 1, 10, 40);
            if (j % 4 == 0) {
                ofDrawCircle(0, 0, size);
            } else if (j % 4 == 1) {
                ofDrawRectangle(-size / 2, -size / 2, size, size);
            } else if (j % 4 == 2) {
                ofDrawEllipse(0, 0, size, size * 0.5);
            } else {
                ofDrawTriangle(-size / 2, size / 2, 0, -size, size / 2, size / 2);
            }

            ofPopMatrix();
        }
    }

    ofPushMatrix();
    ofRotateDeg(ofRadToDeg(time * 10)); 
    for (int k = 0; k < 8; k++) {
        ofRotateDeg(45);
        ofSetColor(255, 127);
        ofDrawCircle(0, baseRadius, baseRadius * 3);
    }
    ofPopMatrix();
}
