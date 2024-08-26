#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    
    numPoints = 300; // Number of particles
    maxRadius = 10;  // Particle size
    rotationSpeed = 0.3;
    noiseOffset = ofPoint(ofRandom(1000), ofRandom(1000)); // Initialize noise offset

    gradientStart = ofColor(20, 20, 60);  // Brighter gradient
    gradientEnd = ofColor(5, 5, 30);

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA); // Set up FBO

    for (int i = 0; i < numPoints; i++) {
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        points.push_back(ofPoint(x, y));
        velocities.push_back(ofPoint(ofRandom(-1, 1), ofRandom(-1, 1)));
        colors.push_back(generateRandomColor());
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < numPoints; i++) {
        // Apply Perlin noise to velocity
        float noiseX = ofNoise(points[i].x * 0.005 + noiseOffset.x, ofGetElapsedTimef() * 0.1);
        float noiseY = ofNoise(points[i].y * 0.005 + noiseOffset.y, ofGetElapsedTimef() * 0.1);
        velocities[i].x = ofMap(noiseX, 0, 1, -1, 1);
        velocities[i].y = ofMap(noiseY, 0, 1, -1, 1);

        // Interaction with mouse
        ofPoint mousePos(ofGetMouseX(), ofGetMouseY());
        ofPoint dir = points[i] - mousePos;
        float dist = dir.length();

        if (attractMode && dist < 300) {
            dir.normalize();
            points[i] -= dir * attractStrength * (300 - dist) / 300.0;
        } else if (!attractMode && dist < 100) {
            dir.normalize();
            points[i] += dir * repelStrength * (100 - dist) / 100.0;
        }

        // Update position
        points[i] += velocities[i];

        // Wrap around edges
        if (points[i].x < 0) points[i].x += ofGetWidth();
        if (points[i].x > ofGetWidth()) points[i].x -= ofGetWidth();
        if (points[i].y < 0) points[i].y += ofGetHeight();
        if (points[i].y > ofGetHeight()) points[i].y -= ofGetHeight();
    }

    // Slowly move noise offset for evolving patterns
    noiseOffset += ofPoint(0.01, 0.01);
}

//--------------------------------------------------------------
void ofApp::draw() {
    fbo.begin();
    ofClear(0, 0, 0, 0); // Clear the FBO

    drawGradientBackground();

    ofEnableBlendMode(OF_BLENDMODE_ADD);

    drawSymmetry(8); // 8-fold symmetry for even more complexity

    ofDisableBlendMode();
    fbo.end();

    applyPostProcessing(); // Apply blur and vignette effects
    fbo.draw(0, 0); // Draw the final output to the screen
}

//--------------------------------------------------------------
ofColor ofApp::generateRandomColor() {
    // Generates a random color with high saturation and brightness
    return ofColor(ofRandom(150, 255), ofRandom(100, 200), ofRandom(200, 255));
}

//--------------------------------------------------------------
void ofApp::drawGradientBackground() {
    // Draws a vertical gradient background
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    // Top-left corner
    mesh.addVertex(ofPoint(0, 0));
    mesh.addColor(gradientStart);
    
    // Top-right corner
    mesh.addVertex(ofPoint(ofGetWidth(), 0));
    mesh.addColor(gradientStart);
    
    // Bottom-right corner
    mesh.addVertex(ofPoint(ofGetWidth(), ofGetHeight()));
    mesh.addColor(gradientEnd);
    
    // Bottom-left corner
    mesh.addVertex(ofPoint(0, ofGetHeight()));
    mesh.addColor(gradientEnd);

    // Draw the mesh to create the gradient
    mesh.draw();
}

//--------------------------------------------------------------
void ofApp::drawSymmetry(int numSymmetry) {
    float angleStep = 360.0 / numSymmetry;
    for (int i = 0; i < numSymmetry; i++) {
        ofPushMatrix();
        ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
        ofRotateDeg(i * angleStep + ofGetElapsedTimef() * rotationSpeed);
        ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);

        for (int j = 0; j < numPoints; j++) {
            ofSetColor(colors[j], 200);  // Increase alpha for more visibility
            float radius = ofMap(sin(ofGetElapsedTimef() + j), -1, 1, 5, maxRadius);
            drawParticleTrail(points[j], colors[j]);
            ofDrawCircle(points[j], radius);
        }

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawParticleTrail(ofPoint point, ofColor color) {
    for (int i = 0; i < 15; i++) {
        float alpha = ofMap(i, 0, 15, 100, 0);  // Increase alpha for brighter trails
        ofSetColor(color, alpha);
        ofDrawCircle(point.x - velocities[i].x * i * 2, point.y - velocities[i].y * i * 2, maxRadius * 0.5 - i * 0.3);
    }
}

//--------------------------------------------------------------
void ofApp::applyPostProcessing() {
    fbo.begin();
    
    // Reduce blur intensity
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255, 255, 255, 50); // Reduce the opacity to decrease blur
    for (int i = 0; i < 2; i++) {  // Fewer blur layers
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }

    ofSetColor(0, 0, 0, 50); // Semi-transparent black for vignette
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    fbo.end();
}

//--------------------------------------------------------------
void ofApp::toggleAttractMode() {
    attractMode = !attractMode; // Toggle between attraction and repulsion
}
