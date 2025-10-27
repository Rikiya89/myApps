#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetCircleResolution(240);
    time = 0.0f;
}

void ofApp::update(){
    time += 0.015;
}

void ofApp::draw(){
    ofBackground(0, 0, 0, 10); // persistent trail effect
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.begin();

    int rings = 48;
    int segments = 220;
    float goldenAngle = TWO_PI * (3 - sqrt(5)); // ≈ 137.5°

    for(int r = 0; r < rings; r++){
        float radius = 80 + r * 9;
        float breathing = sin(time * 1.3 + r * 0.3) * 6.5;
        float twist = goldenAngle * r;

        ofSetLineWidth(1.2);
        ofBeginShape();

        for(int i = 0; i <= segments; i++){
            float t = ofMap(i, 0, segments, 0, TWO_PI);
            float n = ofNoise(i * 0.02, r * 0.05, time) * 30;

            float angle = t + twist;
            float spiralRadius = radius + breathing + n;

            float x = spiralRadius * cos(angle);
            float y = spiralRadius * sin(angle);
            float z = sin(t * 3.0 + time * 2.0 + r) * 60;

            // Aurora-style hue shift
            float hue = fmod(ofMap(i, 0, segments, 180, 360) + time * 30, 255);
            ofColor col = ofColor::fromHsb(hue, 200, 255, 180);
            ofSetColor(col);

            ofVertex(x, y, z);
        }

        ofEndShape(true);
    }

    cam.end();
    ofDisableBlendMode();
}
