#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();

    int numPoints = 1000;
    float helixRadius = 200;
    float helixHeight = 800;
    float helixTurns = 5;

    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth(2);

    ofBeginShape();
    for (int i = 0; i < numPoints; i++) {
        float t = ofMap(i, 0, numPoints, 0, TWO_PI * helixTurns);
        float x = helixRadius * cos(t);
        float y = helixRadius * sin(t);
        float z = ofMap(i, 0, numPoints, -helixHeight / 2, helixHeight / 2);

        ofVertex(x, y, z);
    }
    ofEndShape();

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
