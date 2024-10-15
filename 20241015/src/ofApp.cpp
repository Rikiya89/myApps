#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0); // Black background
    box2d.init();
    box2d.setGravity(0, gravity); // Gravity pulls downward
    box2d.createBounds(); // Create world boundaries
    box2d.setFPS(60.0); // Run simulation at 60 frames per second

    int numParticles = 100;
    for (int i = 0; i < numParticles; i++) {
        // Create particle with random position and size
        auto circle = make_shared<ofxBox2dCircle>();
        circle->setPhysics(1.0, 0.5, 0.1); // density, bounce, friction
        circle->setup(box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(3, 8));
        particles.push_back(circle);
        
        // Initialize noise offset for each particle
        noiseOffsets.push_back(ofVec2f(ofRandom(0, 1000), ofRandom(0, 1000)));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update(); // Update physics world
    
    ofVec2f attractor(ofGetWidth() / 2, ofGetHeight() / 2); // Center point for attraction
    for (int i = 0; i < particles.size(); i++) {
        // Generate Perlin noise force
        float noiseX = ofNoise(noiseOffsets[i].x) * 2 - 1;
        float noiseY = ofNoise(noiseOffsets[i].y) * 2 - 1;
        
        ofVec2f noiseForce(noiseX * 0.5, noiseY * 0.5); // Scale Perlin noise force
        
        // Calculate force towards the attractor and apply it
        ofVec2f force = attractor - particles[i]->getPosition();
        force.normalize();
        particles[i]->addForce(force * 0.05 + noiseForce, 1.0); // Apply attractor and noise forces
        
        // Update Perlin noise offsets for smooth motion
        noiseOffsets[i].x += 0.01;
        noiseOffsets[i].y += 0.01;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Transparent overlay to create trails effect
    ofSetColor(0, 0, 0, 20);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // Draw each particle with varying colors and transparency
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(ofMap(i, 0, particles.size(), 100, 255), ofMap(i, 0, particles.size(), 50, 200), 255, 150);
        particles[i]->draw(); // Draw particle
    }
}
