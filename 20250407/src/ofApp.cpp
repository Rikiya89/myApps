#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(720, 1080);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
}

void ofApp::update(){
    angle += 0.3;
}

void ofApp::draw(){
    ofBackgroundGradient(ofColor(5, 5, 25), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);

    cam.begin();

    spherePositions.clear();

    int numSpheres = 400;
    float baseRadius = 200;
    float spiralHeight = 800;
    float time = ofGetElapsedTimef();

    for (int i = 0; i < numSpheres; i++) {
        float t = (float)i / numSpheres;

        float theta = t * TWO_PI * 8 + ofDegToRad(angle);
        float y = sin(t * TWO_PI * 4 + time * 0.5) * 100 + ofMap(i, 0, numSpheres, -spiralHeight/2, spiralHeight/2);

        float noiseOffset = ofNoise(t * 3.0, time * 0.4) * 60.0;
        float r = baseRadius + noiseOffset;

        float dir = (i % 2 == 0) ? 1.0 : -1.0;

        float x = r * cos(theta * dir);
        float z = r * sin(theta * dir);

        glm::vec3 pos(x, y, z);
        spherePositions.push_back(pos);

        ofPushMatrix();
        ofTranslate(pos);

        float rot = ofNoise(x * 0.01, y * 0.01, time * 0.3) * 360.0;
        ofRotateYDeg(rot);

        float hue = fmod(time * 30 + i * 0.5, 255);
        ofSetColor(ofColor::fromHsb(hue, 220, 255, 150));

        float pulse = ofMap(sin(time * 2 + i * 0.15), -1, 1, 0.7, 1.4);
        ofDrawSphere(0, 0, 0, 5 * pulse);

        ofPopMatrix();
    }

    // Sacred geometry style: connect each point to every nth neighbor
    ofSetLineWidth(1.0);
    for (int i = 0; i < spherePositions.size(); i++) {
        for (int offset = 6; offset < 60; offset += 6) {
            int j = i + offset;
            if (j < spherePositions.size()) {
                ofSetColor(255, 255, 255, 80); // soft sacred glow lines
                ofDrawLine(spherePositions[i], spherePositions[j]);
            }
        }
    }

    cam.end();
}
