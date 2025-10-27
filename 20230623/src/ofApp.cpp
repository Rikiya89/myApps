#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();

    gridSize = 12;
    spacing = 50.0;
    time = 0.0;

    for(int x = -gridSize; x <= gridSize; x++) {
        for(int y = -gridSize; y <= gridSize; y++) {
            for(int z = -gridSize; z <= gridSize; z++) {
                float dist = glm::length(glm::vec3(x, y, z));
                if(dist > 3 && dist < gridSize) {  // central void
                    sphereCenters.push_back(ofVec3f(x, y, z) * spacing);
                }
            }
        }
    }
}

void ofApp::update(){
    time += 0.015;
}

void ofApp::draw(){
    float camRadius = 600 + 200 * sin(time * 0.5);
    cam.setPosition(glm::vec3(
        camRadius * sin(time * 0.4),
        camRadius * cos(time * 0.3),
        camRadius * sin(time * 0.2)
    ));
    cam.lookAt(glm::vec3(0, 0, 0));

    cam.begin();
    ofEnableDepthTest();

    for(auto& center : sphereCenters){
        float n = ofNoise(center.x * 0.01, center.y * 0.01, center.z * 0.01, time);
        float pulse = sin(time * 2.0 + center.length() * 0.05) * 0.5 + 0.5;
        float size = ofMap(n * pulse, 0, 1, 3, 14);

        float alpha = ofMap(center.length(), 0, gridSize * spacing, 255, 40);
        ofSetColor(255, alpha);  // glowing transparency

        ofDrawSphere(center, size);
    }

    cam.end();
}
