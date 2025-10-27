#include "ofApp.h"

void ofApp::setup() {
    ofBackground(0);
    trailLayer1.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
    trailLayer2.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);

    trailLayer1.begin();
    ofClear(0, 0, 0, 255);
    trailLayer1.end();

    trailLayer2.begin();
    ofClear(0, 0, 0, 255);
    trailLayer2.end();

    time = 0;
    repel = false;
    bgColor1 = ofColor(10, 20, 50);
    bgColor2 = ofColor(0, 0, 0);
}

void ofApp::update() {
    time += 0.01;

    particles.push_back(Particle());

    ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());

    for (int i = particles.size() - 1; i >= 0; i--) {
        particles[i].update(mousePos, time, repel);
        if (particles[i].lifespan < 0) {
            particles.erase(particles.begin() + i);
        }
    }

    float colorShift = sin(time * 0.1);
    bgColor1 = ofColor::fromHsb(150 + colorShift * 50, 100, 100);
    bgColor2 = ofColor::fromHsb(200 - colorShift * 50, 100, 50);
}

void ofApp::draw() {
    trailLayer1.begin();
    ofSetColor(0, 0, 0, 15);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].draw();

        for (size_t j = i + 1; j < particles.size(); j++) {
            float distance = particles[i].position.distance(particles[j].position);
            if (distance < 60) {
                float thickness = ofMap(distance, 0, 60, 2.5, 0.5);
                ofSetLineWidth(thickness);
                ofSetColor(255, 100);
                ofDrawLine(particles[i].position, particles[j].position);
            }
        }
    }
    trailLayer1.end();

    trailLayer2.begin();
    trailLayer1.draw(0, 0);
    trailLayer2.end();

    trailLayer2.draw(0, 0);
}

void ofApp::mousePressed(int x, int y, int button) {
    repel = !repel;
}
