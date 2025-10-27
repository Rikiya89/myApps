#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(10, 10, 30); // Set the background to a deep, calming blue

    numLayers = 15; // Number of layers for radial symmetry
    baseRadius = 25.0; // Base radius for the smallest shape
    numShapes = 12; // Number of shapes per layer for intricate symmetry
    rotationSpeed = 0.02; // Slow rotation speed for a calming effect
    globalRotation = 0; // Initialize global rotation angle

    for (int i = 0; i < numLayers; i++) {
        float radius = baseRadius * (i + 1);
        radii.push_back(radius);

        // Generate a smooth gradient of soft, spiritual colors
        ofColor color = ofColor::fromHsb(ofMap(i, 0, numLayers - 1, 160, 220), 180, 220);
        colors.push_back(color);

        // Set a gentle speed for pulsating animation
        float speed = ofRandom(0.02, 0.05);
        speeds.push_back(speed);

        // Create random offsets for each layer to vary their movements
        offsets.push_back(ofRandom(0, TWO_PI));
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < numLayers; i++) {
        // Update radii to create a gentle pulsating effect
        radii[i] += sin(ofGetElapsedTimef() * speeds[i] + offsets[i]) * 0.5;
    }

    // Update global rotation for a dynamic overall effect
    globalRotation += rotationSpeed;
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotateDeg(globalRotation); // Apply global rotation to the entire scene

    for (int i = 0; i < numLayers; i++) {
        ofSetColor(colors[i], 100); // Set the color for each layer with some transparency

        ofPushMatrix();
        ofRotateDeg(ofGetElapsedTimef() * rotationSpeed * (i + 1)); // Apply slow, layered rotation

        // Draw intricate radial symmetry with more complex shapes
        for (int j = 0; j < numShapes; j++) {
            ofRotateDeg(360.0 / numShapes); // Rotate for equal spacing

            // Create an intricate pattern using bezier curves and circles
            ofBeginShape();
            ofVertex(0, 0);
            ofBezierVertex(radii[i] * 0.3, radii[i] * 0.6, -radii[i] * 0.3, radii[i] * 0.8, 0, radii[i] * 0.5);
            ofBezierVertex(radii[i] * 0.3, -radii[i] * 0.8, -radii[i] * 0.3, -radii[i] * 0.6, 0, 0);
            ofEndShape();

            ofDrawCircle(0, -radii[i] * 0.4, radii[i] * 0.1); // Add small circles for more detail
        }

        ofPopMatrix();
    }
}
