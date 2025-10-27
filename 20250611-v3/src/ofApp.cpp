#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofSetCircleResolution(120);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    baseRadius = 50;
    spacing = baseRadius * 1.4;
    rings = 8;     // concentric rings
    sectors = 36;  // like mandala slices

    time = 0.0;
}

void ofApp::update(){
    time += ofGetLastFrameTime();
}

void ofApp::draw(){
    ofBackground(0, 12);  // Black with trail
    cam.begin();

    cam.orbitDeg(time * 4, sin(time * 0.3) * 18 + 30, 680 + sin(time * 0.5) * 40, {0, 0, 0});

    ofNoFill();
    ofSetLineWidth(1.3);

    float breathing = sin(time * 2.0) * 5.0;
    float glowAlpha = 140 + 90 * sin(time * 1.5);

    for (int r = 1; r <= rings; r++) {
        float radius = spacing * r;

        for (int i = 0; i < sectors; i++) {
            float angleDeg = i * (360.0 / sectors) + time * 3.0;
            float angleRad = ofDegToRad(angleDeg);
            float x = cos(angleRad) * radius;
            float y = sin(angleRad) * radius;
            float z = sin(angleRad * 2 + time * 0.6) * 50;

            float size = baseRadius + breathing;

            float dist = ofVec3f(x, y, z).length();
            float fade = ofMap(dist, 0, rings * spacing, 255, 20, true);
            float finalAlpha = ofClamp(glowAlpha * (fade / 255.0), 0, 255);

            ofPushMatrix();
            ofTranslate(x, y, z);
            ofRotateDeg(angleDeg + time * 1.5);
            ofSetColor(255, finalAlpha);
            ofDrawCircle(0, 0, size);
            ofPopMatrix();
        }
    }

    cam.end();
}
