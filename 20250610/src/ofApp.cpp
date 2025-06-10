#include "ofApp.h"

void ofApp::setup(){
    // Window and rendering setup
    ofSetWindowShape(720, 1080);
    ofSetFrameRate(60);
    ofBackground(5, 5, 15); // Deep navy background
    ofEnableAlphaBlending(); // Allow transparency
    ofEnableSmoothing();     // Anti-aliasing for smooth curves
    ofSetCircleResolution(100); // Smooth curve precision
    ofSetLineWidth(1.2);
    ofEnableBlendMode(OF_BLENDMODE_ADD); // Glow effect when lines overlap

    // Grid spacing and dimensions
    spacing = 26.0;
    cols = ofGetWidth() / spacing;
    rows = ofGetHeight() / spacing;
    time = 0.0;

    phi = 1.618; // Golden ratio for elegant curves

    // Mouse effect radius
    influenceRadius = 140.0;
    mousePos.set(ofGetWidth() / 2, ofGetHeight() / 2); // Start at center
}

void ofApp::update(){
    time += 0.007; // Advance animation
}

void ofApp::draw(){
    // Transparent rectangle for motion trail effect (like fading previous frames)
    ofSetColor(5, 5, 15, 18);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // Loop through each grid point
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            float nx = x * 0.05;
            float ny = y * 0.05;

            float cx = x * spacing;
            float cy = y * spacing;
            ofVec2f pos(cx, cy);

            // Calculate base angle using Perlin noise
            float baseAngle = ofNoise(nx, ny, time) * TWO_PI * 2.0;

            // Add mouse repulsion effect
            float dist = pos.distance(mousePos);
            if (dist < influenceRadius) {
                float pullAngle = atan2(pos.y - mousePos.y, pos.x - mousePos.x);
                float strength = ofMap(dist, 0, influenceRadius, 1.0, 0.0); // stronger closer
                baseAngle = baseAngle * (1.0 - strength) + pullAngle * strength;
            }

            // Vector lengths based on angle
            float len = spacing * 0.6;
            float x1 = cos(baseAngle) * len;
            float y1 = sin(baseAngle) * len;
            float x2 = cos(baseAngle + phi * 0.5) * len * 0.8;
            float y2 = sin(baseAngle + phi * 0.5) * len * 0.8;

            // Color generation using HSB
            float hue = fmod(ofMap(baseAngle, 0, TWO_PI * 2.0, 160, 255) + time * 40, 255);
            float brightness = ofMap(dist, 0, influenceRadius * 1.5, 255, 100, true);
            ofColor col1 = ofColor::fromHsb(hue, 120, brightness, 200);    // inner glow
            ofColor col2 = ofColor::fromHsb((int)hue + 30 % 255, 80, 220, 100); // outer aura

            // Draw flowing petal-like curve: first layer
            ofPushMatrix();
            ofTranslate(cx, cy);

            ofPath path1;
            path1.setStrokeColor(col1);
            path1.setStrokeWidth(1.4);
            path1.setFilled(false);
            path1.moveTo(0, 0);
            path1.bezierTo(x1 * 0.5, -y1 * 0.5, x2 * 0.7, y2 * 0.5, x2, y2);
            path1.draw();

            // Mirror shape for layered glow: second layer
            ofPath path2;
            path2.setStrokeColor(col2);
            path2.setStrokeWidth(0.8);
            path2.setFilled(false);
            path2.moveTo(0, 0);
            path2.bezierTo(-x1 * 0.5, y1 * 0.5, -x2 * 0.5, -y2 * 0.5, -x2, -y2);
            path2.draw();

            ofPopMatrix();
        }
    }
}

void ofApp::mouseMoved(int x, int y){
    // Store current mouse position for interaction
    mousePos.set(x, y);
}
