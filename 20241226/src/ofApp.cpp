// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0); // Black background
    ofSetFrameRate(60);

    numParticles = 1000; // Adjusted particle count for performance and beauty
    noiseScale = 0.004;
    lineAlpha = 60; // Increased visibility for connecting lines
    pointSize = 3.0; // Slightly larger particles for elegance
    maxLineDistance = 120; // Extended line distance for interconnected visuals
    maxConnections = 6; // Allow more connections per particle
    gradientAlpha = 80; // Increase gradient intensity
    pulseSpeed = 1.0; // Speed for gradient pulsing

    for (int i = 0; i < numParticles; i++) {
        particles.push_back(ofVec2f(ofRandomWidth(), ofRandomHeight()));
        velocities.push_back(ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1)));
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < particles.size(); i++) {
        float noiseValue = ofNoise(particles[i].x * noiseScale, particles[i].y * noiseScale, ofGetElapsedTimef());
        float angle = ofMap(noiseValue, 0, 1, 0, TWO_PI);

        ofVec2f direction(cos(angle), sin(angle));
        velocities[i] += direction * 0.1;
        velocities[i] *= 0.95; // Add smoother friction

        particles[i] += velocities[i];

        // Wrap around screen edges
        if (particles[i].x < 0) particles[i].x = ofGetWidth();
        if (particles[i].x > ofGetWidth()) particles[i].x = 0;
        if (particles[i].y < 0) particles[i].y = ofGetHeight();
        if (particles[i].y > ofGetHeight()) particles[i].y = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255); // White particles
    for (const auto& particle : particles) {
        ofDrawCircle(particle, pointSize);
    }

    // Draw connecting lines for nearby particles
    for (int i = 0; i < particles.size(); i++) {
        int connections = 0;
        for (int j = i + 1; j < particles.size(); j++) {
            if (connections >= maxConnections) break; // Limit connections per particle

            float distance = particles[i].distance(particles[j]);
            if (distance < maxLineDistance) {
                float alpha = lineAlpha + ofMap(distance, 0, maxLineDistance, 0, 50); // Slight variation
                ofSetColor(255, alpha);
                ofDrawLine(particles[i], particles[j]);
                connections++;
            }
        }
    }

    // Add a vibrant radial gradient effect with pulsing
    float pulse = sin(ofGetElapsedTimef() * pulseSpeed) * 0.5 + 0.5; // Pulsing effect
    float dynamicAlpha = gradientAlpha * pulse;
    ofColor gradientColor = ofColor(0, 0, 0, dynamicAlpha);
    ofMesh gradientMesh;
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    gradientMesh.addVertex(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, 0));
    gradientMesh.addColor(ofColor(0, 0, 0, 0));

    for (int i = 0; i <= 360; i += 10) {
        float rad = ofDegToRad(i);
        float x = ofGetWidth() / 2 + cos(rad) * ofGetWidth();
        float y = ofGetHeight() / 2 + sin(rad) * ofGetWidth();
        gradientMesh.addVertex(ofVec3f(x, y, 0));
        gradientMesh.addColor(gradientColor);
    }

    gradientMesh.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
void ofApp::gotMessage(ofMessage msg) {}
