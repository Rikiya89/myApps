#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Window settings
    ofSetFrameRate(60);
    ofSetCircleResolution(36);

    // Make sure we're using black background in final rendering
    // but we won't call ofBackground(...) directly because we'll
    // draw everything into an FBO.
    ofBackground(0);

    // Allocate FBO for drawing trails
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 255); // fully clear with black
    fbo.end();

    // Particle setup
    numParticles = 2000;
    positions.resize(numParticles);
    velocities.resize(numParticles);

    for(int i=0; i<numParticles; i++){
        positions[i].set(ofRandomWidth(), ofRandomHeight());
        velocities[i].set(ofRandom(-1,1), ofRandom(-1,1));
    }

    // Noise parameters
    noiseScale = 0.002;
    noiseSpeed = 0.2;
    noiseStrength = 1.0;
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0; i<numParticles; i++){
        // Sample Perlin noise to get an angle (0..TWO_PI)
        float time = ofGetElapsedTimef() * noiseSpeed;
        float noiseVal = ofNoise(positions[i].x * noiseScale,
                                 positions[i].y * noiseScale,
                                 time) * TWO_PI;

        // Convert noise value into a 2D direction
        ofVec2f dir(cos(noiseVal), sin(noiseVal));

        // Gently steer velocity towards dir
        velocities[i] += dir * (noiseStrength * 0.01);
        // Add damping
        velocities[i] *= 0.97;

        // Update positions
        positions[i] += velocities[i];

        // Wrap around screen edges
        if(positions[i].x < 0) positions[i].x = ofGetWidth();
        if(positions[i].x > ofGetWidth()) positions[i].x = 0;
        if(positions[i].y < 0) positions[i].y = ofGetHeight();
        if(positions[i].y > ofGetHeight()) positions[i].y = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Begin drawing into the FBO
    fbo.begin();

    // Fade the previous frame slightly
    // The alpha value here controls how quickly trails fade:
    // higher alpha -> faster fade, lower alpha -> slower fade
    ofSetColor(0, 15);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // Draw new particle positions in white
    ofSetColor(255);
    for(int i=0; i<numParticles; i++){
        // draw a small circle
        ofDrawCircle(positions[i], 1.5);
    }

    fbo.end();

    // Now draw the FBO to the screen
    ofSetColor(255);  // full white
    fbo.draw(0, 0);

    // Instructions
    ofDrawBitmapStringHighlight("Press any key to reset particles", 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Reset particles
    for(int i=0; i<numParticles; i++){
        positions[i].set(ofRandomWidth(), ofRandomHeight());
        velocities[i].set(ofRandom(-1,1), ofRandom(-1,1));
    }

    // Clear the FBO
    fbo.begin();
    ofClear(0, 0, 0, 255);
    fbo.end();
}
