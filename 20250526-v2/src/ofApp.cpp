#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(10);
    ofEnableDepthTest();
    gridSize = 6;  // smaller size for performance
}

void ofApp::update() {
    // You can add logic here later if needed
}

void ofApp::draw() {
    cam.begin();

    float time = ofGetElapsedTimef();
    int spacing = 25;

    for (int x = -gridSize; x <= gridSize; x++) {
        for (int y = -gridSize; y <= gridSize; y++) {
            for (int z = -gridSize; z <= gridSize; z++) {

                glm::vec3 pos(x * spacing, y * spacing, z * spacing);

                float noise = ofNoise(x * 0.1, y * 0.1, z * 0.1, time * 0.4);
                float size = ofMap(noise, 0, 1, 2, 12);

                float offsetX = ofSignedNoise(x * 0.1, time * 0.2) * 10;
                float offsetY = ofSignedNoise(y * 0.1, time * 0.2) * 10;
                float offsetZ = ofSignedNoise(z * 0.1, time * 0.2) * 10;

                float hue = fmod((x + y + z + time * 20), 255);
                ofColor color = ofColor::fromHsb(hue, 200, 255);
                ofSetColor(color);

                ofPushMatrix();
                ofTranslate(pos.x + offsetX, pos.y + offsetY, pos.z + offsetZ);
                ofRotateDeg(time * 10 + x + y + z, 1.0, 1.0, 0.0);
                ofDrawBox(size);
                ofPopMatrix();
            }
        }
    }

    cam.end();
}
