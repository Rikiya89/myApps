#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    int numVertices = 3000;  // Increase number of vertices for more detail
    noiseScale = 0.005;
    
    for (int i = 0; i < numVertices; ++i) {
        vertices.push_back(ofPoint(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
        colors.push_back(ofColor(ofRandom(255), ofRandom(255), ofRandom(255)));
    }
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    fbo.end();
    
    startColor = ofColor(0, 128, 255);
    endColor = ofColor(255, 64, 128);
}

//--------------------------------------------------------------
void ofApp::update(){
    float time = ofGetElapsedTimef();
    for (int i = 0; i < vertices.size(); ++i) {
        auto &vertex = vertices[i];
        float noiseValue = ofNoise(vertex.x * noiseScale, vertex.y * noiseScale, time * 0.1);
        float angle = noiseValue * TWO_PI;
        vertex.x += cos(angle) * 3;
        vertex.y += sin(angle) * 3;
        
        if (vertex.x < 0) vertex.x = ofGetWidth();
        if (vertex.x > ofGetWidth()) vertex.x = 0;
        if (vertex.y < 0) vertex.y = ofGetHeight();
        if (vertex.y > ofGetHeight()) vertex.y = 0;
        
        float colorLerp = ofMap(noiseValue, 0, 1, 0, 1);
        colors[i] = startColor.getLerped(endColor, colorLerp);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
    ofSetColor(0, 0, 0, 10);  // Semi-transparent black for trail effect
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for (int i = 0; i < vertices.size(); ++i) {
        ofSetColor(colors[i]);
        ofSetLineWidth(2);
        mesh.addVertex(vertices[i]);
        mesh.addColor(colors[i]);
    }
    
    mesh.draw();
    fbo.end();
    
    ofSetColor(255);
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
