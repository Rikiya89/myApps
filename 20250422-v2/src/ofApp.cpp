#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableDepthTest();

    // Create symmetrical points in 3D (like Metatron's Cube or Flower of Life)
    int rings = 3;
    float radius = 100;
    for (int z = -rings; z <= rings; z++) {
        for (int y = -rings; y <= rings; y++) {
            for (int x = -rings; x <= rings; x++) {
                glm::vec3 p = glm::vec3(x, y, z) * radius;
                if (glm::length(p) < radius * rings + 1) {
                    points.push_back(p);
                }
            }
        }
    }

    originalPoints = points;

    // Create line mesh
    mesh.setMode(OF_PRIMITIVE_LINES);
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            float d = glm::distance(points[i], points[j]);
            if (d > 0 && d < radius * 1.5) {
                mesh.addVertex(points[i]);
                mesh.addVertex(points[j]);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    points = originalPoints;

    float time = ofGetElapsedTimef();
    for (auto& p : points) {
        float angle = time * 0.2;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0, 1, 0));
        p = glm::vec3(rotation * glm::vec4(p, 1.0));
    }

    // Update mesh with new positions
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_LINES);
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            float d = glm::distance(points[i], points[j]);
            if (d > 0 && d < 150) {
                mesh.addVertex(points[i]);
                mesh.addVertex(points[j]);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Camera orbit animation
    float t = ofGetElapsedTimef();
    cam.setPosition(glm::vec3(sin(t * 0.2f) * 600, sin(t * 0.15f) * 300, cos(t * 0.2f) * 600));
    cam.lookAt(glm::vec3(0, 0, 0));

    cam.begin();

    // Enable additive blending for glow
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    // Glowing aura for each sphere (soft glow)
    ofSetColor(255, 50); // faint glow
    for (auto& p : points) {
        ofDrawSphere(p, 7); // larger faint sphere
    }

    // Draw core spheres
    ofSetColor(255);
    for (auto& p : points) {
        ofDrawSphere(p, 3.5); // core sphere
    }

    // Glowing lines
    ofSetColor(255, 120); // translucent white
    mesh.draw();

    ofDisableBlendMode();

    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
