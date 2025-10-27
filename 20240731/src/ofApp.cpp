#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(10, 10, 10); // Darker background for contrast
    ofSetCircleResolution(100);
    time = 0.0;
    noiseOffset = ofRandom(1000);
    numLayers = 6; // Increased number of layers for more depth
    numPoints = 360; // Increased number of points for smoother curves
    baseRadius = 300; // Base radius for central pattern
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    noiseOffset += 0.02;

    points.clear();
    colors.clear();
    for (int layer = 0; layer < numLayers; layer++) {
        for (int i = 0; i < numPoints; i++) {
            float angle = ofMap(i, 0, numPoints, 0, TWO_PI);
            float radius = baseRadius + layer * 30 + ofMap(ofNoise(noiseOffset + i * 0.1, time * 0.1 + layer * 0.3), 0, 1, -60, 60);
            float x = ofGetWidth() / 2 + radius * cos(angle + layer * 0.05 * sin(time * 0.5));
            float y = ofGetHeight() / 2 + radius * sin(angle + layer * 0.05 * cos(time * 0.5));
            points.push_back(ofVec2f(x, y));

            ofColor color;
            float hue = ofMap(layer, 0, numLayers, 180, 220); // Subtle blue palette
            color.setHsb(hue, 200, 255, ofMap(sin(time + i * 0.1 + layer * 0.2), -1, 1, 100, 200));
            colors.push_back(color);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    drawGradientBackground();

    ofNoFill();
    for (int layer = 0; layer < numLayers; layer++) {
        for (int i = 0; i < numPoints; i++) {
            int index = layer * numPoints + i;
            ofSetColor(colors[index % colors.size()]);
            ofSetLineWidth(ofMap(sin(time + index * 0.1), -1, 1, 1, 3));

            int nextIndex = (index + 1) % points.size();
            int controlIndex1 = (index + numPoints / 3) % points.size();
            int controlIndex2 = (index + 2 * numPoints / 3) % points.size();

            ofDrawBezier(points[index].x, points[index].y,
                         points[controlIndex1].x, points[controlIndex1].y,
                         points[controlIndex2].x, points[controlIndex2].y,
                         points[nextIndex].x, points[nextIndex].y);
        }
    }

    drawShapes();
}

// Function to draw gradient background
void ofApp::drawGradientBackground() {
    ofColor color1(10, 10, 20);
    ofColor color2(0, 0, 0);
    ofBackgroundGradient(color1, color2, OF_GRADIENT_CIRCULAR);
}

// Function to draw enhanced shapes
void ofApp::drawShapes() {
    // Dynamic elements like circles
    ofSetColor(220, 220, 220, 100); // Subtle gray circles with transparency
    for (int layer = 0; layer < numLayers; layer++) {
        for (int i = 0; i < numPoints; i++) {
            int index = layer * numPoints + i;
            float radius = 3 + layer;
            ofDrawCircle(points[index].x, points[index].y, radius * sin(time + layer * 0.5));
        }
    }

    // Smooth lines with dynamic transparency
    ofSetColor(180, 180, 180, 50); // Subtle gray lines with more transparency
    ofSetLineWidth(1);
    for (int layer = 0; layer < numLayers; layer++) {
        ofBeginShape();
        for (int i = 0; i < numPoints; i++) {
            int index = layer * numPoints + i;
            ofVertex(points[index].x, points[index].y);
        }
        ofEndShape(true);
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
