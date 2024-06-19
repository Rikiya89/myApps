#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    time = 0.0;
    numPoints = 150;
    
    for (int i = 0; i < numPoints; i++) {
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(150, 390);
        points.push_back(ofVec2f(radius * cos(angle), radius * sin(angle)));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    
    for (int i = 0; i < numPoints; i++) {
        ofPushMatrix();
        ofRotateDeg(ofGetElapsedTimef() * 10 + i * 1.5);
        
        float x = points[i].x * sin(time + i * 0.3);
        float y = points[i].y * cos(time + i * 0.3);
        float size = 15 + 9 * sin(time + i * 0.6);
        
        ofSetColor(ofColor::fromHsb((int)(time * 10 + i * 5) % 255, 200, 255));
        ofDrawCircle(x, y, size);
        
        ofPopMatrix();
    }
    
    for (int i = 0; i < numPoints; i++) {
        for (int j = i + 1; j < numPoints; j++) {
            if (ofDist(points[i].x, points[i].y, points[j].x, points[j].y) < 150) {
                ofSetColor(255, 50);
                ofDrawLine(points[i].x * sin(time + i * 0.1), points[i].y * cos(time + i * 0.1),
                           points[j].x * sin(time + j * 0.1), points[j].y * cos(time + j * 0.1));
            }
        }
    }
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
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}
