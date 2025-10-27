#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetCircleResolution(180);
    time = 0.0;
}

void ofApp::update(){
    time += 0.007; // slower, smoother animation
}

void ofApp::draw(){
    // --- Afterimage trail background ---
    ofSetColor(0, 18); // transparent black overlay
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    // --- Gentle zoom breathing ---
    float zoom = 1.0 + 0.05 * sin(time * 0.5);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofScale(zoom, zoom);

    int layers = 28;

    for(int i = 0; i < layers; i++){
        float baseRadius = 40 + i * 16.0;
        float breathing = 1.0 + 0.07 * sin(time * 0.7 + i * 0.3);
        float wave = sin(time * 1.2 + i * 0.5) * 6.0;
        float radius = (baseRadius + wave) * breathing;

        // counter-rotation for hypnotic flow
        float angle = time * (i % 2 == 0 ? (i * 0.08 + 0.3) : -(i * 0.08 + 0.3));

        float alpha = ofMap(i, 0, layers, 100, 15);
        ofSetColor(255, alpha);
        ofSetLineWidth(ofMap(i, 0, layers, 1.2, 0.2));

        ofPushMatrix();
        ofRotateDeg(ofRadToDeg(angle));

        // --- Draw polygon ring ---
        ofBeginShape();
        int sides = 6;
        for(int j = 0; j < sides; j++){
            float theta = ofMap(j, 0, sides, 0, TWO_PI);
            float x = cos(theta) * radius;
            float y = sin(theta) * radius;
            ofVertex(x, y);
        }
        ofEndShape(true);

        // --- Flowing highlights along edges ---
        for(int j = 0; j < sides; j++){
            float theta = ofMap(j, 0, sides, 0, TWO_PI);
            float x = cos(theta) * radius;
            float y = sin(theta) * radius;

            float flow = 0.5 + 0.5 * sin(time * 2.0 + i * 0.3 + j * 1.2);
            ofSetColor(255, alpha * flow * 0.8);
            ofDrawCircle(x, y, 2.5 + flow * 2.0); // glowing shimmer
        }

        ofPopMatrix();
    }

    ofPopMatrix();

    // --- Soft vignette (edges fade to black) ---
    ofMesh vignette;
    vignette.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    vignette.addVertex(glm::vec3(ofGetWidth()/2, ofGetHeight()/2, 0));
    vignette.addColor(ofColor(0, 0)); // transparent center
    int steps = 64;
    float maxR = glm::length(glm::vec2(ofGetWidth(), ofGetHeight())) * 0.6;
    for(int i = 0; i <= steps; i++){
        float theta = TWO_PI * i / steps;
        float x = cos(theta) * maxR + ofGetWidth()/2;
        float y = sin(theta) * maxR + ofGetHeight()/2;
        vignette.addVertex(glm::vec3(x, y, 0));
        vignette.addColor(ofColor(0, 255)); // fade to black edges
    }
    vignette.draw();
}
