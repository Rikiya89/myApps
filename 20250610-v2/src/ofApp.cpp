#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(60);

    ofSetBackgroundAuto(true);
    ofEnableAlphaBlending();

    time = 0.0;
    numTori = 35;

    for (int i = 0; i < numTori; i++) {
        positions.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
        baseRotations.push_back(ofRandom(360));
    }

    cam.setDistance(800);
}

void ofApp::update() {
    time += ofGetLastFrameTime();
}

void ofApp::draw() {
    cam.begin();

    for (int i = 0; i < numTori; i++) {
        ofPushMatrix();
        ofVec3f pos = positions[i];

        // Floating animation with fluid motion
        float noiseOffset = ofNoise(i * 0.1, time * 0.2);
        float floatY = sin(time * 1.2 + i * 0.1) * 30.0 + noiseOffset * 40.0;
        ofTranslate(pos.x, pos.y + floatY, pos.z);

        // Rotation
        float angle = baseRotations[i] + time * 30.0;
        ofRotateDeg(angle, 1, 1, 0);

        // Scale pulsing
        float scale = 1.0 + 0.4 * sin(time * 1.5 + i);
        ofScale(scale, scale, scale);

        // 💖 Beautiful RGB blend
        ofSetColor(120 + 100 * sin(i + time), 150 + 100 * cos(i + time), 255, 180);

        drawTorus(30, 10, 40, 30);

        ofPopMatrix();
    }


    cam.end();
}

void ofApp::mousePressed(int x, int y, int button) {
    // Scatter the toruses again when clicked
    for (int i = 0; i < numTori; i++) {
        positions[i] = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        baseRotations[i] = ofRandom(360);
    }
}

void ofApp::drawTorus(float radius, float tubeRadius, int sides, int rings) {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    for (int i = 0; i <= sides; i++) {
        float theta1 = TWO_PI * i / sides;
        float theta2 = TWO_PI * (i + 1) / sides;

        for (int j = 0; j <= rings; j++) {
            float phi = TWO_PI * j / rings;

            for (int k = 0; k < 2; k++) {
                float theta = (k == 0) ? theta1 : theta2;

                float x = (radius + tubeRadius * cos(phi)) * cos(theta);
                float y = (radius + tubeRadius * cos(phi)) * sin(theta);
                float z = tubeRadius * sin(phi);

                mesh.addVertex(ofVec3f(x, y, z));
            }
        }
    }

    mesh.draw();
}
