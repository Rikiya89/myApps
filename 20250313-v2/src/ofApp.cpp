#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    
    // Generate a random grid pattern with moving paths
    grid.resize(ROWS, vector<int>(COLS, 0));
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            grid[y][x] = ofRandom(5); // Random color index
        }
    }
    
    // Set initial positions
    playerPos.set(ofRandom(COLS), ofRandom(ROWS));
    enemyPos.set(ofRandom(COLS), ofRandom(ROWS));
    timeElapsed = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    timeElapsed += ofGetLastFrameTime();
    
    // Move player and enemy in a sine-wave motion
    playerPos.x = (sin(timeElapsed * 2.0) * (COLS / 4)) + (COLS / 2);
    playerPos.y = (cos(timeElapsed * 2.0) * (ROWS / 4)) + (ROWS / 2);
    
    enemyPos.x = (cos(timeElapsed * 1.5) * (COLS / 4)) + (COLS / 2);
    enemyPos.y = (sin(timeElapsed * 1.5) * (ROWS / 4)) + (ROWS / 2);
    
    // Animate the grid, shifting colors over time
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            grid[y][x] = (grid[y][x] + 1) % 5;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw the animated grid with shifting colors
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            ofSetColor(colors[(grid[y][x] + (int)(timeElapsed * 2)) % 5]);
            ofDrawRectangle(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
        }
    }
    
    // Draw player with glowing effect
    ofSetColor(ofColor::yellow, 200);
    ofDrawCircle(playerPos.x * GRID_SIZE + GRID_SIZE / 2, playerPos.y * GRID_SIZE + GRID_SIZE / 2, GRID_SIZE);
    
    // Draw enemy with pulsing effect
    float enemySize = GRID_SIZE / 2 + sin(timeElapsed * 3) * 5;
    ofSetColor(ofColor::magenta);
    ofDrawCircle(enemyPos.x * GRID_SIZE + GRID_SIZE / 2, enemyPos.y * GRID_SIZE + GRID_SIZE / 2, enemySize);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ }
void ofApp::keyReleased(int key){ }
void ofApp::mouseMoved(int x, int y ){ }
void ofApp::mouseDragged(int x, int y, int button){ }
void ofApp::mousePressed(int x, int y, int button){ }
void ofApp::mouseReleased(int x, int y, int button){ }
void ofApp::mouseEntered(int x, int y){ }
void ofApp::mouseExited(int x, int y){ }
void ofApp::windowResized(int w, int h){ }
void ofApp::gotMessage(ofMessage msg){ }
void ofApp::dragEvent(ofDragInfo dragInfo){ }
