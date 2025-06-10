#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080); // Vertical format
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);

    font.load("georgia.ttf", 28, true, true, true);

    // Repeat text to increase length
    string base = "Every letter floats like a memory. "
                  "Black and white, silence and breath. "
                  "This spiral is a poem in space. ";
    for (int i = 0; i < 10; i++) {
        message += base; // repeat 10 times
    }

    time = 0.0f;
    cam.setDistance(700);
}

void ofApp::update(){
    time += ofGetLastFrameTime();
}

void ofApp::draw(){
    ofBackground(0);
    cam.begin();

    float angleStep = 0.18;     // tighter spiral turn
    float radiusStep = 3.5;     // tighter character spacing

    for (int i = 0; i < message.size(); i++) {
        float angle = time * 0.1 + i * angleStep;
        float radius = i * radiusStep;
        float floatY = sin(time * 1.4 + i * 0.05) * 22 - i * 4.8;

        float x = cos(angle) * radius;
        float z = sin(angle) * radius;
        float y = floatY;

        ofPushMatrix();
        ofTranslate(x, y, z);
        ofRotateDeg(-ofRadToDeg(angle), 0, 1, 0);

        int alpha = ofMap(i, 0, message.size(), 255, 50); // fade at outer edge
        ofSetColor(255, alpha);

        font.drawString(ofToString(message[i]), -font.stringWidth("W") / 2, 0);
        ofPopMatrix();
    }

    cam.end();
}
