
// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0); // Set background to black
    ofSetFrameRate(60);
    ofSetLineWidth(2);

    time = 0.0;
    numPoints = 800; // Increase number of points for denser visuals
    sacredRatio = 1.618; // Golden ratio for harmonious aesthetics
    animationSpeed = 0.01; // Control animation speed

    // Initialize points, radii, colors, rotations, and pulse speeds with random values
    for (int i = 0; i < numPoints; i++) {
        float x = ofRandomWidth();
        float y = ofRandomHeight();
        points.push_back(ofVec2f(x, y));

        float radius = ofRandom(2, 5) * sacredRatio;
        radii.push_back(radius);

        float brightness = ofRandom(150, 255);
        colors.push_back(ofColor(brightness)); // Shades of white for variety

        float rotation = ofRandom(0, 360);
        rotations.push_back(rotation);

        float pulseSpeed = ofRandom(0.01, 0.03);
        pulseSpeeds.push_back(pulseSpeed);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    time += animationSpeed; // Increment time for Perlin noise
    for (int i = 0; i < rotations.size(); i++) {
        rotations[i] += animationSpeed * 100; // Gradual rotation
        radii[i] += sin(time * pulseSpeeds[i]) * 0.5; // Dynamic pulsing effect
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    for (int i = 0; i < points.size(); i++) {
        float noiseX = ofNoise(points[i].x * 0.005 * sacredRatio, points[i].y * 0.005, time);
        float noiseY = ofNoise(points[i].y * 0.005 * sacredRatio, points[i].x * 0.005, time);

        float offsetX = ofMap(noiseX, 0, 1, -20, 20);
        float offsetY = ofMap(noiseY, 0, 1, -20, 20);

        ofPushMatrix();
        ofTranslate(points[i].x + offsetX, points[i].y + offsetY);
        ofRotateDeg(rotations[i]);

        ofSetColor(colors[i]); // Set individual point color
        ofDrawCircle(0, 0, radii[i]);
        ofPopMatrix();

        // Update points slightly to create motion
        points[i].x += offsetX * 0.1;
        points[i].y += offsetY * 0.1;

        // Wrap around screen edges
        if (points[i].x < 0) points[i].x += ofGetWidth();
        if (points[i].x > ofGetWidth()) points[i].x -= ofGetWidth();
        if (points[i].y < 0) points[i].y += ofGetHeight();
        if (points[i].y > ofGetHeight()) points[i].y -= ofGetHeight();
    }

    // Draw additional connecting lines for a more spiritual aesthetic
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            float distance = points[i].distance(points[j]);
            if (distance < 100) { // Draw lines for close points
                float alpha = ofMap(distance, 0, 100, 255, 0);
                ofSetColor(255, alpha); // White lines with transparency

                // Use golden angle for spiral-like connections
                float angle = sacredRatio * distance;
                float x1 = points[i].x + cos(angle) * 5;
                float y1 = points[i].y + sin(angle) * 5;
                float x2 = points[j].x + cos(angle) * 5;
                float y2 = points[j].y + sin(angle) * 5;

                ofDrawLine(x1, y1, x2, y2);
            }
        }
    }

    // Add concentric circles for a meditative feel
    for (int i = 0; i < 5; i++) {
        float radius = ofGetWidth() / 8.0 * (i + 1) * sacredRatio;
        ofNoFill();
        ofSetColor(255, 50); // Transparent white
        ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, radius);
    }

    // Add rotating geometric patterns for enhanced aesthetics
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    for (int i = 0; i < 8; i++) {
        ofRotateDeg(i * 45 + time * 50); // Rotating star-like pattern
        ofSetColor(255, 100);
        ofDrawLine(0, 0, ofGetWidth() / 6, 0);
    }
    ofPopMatrix();

    // Add dynamic radiant lines for a burst effect
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    for (int i = 0; i < 16; i++) {
        float angle = i * 360 / 16 + time * 30;
        float length = ofGetWidth() / 4 + sin(time * sacredRatio) * 50;
        ofSetColor(255, 75);
        ofDrawLine(0, 0, cos(ofDegToRad(angle)) * length, sin(ofDegToRad(angle)) * length);
    }
    ofPopMatrix();
}
