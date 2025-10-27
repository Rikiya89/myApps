#include "ofApp.h"
#include "glm/gtc/random.hpp" // Important! Add this for sphericalRand

const float ofApp::BASE_RADIUS = 320.0f;

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofSetLineWidth(1.5f);

    meshPrimary.setMode(OF_PRIMITIVE_LINES);
    meshSecondary.setMode(OF_PRIMITIVE_LINES);
    basePos.reserve(NUM_POINTS);

    for (int i = 0; i < NUM_POINTS; ++i) {
        float theta = ofRandom(TWO_PI);
        float phi = acos(ofRandom(-1, 1));
        glm::vec3 p = {
            BASE_RADIUS * sin(phi) * cos(theta),
            BASE_RADIUS * sin(phi) * sin(theta),
            BASE_RADIUS * cos(phi)
        };
        basePos.push_back(p);
        meshPrimary.addVertex(p);
        meshSecondary.addVertex(p);
    }

    for (int i = 0; i < NUM_POINTS; ++i) {
        int a = i;
        int b = (int)ofRandom(NUM_POINTS);
        int c = (int)ofRandom(NUM_POINTS);
        meshPrimary.addIndex(a); meshPrimary.addIndex(b);
        meshPrimary.addIndex(a); meshPrimary.addIndex(c);
        meshSecondary.addIndex(a); meshSecondary.addIndex(b);
        meshSecondary.addIndex(a); meshSecondary.addIndex(c);
    }

    // Create random floating particles
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles.push_back(glm::sphericalRand(BASE_RADIUS * 2.0f));
    }

    rotationOffset = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update() {
    float t = ofGetElapsedTimef();
    float noiseScale = 0.004f;
    float noiseMag = 60.0f;
    float breathing = 1.0f + 0.07f * sin(t * 0.6f); // gentle breathing

    for (int i = 0; i < NUM_POINTS; ++i) {
        const glm::vec3& base = basePos[i];

        float n = ofNoise(base.x * noiseScale, base.y * noiseScale, base.z * noiseScale, t * 0.2f);
        float offset = (n - 0.5f) * noiseMag;
        float wave = 10.0f * sin(t * 1.0f + base.x * 0.008f + base.y * 0.008f + base.z * 0.008f);

        glm::vec3 deform = glm::normalize(base) * (BASE_RADIUS * breathing + offset + wave);
        meshPrimary.setVertex(i, deform);
        meshSecondary.setVertex(i, deform * 1.2f); // Second layer slightly larger
    }

    // Update particle positions
    for (auto& p : particles) {
        p += glm::normalize(p) * 0.1f; // gentle outward drift
        if (glm::length(p) > BASE_RADIUS * 3.0f) {
            p = glm::sphericalRand(BASE_RADIUS * 2.0f);
        }
    }

    // Oscillating rotation offset
    rotationOffset = sin(t * 0.4f) * 20.0f;
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();

    float t = ofGetElapsedTimef();

    ofPushMatrix();
    ofRotateDeg(t * 10.0f + rotationOffset, 0.5f, 1.0f, 0.2f);

    // Soft color shifting
    float hue = 160 + 20 * sin(t * 0.2f);
    ofColor c1 = ofColor::fromHsb(hue, 30, 255);
    ofSetColor(c1);
    meshPrimary.draw();

    ofSetColor(200, 200, 255, 80);
    meshSecondary.draw();

    ofPopMatrix();

    // Draw floating particles
    for (auto& p : particles) {
        ofPushMatrix();
        ofTranslate(p);
        ofSetColor(180, 180, 255, 100);
        ofDrawSphere(0, 0, 0, 1.5f);
        ofPopMatrix();
    }

    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 's' || key == 'S') {
        ofSaveScreen("sacred_breathing_galaxy_" + ofGetTimestampString() + ".png");
    }
}
