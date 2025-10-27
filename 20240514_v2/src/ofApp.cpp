#include "ofApp.h"

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);
    
    image.load("mandala.jpg");

    width = image.getWidth();
    height = image.getHeight();
    
    staticMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    animatedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    staticMesh.enableColors();
    animatedMesh.enableColors();
    staticMesh.enableIndices();
    animatedMesh.enableIndices();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ofColor color = image.getColor(x, y);
            float z = ofMap(color.getBrightness(), 0, 255, -100, 100);
            glm::vec3 vertex = glm::vec3(x, y, z);
            staticMesh.addVertex(vertex);
            animatedMesh.addVertex(vertex);
            staticMesh.addColor(color);
            animatedMesh.addColor(color);
        }
    }

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int i1 = x + y * width;
            int i2 = x + 1 + y * width;
            int i3 = x + (y + 1) * width;
            int i4 = x + 1 + (y + 1) * width;

            staticMesh.addIndex(i1);
            staticMesh.addIndex(i2);
            staticMesh.addIndex(i3);
            staticMesh.addIndex(i2);
            staticMesh.addIndex(i4);
            staticMesh.addIndex(i3);

            animatedMesh.addIndex(i1);
            animatedMesh.addIndex(i2);
            animatedMesh.addIndex(i3);
            animatedMesh.addIndex(i2);
            animatedMesh.addIndex(i4);
            animatedMesh.addIndex(i3);
        }
    }
    
    time = 0;  // Initialize time
}

void ofApp::update() {
    time += ofGetLastFrameTime();  // Update time with the time elapsed since the last frame

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = x + y * width;
            glm::vec3 vertex = staticMesh.getVertex(index);
            float z = vertex.z;
            vertex.z = z + 50 * sin(time + (x * 0.1) + (y * 0.1));  // Add a sine wave effect
            animatedMesh.setVertex(index, vertex);
        }
    }
}

void ofApp::draw() {
    ofEnableDepthTest();
    cam.begin();

    ofPushMatrix();
    ofTranslate(-width / 2, -height / 2);
    staticMesh.drawWireframe(); // Draw the static mesh first
    animatedMesh.drawWireframe(); // Draw the animated mesh on top
    ofPopMatrix();

    cam.end();
    ofDisableDepthTest();
}
