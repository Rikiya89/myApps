#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    circleRadius = 50; // Radius of the circles
    numLayers = 5;     // Number of layers in the flower of life
    animationTime = 0; // Initialize animation time

    ofEnableBlendMode(OF_BLENDMODE_ADD); // Enable additive blending for glow effect
}

//--------------------------------------------------------------
void ofApp::update(){
    animationTime += ofGetLastFrameTime(); // Update animation time
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2); // Center the drawing
    drawFlowerOfLife(0, 0, circleRadius, numLayers, animationTime);
}

//--------------------------------------------------------------
void ofApp::drawFlowerOfLife(float x, float y, float radius, int layers, float time){
    for (int i = 0; i <= layers; i++) {
        for (int j = -i; j <= i; j++) {
            for (int k = -i; k <= i; k++) {
                if (abs(j + k) <= i) {
                    float distance = sqrt(j*j + k*k);
                    float alpha = ofMap(distance, 0, layers, 200, 50);
                    ofColor color;
                    color.setHsb(fmod(ofMap(distance, 0, layers, 150, 255) + time * 20, 255), 255, 255, alpha);

                    float animatedRadius = radius + sin(time * 1.5 + distance) * 10; // Animate the radius
                    ofSetColor(color);
                    ofNoFill();
                    ofSetLineWidth(1.5);
                    ofDrawCircle(x + (j * animatedRadius * 1.5), y + (k * animatedRadius * sqrt(3)), animatedRadius);

                    // Draw glow effect
                    ofSetColor(color, 50);
                    ofSetLineWidth(0.5);
                    ofDrawCircle(x + (j * animatedRadius * 1.5), y + (k * animatedRadius * sqrt(3)), animatedRadius * 1.1);
                    ofDrawCircle(x + (j * animatedRadius * 1.5), y + (k * animatedRadius * sqrt(3)), animatedRadius * 1.2);
                }
            }
        }
    }
}
