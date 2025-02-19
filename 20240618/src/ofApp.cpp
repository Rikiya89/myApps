#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    for (int i = 0; i < numParticles; i++) {
        ofVec2f position(ofRandomWidth(), ofRandomHeight());
        ofVec2f velocity(ofRandom(-2, 2), ofRandom(-2, 2));
        float size = ofRandom(5, 20);
        ofColor color;
        color.setHsb(ofRandom(255), 255, 255);
        particles.push_back(Particle(position, velocity, size, color));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& particle : particles) {
        particle.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (auto& particle : particles) {
        float dist = ofDist(particle.position.x, particle.position.y, mousePos.x, mousePos.y);
        particle.color.a = ofMap(dist, 0, ofGetWidth() / 2, 255, 50);
        particle.draw();
    }

    // Draw trails
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            float dist = particles[i].position.distance(particles[j].position);
            if (dist < 100) {
                ofSetColor(255, ofMap(dist, 0, 100, 255, 0));
                ofDrawLine(particles[i].position, particles[j].position);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousePos.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for (auto& particle : particles) {
        ofVec2f dir = particle.position - mousePos;
        dir.normalize();
        particle.velocity += dir * 2;
    }
}
