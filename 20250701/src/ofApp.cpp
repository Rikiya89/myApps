#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    // Generate points in 3D spiral (sacred geometry-inspired)
    float goldenAngle = TWO_PI * (3 - sqrt(5)); // ≈ 137.5°
    for (int i = 0; i < numPoints; i++) {
        float t = i / (float)numPoints;
        float radius = 300 * sqrt(t);
        float theta = i * goldenAngle;

        float x = radius * cos(theta);
        float y = ofMap(i, 0, numPoints, -300, 300);
        float z = radius * sin(theta);

        points.push_back(ofVec3f(x, y, z));
    }
}

void ofApp::update(){
    time = ofGetElapsedTimef();
}

void ofApp::draw(){
    cam.begin();
    drawPointCloud();
    cam.end();
}

void ofApp::drawPointCloud(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    for (int i = 0; i < points.size(); i++) {
        ofVec3f p = points[i];

        // Add Perlin noise float
        float scaleNoise = ofNoise(p.x * 0.005, p.y * 0.005, p.z * 0.005, time * 0.2);
        float scale = 1.0 + 0.4 * sin(scaleNoise * TWO_PI + time);

        ofVec3f animated = p * scale;

        // Dynamic hue
        float hue = fmod((animated.length() * 0.3 + time * 20), 255);
        ofColor c = ofColor::fromHsb(hue, 200, 255, 180);
        ofSetColor(c);

        ofDrawSphere(animated, 1.8);
    }

    ofDisableBlendMode();
}
