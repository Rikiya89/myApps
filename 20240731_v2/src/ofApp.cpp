#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    hueOffset = 0;
    showBezier = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    // Generate new points and update the list
    if (ofGetFrameNum() % 2 == 0) {
        ofPoint point(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        points.push_back(point);
        // Pastel colors with lower saturation and higher brightness
        colors.push_back(ofColor::fromHsb(fmod(hueOffset, 255), 120, 255));
        sizes.push_back(ofRandom(2, 5));
        velocities.push_back(ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1)));
        trails.push_back(vector<ofPoint>()); // Initialize trail for new point
        hueOffset += 0.5;  // Slowly change the hue
    }

    // Update points positions with velocity and record trails
    for (size_t i = 0; i < points.size(); i++) {
        points[i] += velocities[i];

        // Bounce off the walls
        if (points[i].x < 0 || points[i].x > ofGetWidth()) velocities[i].x *= -1;
        if (points[i].y < 0 || points[i].y > ofGetHeight()) velocities[i].y *= -1;

        // Record the trail
        trails[i].push_back(points[i]);
        if (trails[i].size() > 50) { // Limit trail length
            trails[i].erase(trails[i].begin());
        }
    }

    // Remove old points to keep the pattern dynamic
    if (points.size() > 1000) {
        points.erase(points.begin());
        colors.erase(colors.begin());
        sizes.erase(sizes.begin());
        velocities.erase(velocities.begin());
        trails.erase(trails.begin());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    
    // Draw subtle background gradient
    ofMesh gradientMesh;
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    gradientMesh.addVertex(ofPoint(ofGetWidth() / 2, ofGetHeight() / 2));
    gradientMesh.addColor(ofColor(0, 0, 0, 0));
    gradientMesh.addVertex(ofPoint(0, 0));
    gradientMesh.addColor(ofColor(50, 50, 50, 100));
    gradientMesh.addVertex(ofPoint(ofGetWidth(), 0));
    gradientMesh.addColor(ofColor(50, 50, 50, 100));
    gradientMesh.addVertex(ofPoint(ofGetWidth(), ofGetHeight()));
    gradientMesh.addColor(ofColor(50, 50, 50, 100));
    gradientMesh.addVertex(ofPoint(0, ofGetHeight()));
    gradientMesh.addColor(ofColor(50, 50, 50, 100));
    gradientMesh.draw();
    
    for (size_t i = 0; i < points.size(); i++) {
        // Draw particle trails
        for (size_t t = 0; t < trails[i].size(); t++) {
            float alpha = ofMap(t, 0, trails[i].size(), 0, 255);
            ofSetColor(colors[i], alpha);
            ofDrawCircle(trails[i][t], sizes[i] * ofMap(t, 0, trails[i].size(), 0.1, 1));
        }

        // Draw bezier curves if enabled
        if (showBezier) {
            for (size_t j = i + 1; j < points.size(); j++) {
                float distance = ofDist(points[i].x, points[i].y, points[j].x, points[j].y);
                if (distance < 150) {
                    ofSetColor(colors[i], 128);  // Semi-transparent lines
                    ofDrawBezier(points[i].x, points[i].y,
                                 (points[i].x + points[j].x) / 2, points[i].y - 50,
                                 (points[i].x + points[j].x) / 2, points[j].y + 50,
                                 points[j].x, points[j].y);
                }
            }
        }

        // Draw circles with varying sizes and alpha
        ofSetColor(colors[i], 200);
        ofDrawCircle(points[i], sizes[i]);
    }
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'b') {
        showBezier = !showBezier; // Toggle bezier curves on/off
    }
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
