#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(120);

    time = 0.0;

    chakraColors = {
        ofColor(255, 100, 100),
        ofColor(255, 180, 100),
        ofColor(255, 255, 120),
        ofColor(120, 255, 140),
        ofColor(120, 180, 255),
        ofColor(180, 120, 255),
        ofColor(255, 120, 200)
    };

    // Star field for ambiance
    starField.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < 1000; i++) {
        float x = ofRandom(-1000, 1000);
        float y = ofRandom(-1000, 1000);
        float z = ofRandom(-1000, 1000);
        starField.addVertex(glm::vec3(x, y, z));
        starField.addColor(ofColor(255, 255, 255, ofRandom(10, 60)));
    }
    glPointSize(1.2);
}

void ofApp::update(){
    time += 0.015; // slightly faster animation
}

void ofApp::draw(){
    cam.begin();

    // Draw starfield background
    ofSetColor(255);
    starField.draw();

    int layers = 16;
    int petals = 40;
    float baseRadius = 40;
    float breathing = sin(time * 0.6) * 15 + 20;

    for (int j = 0; j < layers; j++) {
        ofPushMatrix();

        float goldenAngle = 137.5;
        float spiralRot = time * 10 + j * goldenAngle;
        float layerScale = 1.0 + 0.1 * sin(time * 0.8 + j); // pulse scale

        ofRotateZDeg(spiralRot);
        ofScale(layerScale, layerScale, 1.0);
        ofTranslate(0, 0, sin(time * 0.4 + j * 0.2) * 30); // float in Z

        ofColor colA = chakraColors[j % chakraColors.size()];
        ofColor colB = chakraColors[(j + 1) % chakraColors.size()];

        for (int i = 0; i < petals; i++) {
            float angle = ofDegToRad((360.0 / petals) * i);
            float radius = baseRadius + j * 25 + breathing;

            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            float z = cos(time * 1.1 + i * 0.15 + j) * 10;

            float t = float(i) / petals;
            ofColor c = colA.getLerped(colB, t);
            c.a = 150 + 100 * sin(time + i * 0.2); // soft fade

            ofSetColor(c);
            ofDrawCircle(x, y, z, 3 + 2 * sin(time * 0.5 + i * 0.3));
        }

        ofPopMatrix();
    }

    cam.end();
}
