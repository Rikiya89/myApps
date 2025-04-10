#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofEnableDepthTest();
    ofBackground(2, 2, 12);
    ofEnableSmoothing();
    ofSetCircleResolution(60);
    ofEnableAlphaBlending();

    numPoints = 1200;
    radius = 300;

    for (int i = 0; i < numPoints; i++) {
        float theta = ofRandom(TWO_PI);
        float phi = ofRandom(PI);
        float r = radius * ofRandom(0.6, 1.0);

        float x = r * sin(phi) * cos(theta);
        float y = r * sin(phi) * sin(theta);
        float z = r * cos(phi);

        points.push_back(glm::vec3(x, y, z));
    }
}

void ofApp::update(){
}

void ofApp::draw(){
    ofEnableDepthTest();
    cam.begin();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    float time = ofGetElapsedTimef();

    ofPushMatrix();
    ofRotateDeg(time * 6, 0.4, 1.0, 0.25);

    for (int i = 0; i < points.size(); i++) {
        glm::vec3 p = points[i];

        // Smooth noise-based wobble
        glm::vec3 animated = p + glm::vec3(
            ofSignedNoise(p.x * 0.01, time * 0.2) * 6,
            ofSignedNoise(p.y * 0.01, time * 0.2) * 6,
            ofSignedNoise(p.z * 0.01, time * 0.2) * 6
        );

        // Chakra color based on distance
        float dist = glm::length(p);
        float hue = ofMap(dist, 0, radius, 0, 255); // red to violet
        float alpha = ofMap(dist, 0, radius, 255, 100);
        ofColor c = ofColor::fromHsb(hue, 200, 255, alpha);
        ofSetColor(c);

        float size = ofMap(sin(time + dist * 0.05), -1, 1, 2.5, 6.5);
        ofDrawSphere(animated, size);

        // Optional aura effect (halo glow)
        ofSetColor(c, 60); // low alpha halo
        ofDrawSphere(animated, size * 2.5);
    }

    // Sacred geometry-style line connections
    ofSetColor(255, 60);
    for (int i = 0; i < points.size(); i += 6) {
        for (int j = i + 6; j < points.size(); j += 6) {
            float d = glm::distance(points[i], points[j]);
            if (d < 150) {
                glm::vec3 a = points[i];
                glm::vec3 b = points[j];
                ofDrawLine(a, b);
            }
        }
    }

    ofPopMatrix();
    ofDisableBlendMode();
    cam.end();
    ofDisableDepthTest();
}
