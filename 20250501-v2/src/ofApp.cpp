#include "ofApp.h"

void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    baseRotationAngle = 0.0;
    scaleFactor = 1.0;

    int numStars = 300;
    for (int i = 0; i < numStars; i++) {
        stars.push_back(glm::vec2(ofRandomWidth(), ofRandomHeight()));
    }

    int totalCircles = 500;
    wiggleOffsets.resize(totalCircles);
    for (auto& w : wiggleOffsets) {
        w = ofRandom(1000);
    }
}

void ofApp::update(){
    baseRotationAngle += 0.03;
    float time = ofGetElapsedTimef();
    scaleFactor = 1.0 + 0.02 * sin(time * 0.4);

    for (auto& star : stars) {
        star.y -= 0.1;
        if (star.y < 0) star.y = ofGetHeight();
    }
}

void ofApp::draw(){
    // Background gradient
    ofColor topColor(10, 10, 30);
    ofColor bottomColor(0, 0, 0);
    ofMesh bgMesh;
    bgMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    bgMesh.addVertex(glm::vec3(0, 0, 0));
    bgMesh.addColor(topColor);
    bgMesh.addVertex(glm::vec3(ofGetWidth(), 0, 0));
    bgMesh.addColor(topColor);
    bgMesh.addVertex(glm::vec3(ofGetWidth(), ofGetHeight(), 0));
    bgMesh.addColor(bottomColor);
    bgMesh.addVertex(glm::vec3(0, ofGetHeight(), 0));
    bgMesh.addColor(bottomColor);
    bgMesh.draw();

    // Stars
    ofPushStyle();
    for (auto& star : stars) {
        ofSetColor(255, 255, 255, ofRandom(80, 150));
        ofDrawCircle(star.x, star.y, ofRandom(0.5, 1.5));
    }
    ofPopStyle();

    // Sacred flower (multi-layer)
    float time = ofGetElapsedTimef();
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

    // Background faded layers
    drawFlowerOfLife(0, 0, 40, 5, time * 0.5, 0.3, 1.2, 0.5);
    drawFlowerOfLife(0, 0, 40, 5, time * 0.8, 0.5, 1.05, 0.8);

    // Main bright layer
    drawFlowerOfLife(0, 0, 40, 5, time, 1.0, 1.0, 1.0);

    ofPopMatrix();
}

void ofApp::drawFlowerOfLife(float centerX, float centerY, float radius, int layers, float timeOffset, float alphaScale, float scaleMult, float rotationMult){
    float spacing = radius * sqrt(3);
    int wiggleIndex = 0;
    float localRotation = baseRotationAngle * rotationMult;
    float breathing = 1.0 + 0.05 * sin(timeOffset * 0.8);

    vector<ofColor> colors = {
        ofColor(255, 150, 150),
        ofColor(255, 200, 150),
        ofColor(255, 255, 180),
        ofColor(180, 255, 180),
        ofColor(180, 220, 255),
        ofColor(220, 180, 255),
    };

    ofPushMatrix();
    ofRotateDeg(localRotation);
    ofScale(scaleMult, scaleMult);

    for (int layer = 0; layer <= layers; ++layer){
        for (int i = -layer; i <= layer; ++i){
            for (int j = -layer; j <= layer; ++j){
                float baseX = centerX + (i * spacing) + (j * spacing / 2);
                float baseY = centerY + (j * spacing * 0.866);

                if (glm::length(glm::vec2(baseX, baseY)) < spacing * (layers + 0.5)) {
                    float wiggleX = 4.0 * sin(timeOffset + wiggleOffsets[wiggleIndex]);
                    float wiggleY = 4.0 * cos(timeOffset + wiggleOffsets[wiggleIndex]);
                    float breathingRadius = radius * (0.9 + 0.1 * sin(timeOffset * 2.0 + wiggleOffsets[wiggleIndex]));

                    ofColor c = colors[wiggleIndex % colors.size()];
                    c.setHueAngle(fmod(c.getHueAngle() + timeOffset * 5.0, 360));
                    c.a = 160 * alphaScale;
                    ofSetColor(c);

                    ofNoFill();
                    ofSetLineWidth(1.0);

                    // Halo aura
                    ofPushStyle();
                    ofSetColor(c, 50 * alphaScale);
                    ofDrawCircle(baseX + wiggleX, baseY + wiggleY, breathingRadius * breathing * 1.1);
                    ofPopStyle();

                    ofDrawCircle(baseX + wiggleX, baseY + wiggleY, breathingRadius * breathing);

                    wiggleIndex++;
                }
            }
        }
    }

    ofPopMatrix();
}
