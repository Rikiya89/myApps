#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofBackground(0);                      // Pure black canvas
    ofEnableBlendMode(OF_BLENDMODE_ADD);  // Additive glow effect
    ofSetCircleResolution(100);
    ofNoFill();
    time = 0.0;
}

void ofApp::update() {
    time = ofGetElapsedTimef();
}

void ofApp::draw() {
    ofPushMatrix();

    // Center and slow orbit for spiritual feel
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotateDeg(sin(time * 0.05) * 2);  // very slow sway

    // Breathing radius
    float dynamicRadius = 38 + sin(time * 0.8) * 6;

    drawFlowerOfLife(0, 0, 5, dynamicRadius);
    ofPopMatrix();
}

void ofApp::drawFlowerOfLife(float cx, float cy, int layers, float baseRadius) {
    float dx = baseRadius * sqrt(3);
    float dy = baseRadius * 1.5;

    for (int row = -layers; row <= layers; row++) {
        for (int col = -layers; col <= layers; col++) {
            float x = cx + col * dx + (row % 2) * dx / 2;
            float y = cy + row * dy;

            if (abs(col + row) <= layers) {
                float dist = ofDist(x, y, 0, 0);
                float alpha = ofMap(dist, 0, layers * dx, 255, 40, true);
                
                // Glowing strokes
                for (int i = 0; i < 5; i++) {
                    float r = baseRadius + i * 0.4;
                    ofSetColor(255, alpha * ofMap(i, 0, 4, 1.0, 0.05));
                    ofDrawCircle(x, y, r);
                }
            }
        }
    }
}
