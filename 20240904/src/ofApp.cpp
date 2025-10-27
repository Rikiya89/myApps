#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    numRows = 15;
    numCols = 15;
    baseRadius = 36.0f;
    waveSpeed = 0.9f;
    waveFrequency = 0.3f;
    rotationSpeed = 0.6f;
    trailFadeSpeed = 6.0f;

    // Initialize particle system with colors
    for(int i = 0; i < 100; i++) {
        particles.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
        velocities.push_back(ofVec2f(ofRandom(-2, 2), ofRandom(-2, 2)));
        particleColors.push_back(ofColor::white);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimef();

    // Update particle positions and colors
    for(int i = 0; i < particles.size(); i++) {
        particles[i] += velocities[i];
        velocities[i] *= 0.99;  // Dampen velocity for smoother motion

        // Boundaries
        if(particles[i].x < 0 || particles[i].x > ofGetWidth()) velocities[i].x *= -1;
        if(particles[i].y < 0 || particles[i].y > ofGetHeight()) velocities[i].y *= -1;

        // Update particle colors based on position and time
        float hue = ofMap(particles[i].x, 0, ofGetWidth(), 0, 255);
        particleColors[i].setHsb(hue, 200, 255);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw dynamic gradient background
    ofBackgroundGradient(getBackgroundColor(), ofColor(0, 0, 0), OF_GRADIENT_LINEAR);

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);  // Center the drawing
    ofRotateDeg(time * rotationSpeed * 50);  // Apply subtle rotation for dynamic movement

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            float x = (col - numCols / 2) * baseRadius * 2.5;
            float y = (row - numRows / 2) * baseRadius * 2.5;

            // Use Perlin noise to influence the radius and position dynamically
            float noiseFactor = ofNoise(row * 0.1, col * 0.1, time * 0.2);
            float radius = baseRadius + 20 * sin(time * waveSpeed + row * waveFrequency + col * waveFrequency) * noiseFactor;

            drawPattern(x, y, radius, 4);  // Deep recursion for complexity
        }
    }

    ofPopMatrix();
    drawParticles();  // Draw particles with trails and glow
}

//--------------------------------------------------------------
void ofApp::drawPattern(float x, float y, float radius, int depth) {
    if (depth == 0) return;

    float t = ofMap(radius, baseRadius - 20, baseRadius + 20, 0, 1);
    ofSetColor(getGradientColor(t, depth), 200);  // Dynamic gradient color based on depth
    ofNoFill();
    ofSetLineWidth(2 + depth);  // Thicker lines for deeper levels

    // Alternate between circles and polygons
    if (depth % 2 == 0) {
        ofDrawCircle(x, y, radius);
    } else {
        int sides = 3 + depth * 2;  // Increase number of sides with depth
        ofBeginShape();
        for(int i = 0; i < sides; i++) {
            float angle = ofDegToRad(360.0 / sides * i);
            ofVertex(x + radius * cos(angle), y + radius * sin(angle));
        }
        ofEndShape(true);
    }

    for (int i = 0; i < 6; ++i) {
        float angle = ofDegToRad(60 * i + time * 40);
        float nx = x + radius * cos(angle);
        float ny = y + radius * sin(angle);

        float smallRadius = radius * 0.5 * ofNoise(nx * 0.02, ny * 0.02, time * 0.2);
        drawPattern(nx, ny, smallRadius, depth - 1);
    }
}

// Function to generate a rich color gradient with variants
ofColor ofApp::getGradientColor(float t, int variant) {
    ofColor colorStart, colorMiddle, colorEnd;

    // Different color palettes based on variant
    switch(variant % 3) {
        case 0:
            colorStart = ofColor::blue;
            colorMiddle = ofColor::cyan;
            colorEnd = ofColor::green;
            break;
        case 1:
            colorStart = ofColor::red;
            colorMiddle = ofColor::orange;
            colorEnd = ofColor::yellow;
            break;
        case 2:
            colorStart = ofColor::purple;
            colorMiddle = ofColor::magenta;
            colorEnd = ofColor::pink;
            break;
    }

    if (t < 0.5) {
        return colorStart.getLerped(colorMiddle, t * 2.0);
    } else {
        return colorMiddle.getLerped(colorEnd, (t - 0.5) * 2.0);
    }
}

// Function to generate a dynamic background color
ofColor ofApp::getBackgroundColor() {
    float hue = ofMap(sin(time * 0.5), -1, 1, 0, 255);
    return ofColor::fromHsb(hue, 100, 50);
}

// Function to draw the particle system with trails and glow
void ofApp::drawParticles() {
    for(int i = 0; i < particles.size(); i++) {
        ofSetColor(particleColors[i], 80);  // Semi-transparent for glow effect
        ofDrawCircle(particles[i], 8);  // Glow

        ofSetColor(particleColors[i]);
        ofDrawCircle(particles[i], 2);  // Particle

        // Draw fading trails
        ofSetColor(particleColors[i], 40);
        for(int j = 1; j < trailFadeSpeed; j++) {
            ofDrawCircle(particles[i] - velocities[i] * j * 0.5, 2 - j * 0.2);  // Smaller fading circles
        }
    }
}
