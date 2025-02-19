#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    int numParticles = 8000; // Increase the number of particles for more complexity
    for (int i = 0; i < numParticles; i++) {
        float x = ofRandomWidth();
        float y = ofRandomHeight();
        float radius = ofRandom(1, 5); // Vary the sizes of particles
        ofColor color = ofColor::fromHsb(ofRandom(255), 200, 255, 150);
        particles.push_back(Particle(x, y, radius, color));
    }
    time = 0;

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F); // Allocate the framebuffer
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();

    bgColor1 = ofColor(10, 20, 30);
    bgColor2 = ofColor(30, 60, 90);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;
    for (auto& particle : particles) {
        particle.update(time);
        if (particle.position.x > ofGetWidth()) particle.position.x = 0;
        if (particle.position.x < 0) particle.position.x = ofGetWidth();
        if (particle.position.y > ofGetHeight()) particle.position.y = 0;
        if (particle.position.y < 0) particle.position.y = ofGetHeight();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw gradient background
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    mesh.addVertex(ofPoint(0, 0));
    mesh.addColor(bgColor1);
    mesh.addVertex(ofPoint(ofGetWidth(), 0));
    mesh.addColor(bgColor2);
    mesh.addVertex(ofPoint(ofGetWidth(), ofGetHeight()));
    mesh.addColor(bgColor1);
    mesh.addVertex(ofPoint(0, ofGetHeight()));
    mesh.addColor(bgColor2);
    mesh.draw();

    fbo.begin();
    ofSetColor(0, 0, 0, 10); // Slightly fade the previous frame for blending
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    for (auto& particle : particles) {
        particle.draw();
    }
    fbo.end();

    ofSetColor(255); // Set the color to white for drawing the framebuffer
    fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mousePos.set(x, y);
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
