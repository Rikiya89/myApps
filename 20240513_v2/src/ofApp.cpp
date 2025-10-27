#include "ofApp.h"

void ofApp::setup(){
    // GUI setup
    gui.setup();
    gui.add(radius.setup("Radius", 35, 10, 100));
    gui.add(numCircles.setup("Number of Circles", 6, 1, 12));

    ofSetBackgroundColor(ofColor::black);
}

void ofApp::update(){

}

void ofApp::draw(){
    drawFlowerOfLife(ofGetWidth()/2, ofGetHeight()/2, radius, numCircles);
    gui.draw();
}

void ofApp::drawFlowerOfLife(float x, float y, float radius, int numCircles) {
    float angle = TWO_PI / numCircles;
    for (int i = 0; i < numCircles; i++) {
        float dx = cos(angle * i) * radius * sqrt(3);
        float dy = sin(angle * i) * radius * sqrt(3);
        ofDrawCircle(x + dx, y + dy, radius);
    }
    // Central circle
    ofDrawCircle(x, y, radius);
}
