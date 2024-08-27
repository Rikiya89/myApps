#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    
    numPoints = 300; // Number of particles
    maxRadius = 10;  // Particle size
    rotationSpeed = 0.2;  // Slower rotation for smoother visual
    noiseOffset = ofPoint(ofRandom(1000), ofRandom(1000)); // Initialize noise offset

    gradientStart = ofColor(20, 20, 60);  // Brighter gradient
    gradientEnd = ofColor(5, 5, 30);

    colorShiftSpeed = 0.005;  // Slower color shift for smoother transitions
    baseColor = ofColor(255, 100, 200);  // Base color for color cycling

    attractStrength = 0.05;  // Lowered attract strength for smoother motion
    repelStrength = 0.1;     // Lowered repel strength for smoother motion

    for (int i = 0; i < numPoints; i++) {
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        points.push_back(ofPoint(x, y));
        velocities.push_back(ofPoint(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5)));  // Smoother initial velocity
        colors.push_back(generateRandomColor());
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    // Update base color for color cycling
    baseColor.r = 127 + 127 * sin(ofGetElapsedTimef() * colorShiftSpeed);
    baseColor.g = 127 + 127 * sin(ofGetElapsedTimef() * colorShiftSpeed + PI / 2);
    baseColor.b = 127 + 127 * sin(ofGetElapsedTimef() * colorShiftSpeed + PI);

    for (int i = 0; i < numPoints; i++) {
        // Apply smoother Perlin noise to velocity
        float noiseX = ofNoise(points[i].x * 0.002 + noiseOffset.x, ofGetElapsedTimef() * 0.05);
        float noiseY = ofNoise(points[i].y * 0.002 + noiseOffset.y, ofGetElapsedTimef() * 0.05);
        velocities[i].x = ofLerp(velocities[i].x, ofMap(noiseX, 0, 1, -0.5, 0.5), 0.1);  // Smooth interpolation
        velocities[i].y = ofLerp(velocities[i].y, ofMap(noiseY, 0, 1, -0.5, 0.5), 0.1);  // Smooth interpolation

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
    noiseOffset += ofPoint(0.005, 0.005);
}

//--------------------------------------------------------------
void ofApp::draw() {
    drawGradientBackground();

    ofEnableBlendMode(OF_BLENDMODE_ADD);

    drawSymmetry(8); // 8-fold symmetry for even more complexity

    ofDisableBlendMode();
}

//--------------------------------------------------------------
ofColor ofApp::generateRandomColor() {
    // Generates a color based on the baseColor with some randomness
    return baseColor.getLerped(ofColor::white, ofRandom(0.0, 0.3));  // Less variation for smoother transitions
}

//--------------------------------------------------------------
void ofApp::drawGradientBackground() {
    // Animate the gradient background for more visual interest
    float t = ofGetElapsedTimef();
    gradientStart = ofColor(20 + 20 * sin(t * 0.05), 20 + 20 * cos(t * 0.07), 60 + 20 * sin(t * 0.05));
    gradientEnd = ofColor(5 + 5 * sin(t * 0.1), 5 + 5 * cos(t * 0.12), 30 + 10 * sin(t * 0.1));

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
void ofApp::toggleAttractMode() {
    attractMode = !attractMode; // Toggle between attraction and repulsion
}
