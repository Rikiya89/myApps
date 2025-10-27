#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);

    font.load("georgia.ttf", 26, true, true, true);

    // Add long poetic string
    string base = "Letters become breath. Thoughts drift like light. ";
    for (int i = 0; i < 12; i++) {
        message += base;
    }

    time = 0.0f;

    // Enable mouse/touch camera control
    cam.setAutoDistance(false);
    cam.setDistance(720); // starting distance
    cam.enableMouseInput(); // allow full user control
}

void ofApp::update(){
    time += ofGetLastFrameTime();
}

void ofApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofBackground(0);

    cam.begin(); // now fully user-controlled

    float angleStep = 0.18;
    float radiusStep = 3.6;

    for (int i = 0; i < message.size(); i++) {
        float angle = time * 0.07 + i * angleStep;
        float radius = i * radiusStep;
        float floatY = sin(time * 1.4 + i * 0.05) * 22 - i * 4.6;
        float scale = 1.0 + 0.08 * sin(time * 2.0 + i * 0.15);

        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        float y = floatY;

        ofPushMatrix();
        ofTranslate(x, y, z);
        ofRotateDeg(-ofRadToDeg(angle), 0, 1, 0);
        ofScale(scale, scale);

        int alpha = ofMap(i, 0, message.size(), 240, 50);
        ofSetColor(255, alpha);
        font.drawString(ofToString(message[i]), -font.stringWidth("W") / 2, 0);
        ofPopMatrix();
    }

    cam.end();
    ofDisableBlendMode();
}
