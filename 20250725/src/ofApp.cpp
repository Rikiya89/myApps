#include "ofApp.h"

void ofApp::setup() {
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);

    phi = (1 + sqrtf(5.0f)) / 2.0f;
    tau = TWO_PI;
    goldenAngle = tau * (1 - 1 / phi);

    numPoints = 3000;
    baseScale = 5.0f;
    center.set(ofGetWidth() / 2, ofGetHeight() / 2);
    rotationSpeed = 0.2f;
    breathingSpeed = 0.5f;
    noiseScale = 0.005f;
}

void ofApp::update() {
    // no state updates needed here
}

void ofApp::draw() {
    float time = ofGetElapsedTimef();

    // softer fade trails
    ofSetColor(0, 0, 0, 20);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // switch to alpha blending for subtler glow
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    float rot = time * rotationSpeed;
    float dynamicScale = baseScale + sin(time * breathingSpeed) * 2.0f;

    ofPolyline spiral;

    for (int i = 0; i < numPoints; i++) {
        float angle = i * goldenAngle + rot;
        float radius = dynamicScale * sqrtf(i);
        radius += (ofNoise(i * noiseScale, time * 0.1f) - 0.5f) * 15.0f;  // slightly reduced noise amplitude

        float x = center.x + radius * cosf(angle);
        float y = center.y + radius * sinf(angle);

        float n = ofNoise(i * noiseScale * 2.0f, time * 0.2f);
        float size = ofMap(n, 0, 1, 1, 5); // slightly smaller max size

        // toned-down brightness
        float bright = ofMap(sin(i * 0.05f + time), -1, 1, 50, 150);
        ofSetColor(bright);
        ofDrawCircle(x, y, size);
        spiral.addVertex(x, y);
    }

    // faint connecting curve
    ofSetColor(255, 30);
    spiral.draw();

    ofDisableBlendMode();
}
