#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();

    sphereMesh = ofMesh::sphere(200, 30); // Reduced resolution for performance
    originalVertices = sphereMesh.getVertices();
    time = 0.0;

    // Initialize trails
    trails.resize(sphereMesh.getNumVertices());

    // Setup first light
    light1.setPosition(300, 300, 600);
    light1.setDiffuseColor(ofFloatColor(0.8, 0.8, 1.0));
    light1.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    light1.enable();

    // Setup second light
    light2.setPosition(-300, -300, 600);
    light2.setDiffuseColor(ofFloatColor(1.0, 0.8, 0.8));
    light2.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    light2.enable();

    // Setup material properties
    material.setShininess(64);
    material.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update() {
    time += 0.01;
    int i = 0;
    float pulsateFactor = sin(time * 2) * 0.5 + 1.5;
    for (auto &vertex : sphereMesh.getVertices()) {
        glm::vec3 originalVertex = originalVertices[i++];
        glm::vec3 normalizedVertex = glm::normalize(originalVertex);
        float noise = ofNoise(normalizedVertex.x * 0.1, normalizedVertex.y * 0.1, normalizedVertex.z * 0.1, time);
        float oscillation = sin(time * 5.0 + glm::length(originalVertex) * 0.1) * 5.0;
        glm::vec3 newVertex = normalizedVertex * (200 * pulsateFactor + noise * 100 + oscillation);
        vertex = newVertex;

        // Update trails
        trails[i - 1].push_back(newVertex);
        if (trails[i - 1].size() > maxTrailLength) {
            trails[i - 1].erase(trails[i - 1].begin());
        }
    }
}

//--------------------------------------------------------------
void ofApp::applyColorTransition(float &colorValue) {
    float phase = fmod(time * 10, 255); // Create a phase for smooth transitions
    colorValue = ofMap(sin(phase), -1, 1, 0, 255);
}

//--------------------------------------------------------------
void ofApp::drawVertexEffects(const glm::vec3 &vertex, float colorValue, int trailIndex) {
    // Apply color transition
    applyColorTransition(colorValue);

    // Draw small sphere at vertex
    ofSetColor(0, colorValue, 255 - colorValue, 200);
    ofDrawSphere(vertex, 4); // Increase sphere size for better visibility

    // Glow effect
    for (int i = 0; i < 3; ++i) {
        ofSetColor(0, colorValue, 255 - colorValue, 30 - i * 10);
        ofDrawSphere(vertex, 8 + i * 4); // Layered glow effect
    }

    // Additional visual effects
    ofSetColor(255, 255, 255, 50);
    ofDrawLine(vertex, vertex * 1.1);

    // Trail effect
    for (size_t j = 1; j < trails[trailIndex].size(); ++j) {
        float trailColorValue = ofMap(j, 0, trails[trailIndex].size(), 0, 255);
        ofSetColor(0, trailColorValue, 255 - trailColorValue, 150);
        ofDrawLine(trails[trailIndex][j - 1], trails[trailIndex][j]);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();

    ofEnableLighting();
    material.begin();

    // Rotate the sphere and add dynamic camera movement
    ofRotateDeg(time * 10, 0.5, 1.0, 0.3);
    cam.orbitDeg(time * 5, 30, cam.getDistance() + sin(time * 2) * 50);
    cam.dolly(sin(time * 1.5) * 20); // Add dolly motion

    ofSetColor(255, 150); // Set white color with some transparency
    sphereMesh.drawWireframe();

    for (int i = 0; i < sphereMesh.getNumVertices(); ++i) {
        auto &vertex = sphereMesh.getVertices()[i];
        float colorValue = ofMap(ofNoise(vertex.x * 0.1, vertex.y * 0.1, vertex.z * 0.1, time), 0, 1, 0, 255);
        drawVertexEffects(vertex, colorValue, i);
    }

    // Add particles
    ofSetColor(255, 255, 255, 100);
    for (int i = 0; i < 100; ++i) {
        float x = ofRandom(-300, 300);
        float y = ofRandom(-300, 300);
        float z = ofRandom(-300, 300);
        ofDrawSphere(x, y, z, 2);
    }

    material.end();
    ofDisableLighting();

    cam.end();
}
