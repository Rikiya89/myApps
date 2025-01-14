#include "ofApp.h"

void ofApp::setup() {
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);  // Additive blending for spiritual glow
    ofSetFrameRate(60);

    // Initialize particles
    int numParticles = 400;
    for (int i = 0; i < numParticles; i++) {
        glm::vec3 position(ofRandom(-500, 500), ofRandom(-500, 500), ofRandom(-500, 500));
        glm::vec3 velocity(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        particlePositions.push_back(position);
        particleVelocities.push_back(velocity);
    }

    angle = 0.0f;
}

void ofApp::update() {
    float time = ofGetElapsedTimef();

    // Update particle positions with gentle rising motion
    for (size_t i = 0; i < particlePositions.size(); i++) {
        particlePositions[i] += particleVelocities[i] * 0.5;
        particlePositions[i].y += sin(time * 0.5 + i) * 0.5;  // Gentle upward drift

        // Keep particles within bounds
        if (glm::length(particlePositions[i]) > 500) {
            particlePositions[i] = glm::normalize(particlePositions[i]) * 500;
            particleVelocities[i] *= -1;
        }
    }

    // Update rotation angle
    angle += 0.5f;
}

void ofApp::draw() {
    cam.begin();

    // Draw sacred geometry
    drawSacredGeometry();

    // Draw spiritual mandala
    drawMandala();

    // Draw cosmic particles
    drawCosmicParticles();

    cam.end();
}

void ofApp::drawSacredGeometry() {
    ofPushMatrix();

    // Rotate geometry
    ofRotateDeg(angle, 1, 1, 0);

    // Pulsating central sphere
    float pulse = 50 + sin(ofGetElapsedTimef() * 2) * 5;  // Breathing effect
    ofSetColor(255, 200, 150, 200);
    ofDrawSphere(0, 0, 0, pulse);

    // Chakra-colored concentric layers
    std::vector<ofColor> chakraColors = {
        ofColor(255, 0, 0),     // Red
        ofColor(255, 165, 0),   // Orange
        ofColor(255, 255, 0),   // Yellow
        ofColor(0, 255, 0),     // Green
        ofColor(0, 0, 255),     // Blue
        ofColor(75, 0, 130),    // Indigo
        ofColor(238, 130, 238)  // Violet
    };

    for (int i = 0; i < chakraColors.size(); i++) {
        ofNoFill();
        ofSetColor(chakraColors[i], 150);
        ofDrawIcoSphere(0, 0, 0, 80 + i * 30);
    }

    ofPopMatrix();
}

void ofApp::drawMandala() {
    ofPushMatrix();

    // Spinning mandala
    float time = ofGetElapsedTimef();
    ofRotateDeg(time * 20, 0, 1, 0);  // Rotate over time
    ofSetColor(150, 200, 255, 100);

    for (int i = 0; i < 12; i++) {
        float angle = ofDegToRad(i * 30);
        float x = cos(angle) * 200;
        float z = sin(angle) * 200;
        ofDrawSphere(x, 0, z, 10);  // Place spheres in a circular pattern
    }

    ofPopMatrix();
}

void ofApp::drawCosmicParticles() {
    for (size_t i = 0; i < particlePositions.size(); i++) {
        float brightness = ofMap(sin(ofGetElapsedTimef() + i), -1, 1, 150, 255);
        ofSetColor(ofColor::fromHsb((int)(i * 10) % 255, 200, brightness));
        ofDrawSphere(particlePositions[i], 2);  // Glowing energy particles
    }
}
