#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(10);  // Set a very dark background
    ofSetCircleResolution(100);  // High resolution for smoother circles
    ofSetFrameRate(60);  // Smooth animation at 60 FPS
    ofEnableSmoothing();  // Enable smoothing to reduce visual artifacts
    ofEnableAlphaBlending();  // Enable alpha blending for semi-transparent drawing
}

//--------------------------------------------------------------
void ofApp::update(){
    // Additional interactive updates can be handled here
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor = ofColor(85, 78, 68);  // Central color for the background gradient
    ofColor edgeColor(0, 0, 0);  // Edge color for the background gradient
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);  // Circular gradient background

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);  // Center the drawing origin
    int numLayers = 5;  // Number of concentric layers
    float initialRadius = 80;  // Initial radius
    float scaleFactor = 2.0;  // Scaling factor
    float time = ofGetElapsedTimef();  // Time for dynamic effects

    for (int layer = 0; layer < numLayers; layer++) {
        float layerTimeOffset = layer * 0.15;
        float radius = initialRadius * pow(scaleFactor, layer) * (1 + 0.1 * sin(0.5 * time + layerTimeOffset));
        int numberOfCircles = 12 + 12 * layer;
        float angleStep = TWO_PI / numberOfCircles;

        for (int i = 0; i < numberOfCircles; i++) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            ofColor circleColor;
            float hue = fmod(255 * (layer / float(numLayers)) + 255 * 0.6 * sin(time * 0.2 + i), 255);
            circleColor.setHsb(hue, 200, 255, 180);
            ofSetColor(circleColor);
            float circleSize = initialRadius * (1 + 0.5 * sin(time + angle));
            ofDrawCircle(x, y, circleSize);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Key interactions can control aspects like number of layers, colors, etc.
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    // Use mouse position to affect the pattern dynamically
    float widthRatio = float(x) / ofGetWidth();
    initialRadius = 50 + 100 * widthRatio;  // Adjust the base circle size based on mouse X position
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
