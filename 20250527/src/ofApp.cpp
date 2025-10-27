#include "ofApp.h"

void ofApp::setup() {
    ofBackground(5, 5, 10);
    ofSetCircleResolution(100);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(60);

    numRings = 6;
    circlesPerRing = 48;
    baseRadius = 50;

    for (int i = 0; i < 100; i++) {
        particles.push_back(ofVec2f(ofRandomWidth(), ofRandomHeight()));
    }
}

void ofApp::update() {
    time = ofGetElapsedTimef();
}

void ofApp::draw() {
    // Starfield background
    for (auto &p : particles) {
        float flicker = ofNoise(p.x * 0.01, p.y * 0.01, time * 0.2);
        ofSetColor(255, 255, 255, 10 + flicker * 40);
        ofDrawCircle(p, flicker * 2.0 + 1.0);
    }
    
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    for (int r = 0; r < numRings; r++) {
        float radiusOffset = baseRadius * pow(1.45, r);
        float rotation = time * (0.1 + r * 0.03);

        for (int i = 0; i < circlesPerRing; i++) {
            float angle = TWO_PI * i / circlesPerRing + rotation;
            float x = cos(angle) * radiusOffset;
            float y = sin(angle) * radiusOffset;

            float pulse = sin(time * 2.0 + i * 0.3 + r) * 6;
            float depth = 1.0 - (float)r / (numRings + 2);
            float glowAlpha = 80 + depth * 100;
            float scale = 0.8 + 0.2 * depth;

            ofSetColor(255, 255, 255, glowAlpha);
            ofNoFill();
            ofSetLineWidth(1.5);
            ofDrawCircle(x, y, (baseRadius * scale) + pulse);

            // Glow overlay
            for (int g = 0; g < 3; g++) {
                float glowScale = 1.0 + g * 0.02;
                ofSetColor(255, 255, 255, 10);
                ofDrawCircle(x, y, ((baseRadius * scale) + pulse) * glowScale);
            }
        }
    }

    // Central aura
    float centerPulse = sin(time * 1.5) * 12;
    ofSetColor(255, 255, 255, 200);
    ofDrawCircle(0, 0, baseRadius * 1.2 + centerPulse);

    // Glow echo
    ofSetColor(255, 255, 255, 15);
    for (int i = 0; i < 3; i++) {
        ofDrawCircle(0, 0, baseRadius * (1.5 + i * 0.2) + centerPulse);
    }
}
