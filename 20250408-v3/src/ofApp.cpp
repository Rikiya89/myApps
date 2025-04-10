#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD); // ✨ glowing blend
    ofSetCircleResolution(100);

    for (int i = 0; i < numPoints; i++) {
        float angle = ofDegToRad(i * angleStep);
        float radius = spiralScale * sqrt(i);
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        float z = 0;
        points.push_back(glm::vec3(x, y, z));
    }
}

void ofApp::update() {
    time = ofGetElapsedTimef();

    float breathing = 1.0 + 0.1 * sin(time * 0.5); // spiral breathing effect

    for (int i = 0; i < points.size(); i++) {
        float angle = ofDegToRad(i * angleStep);
        float radius = breathing * spiralScale * sqrt(i);

        float x = radius * cos(angle);
        float y = radius * sin(angle);
        float z = sin(time * 0.7 + i * 0.02) * 100 + cos(time * 0.4 + i * 0.01) * 20;

        points[i] = glm::vec3(x, y, z);
    }
}

void ofApp::draw() {
    ofBackground(0);

    cam.begin();
    ofPushMatrix();
    ofTranslate(0, 0, -300);
    ofRotateXDeg(sin(time * 0.2) * 10);
    ofRotateYDeg(time * 8);
    ofRotateZDeg(cos(time * 0.3) * 5);

    // 🌟 Sacred center orb
    float centerGlow = ofMap(sin(time), -1, 1, 100, 255);
    ofSetColor(255, centerGlow);
    ofDrawSphere(0, 0, 0, 10);

    // 🌌 Lines connecting particles
    for (int i = 0; i < points.size(); i += 10) {
        for (int j = i + 1; j < i + 5 && j < points.size(); j++) {
            float dist = glm::distance(points[i], points[j]);
            if (dist < 50) {
                float alpha = ofMap(dist, 0, 50, 100, 0);
                ofSetColor(255, alpha);
                ofDrawLine(points[i], points[j]);
            }
        }
    }

    // ✨ Sparkling spiral particles
    for (int i = 0; i < points.size(); i++) {
        float sparkle = sin(time * 2.0 + i * 0.05);
        float alpha = ofMap(sparkle, -1, 1, 50, 255);
        float size = ofMap(sin(time * 1.5 + i * 0.01), -1, 1, 1.0, 3.0);
        ofSetColor(255, alpha);
        ofDrawSphere(points[i], size);
    }

    ofPopMatrix();
    cam.end();
}
