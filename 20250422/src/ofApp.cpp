#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableSmoothing();

    int count = 1000;
    points.resize(count);
    for (int i = 0; i < count; i++) {
        float x = ofRandom(-300, 300);
        float y = ofRandom(-300, 300);
        float z = ofRandom(-300, 300);
        points[i].set(x, y, z);
    }

    noiseScale = 0.004;
    time = 0.0;
}

void ofApp::update() {
    time += 0.01;

    for (int i = 0; i < points.size(); i++) {
        ofVec3f &p = points[i];
        float noiseX = ofNoise(p.x * noiseScale, p.y * noiseScale, time);
        float noiseY = ofNoise(p.y * noiseScale, p.z * noiseScale, time);
        float noiseZ = ofNoise(p.z * noiseScale, p.x * noiseScale, time);

        p += ofVec3f((noiseX - 0.5), (noiseY - 0.5), (noiseZ - 0.5)) * 1.8;
        p.x = ofClamp(p.x, -300, 300);
        p.y = ofClamp(p.y, -300, 300);
        p.z = ofClamp(p.z, -300, 300);
    }
}

void ofApp::drawConnections() {
    float maxDist = 50;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            float dist = points[i].distance(points[j]);
            if (dist < maxDist) {
                float alpha = ofMap(dist, 0, maxDist, 255, 0);
                ofSetColor(255, 150, 255, alpha); // spiritual pink glow
                ofDrawLine(points[i], points[j]);
            }
        }
    }
}

void ofApp::drawSpiral() {
    int spiralPoints = 400;
    float radius = 10;
    for (int i = 0; i < spiralPoints; i++) {
        float angle = i * 0.1;
        float r = radius + i * 0.5;
        float x = cos(angle) * r;
        float y = sin(angle) * r;
        float z = sin(i * 0.2 + time * 2) * 50;

        ofColor c;
        c.setHsb(ofMap(i, 0, spiralPoints, 180, 255), 200, 255);
        c.a = ofMap(i, 0, spiralPoints, 255, 50);
        ofSetColor(c);
        ofDrawSphere(x, y, z, 1.8);
    }
}

void ofApp::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    cam.begin();
    ofPushMatrix();
    ofRotateDeg(time * 10, 0, 1, 0); // slow scene rotation

    drawConnections();
    drawSpiral();

    for (auto &p : points) {
        float hue = ofMap(p.z, -300, 300, 180, 300); // pastel gradient
        ofColor c = ofColor::fromHsb(hue, 150, 255);
        c.a = ofMap(p.z, -300, 300, 100, 255);
        ofSetColor(c);
        ofDrawSphere(p, 2.5);
    }

    ofPopMatrix();
    cam.end();
    ofDisableBlendMode();
}
