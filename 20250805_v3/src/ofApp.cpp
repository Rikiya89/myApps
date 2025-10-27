#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    time = 0;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < 1000; i++) {
        points.push_back(glm::vec3(ofRandom(-ofGetWidth()/2, ofGetWidth()/2),
                                     ofRandom(-ofGetHeight()/2, ofGetHeight()/2),
                                     ofRandom(-500, 500)));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
    ofRotateYDeg(ofGetElapsedTimef() * 10);

    mesh.clear();
    for (auto& p : points) {
        float noise = ofNoise(p.x * 0.005, p.y * 0.005, p.z * 0.005 + time);
        glm::vec3 displaced_p = p * (1 + noise * 2);
        mesh.addVertex(displaced_p);
        mesh.addColor(ofColor::fromHsb(noise * 255, 255, 255));
    }
    mesh.draw();

    ofPopMatrix();
    ofDisableDepthTest();
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
