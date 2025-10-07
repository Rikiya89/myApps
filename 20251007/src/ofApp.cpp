#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableSmoothing();

    // Create particles
    for (int i = 0; i < 200; i++) {
        particles.push_back(Particle());
    }

    // Setup mesh for connecting particles
    mesh.setMode(OF_PRIMITIVE_LINES);

    time = 0;

    // Setup lighting
    light.setPosition(500, 500, 500);
    light.enable();
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;

    // Update all particles
    for (auto& p : particles) {
        p.update();
    }

    // Build mesh connections between nearby particles
    mesh.clear();
    float connectionDist = 150;

    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            float dist = particles[i].position.distance(particles[j].position);
            if (dist < connectionDist) {
                float alpha = ofMap(dist, 0, connectionDist, 255, 0);
                mesh.addVertex(particles[i].position);
                mesh.addColor(ofFloatColor(1, alpha / 255.0));
                mesh.addVertex(particles[j].position);
                mesh.addColor(ofFloatColor(1, alpha / 255.0));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);

    cam.begin();

    // Rotate the entire scene
    ofPushMatrix();
    ofRotateDeg(time * 10, 0, 1, 0);
    ofRotateDeg(time * 5, 1, 0, 0);

    // Draw connection lines
    ofSetColor(255, 100);
    mesh.draw();

    // Draw particles as spheres and boxes
    for (int i = 0; i < particles.size(); i++) {
        ofPushMatrix();
        ofTranslate(particles[i].position);
        ofRotateDeg(particles[i].rotation, 1, 1, 1);

        ofSetColor(255);

        // Alternate between spheres and boxes
        if (i % 3 == 0) {
            ofDrawSphere(0, 0, 0, particles[i].size);
        } else if (i % 3 == 1) {
            ofDrawBox(0, 0, 0, particles[i].size);
        } else {
            // Draw wireframe box
            ofNoFill();
            ofSetLineWidth(2);
            ofDrawBox(0, 0, 0, particles[i].size * 1.5);
            ofFill();
        }

        ofPopMatrix();
    }

    // Draw some additional geometric elements
    ofPushMatrix();
    ofRotateDeg(time * 20, 0, 1, 0);
    ofNoFill();
    ofSetColor(255);
    ofSetLineWidth(2);
    ofDrawSphere(0, 0, 0, 250);
    ofFill();
    ofPopMatrix();

    // Draw spiraling torus
    ofPushMatrix();
    ofRotateDeg(time * 15, 1, 0, 1);
    ofSetColor(255);
    ofDrawCylinder(0, 0, 0, 5, 400);
    ofPopMatrix();

    ofPopMatrix();

    cam.end();

    // Draw info
    ofDisableLighting();
    ofSetColor(255);
//    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 0), 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
