#include "ofApp.h"

void ofApp::setup(){
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(120);
    ofSetFrameRate(60);
}

void ofApp::update(){
    angleOffset += 0.003;

    ringPetalPositions.clear();
    for (int r = 0; r < numRings; r++) {
        std::vector<glm::vec3> ring;
        float radius = baseRadius + r * 80;
        float pulse = 1.0 + 0.15 * sin(ofGetElapsedTimef() * (1.5 + 0.3 * r));
        for (int i = 0; i < petalsPerRing; i++) {
            float angle = TWO_PI / petalsPerRing * i + angleOffset * (1 + 0.2 * r);
            float deform = sin(angle * 5 + ofGetElapsedTimef() * 2) * 10;
            float x = cos(angle) * (radius + deform) * pulse;
            float y = sin(angle) * (radius + deform) * pulse;
            float z = cos(angle * 3 + r) * 30;
            ring.push_back(glm::vec3(x, y, z));
        }
        ringPetalPositions.push_back(ring);
    }
}

void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotateDeg(ofGetElapsedTimef() * 3);

    // ✨ Background floating particles
    for (int i = 0; i < 300; i++) {
        float a = ofRandom(TWO_PI);
        float d = ofRandom(100, 600);
        ofSetColor(255, 120, 240, ofRandom(10, 50));
        ofDrawCircle(cos(a) * d, sin(a) * d, ofRandom(0.5, 2));
    }

    // 🌐 Draw each ring
    for (int r = 0; r < numRings; r++) {
        auto& petals = ringPetalPositions[r];

        // 🔶 Sacred thin golden lines
        for (int i = 0; i < petals.size(); i++) {
            int next6 = (i + 6) % petals.size();
            int next12 = (i + 12) % petals.size();

            ofSetColor(255, 215, 100, 25); // soft gold
            ofSetLineWidth(0.5);
            ofDrawLine(petals[i], petals[next6]);
            ofDrawLine(petals[i], petals[next12]);
            ofSetLineWidth(1);
        }

        // 🌈 Glowing petals
        for (int i = 0; i < petals.size(); i++) {
            ofPushMatrix();
            ofTranslate(petals[i]);
            float hue = fmod(i * 5 + ofGetElapsedTimef() * 10 + r * 40, 255);
            ofColor c;
            c.setHsb(hue, 200, 255, 200);
            ofSetColor(c);
            ofDrawCircle(0, 0, 8 + 2 * sin(ofGetElapsedTimef() + i));
            ofNoFill();
            ofDrawCircle(0, 0, 30 + 5 * sin(ofGetElapsedTimef() + i * 0.5));
            ofFill();
            ofPopMatrix();
        }
    }

    ofPopMatrix();
}
