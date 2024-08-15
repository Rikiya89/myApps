#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    // Create a cube mesh
    mesh = ofMesh::box(200, 200, 200, 10, 10, 10);
    
    // Manipulate the vertices for a generative effect
    for (auto & vertex : mesh.getVertices()) {
        vertex.x += ofRandom(-10, 10);
        vertex.y += ofRandom(-10, 10);
        vertex.z += ofRandom(-10, 10);
    }

    // Initialize particles
    int numParticles = 500;
    for (int i = 0; i < numParticles; i++) {
        particles.push_back(ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300)));
        particleColors.push_back(ofColor::fromHsb(ofRandom(0, 255), 200, 255, 100));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    // Rotate the cube
    ofRotateDeg(ofGetElapsedTimef() * 20, 1.0, 1.0, 0.0);

    // Update particles to orbit around the cube
    for (int i = 0; i < particles.size(); i++) {
        float time = ofGetElapsedTimef() * 0.5;
        particles[i].x = 300 * sin(time + i);
        particles[i].y = 300 * cos(time + i * 0.5);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();

    // Draw the main cube
    ofSetColor(255, 255, 255, 150);  // Soft white with transparency
    mesh.drawWireframe();
    
    // Draw pulsating aura around the cube
    float time = ofGetElapsedTimef();
    float scale = 1.5 + sin(time * 2.0) * 0.2; // Pulsating effect
    float alpha = 100 + (sin(time * 4.0) * 50); // Dynamic transparency

    ofPushMatrix();
    ofScale(scale, scale, scale);  // Scale the aura

    // Create a gradient color effect for the aura
    ofColor innerColor(255, 255, 255, alpha);  // Soft white
    ofColor outerColor(200, 200, 255, alpha / 2); // Light blue with more transparency
    for (int i = 0; i < 5; i++) {
        ofSetColor(innerColor.lerp(outerColor, i / 4.0));  // Gradient effect
        mesh.drawWireframe();
        ofScale(1.05, 1.05, 1.05);  // Slightly increase scale with each iteration
    }

    ofPopMatrix();

    // Draw glowing effect around the cube
    ofSetColor(255, 255, 255, 50);  // Very soft white for glow
    ofDrawSphere(0, 0, 0, 250);  // Draw a sphere around the cube for glow effect

    // Draw particles orbiting around the cube
    for (int i = 0; i < particles.size(); i++) {
        ofSetColor(particleColors[i]);
        ofDrawSphere(particles[i], 2);  // Draw small spheres as particles
    }

    cam.end();
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
