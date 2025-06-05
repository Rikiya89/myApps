#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableLighting();

    // Lighting
    light.setup();
    light.setPosition(300, 300, 600);
    light.enable();
    ambient.setAmbientColor(ofFloatColor(0.3));
    ambient.enable();

    cam.setDistance(600);
    time = 0.0;
}

void ofApp::update(){
    time += ofGetLastFrameTime();
}

void ofApp::draw(){
    // Auto-rotate camera for sculptural view
    cam.orbitDeg(time * 8, sin(time * 0.3) * 20, cam.getDistance(), {0, 0, 0});
    cam.begin();

    int total = 320;
    float baseRadius = 160 + sin(time * 0.5) * 20;
    float height = 600;
    float noiseScale = 0.07;

    for (int i = 0; i < total; i++) {
        float t = (float)i / total;
        float angle = t * TWO_PI * 8 + time * 0.3;
        float localRadius = baseRadius + sin(t * PI * 10 + time * 2.0) * 15;

        float x = localRadius * cos(angle);
        float y = ofMap(i, 0, total, -height / 2, height / 2);
        float z = localRadius * sin(angle);

        float noise = ofNoise(x * noiseScale, y * noiseScale, time * 0.5);
        float size = ofMap(noise, 0, 1, 3, 12);
        float brightness = ofMap(noise, 0, 1, 180, 255);

        ofSetColor(brightness);  // grayscale
        ofPushMatrix();
        ofTranslate(x, y, z);
        ofDrawSphere(size);
        ofPopMatrix();
    }

    cam.end();
}
