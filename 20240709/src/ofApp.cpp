#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    noiseScale = 0.03;
    time = 0.0;
    
    // Initialize points
    int numPoints = 300;
    for(int i = 0; i < numPoints; i++){
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        points.push_back(ofVec2f(x, y));
        sizes.push_back(ofRandom(5, 20));
        colors.push_back(ofColor::fromHsb(ofRandom(255), 255, 255));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;
    
    for(int i = 0; i < points.size(); i++){
        float noiseValue = ofNoise(points[i].x * noiseScale, points[i].y * noiseScale, time);
        float angle = ofMap(noiseValue, 0, 1, 0, TWO_PI);
        
        float speed = 2.0;
        points[i].x += cos(angle) * speed;
        points[i].y += sin(angle) * speed;
        
        // Update sizes with Perlin noise
        sizes[i] = ofMap(ofNoise(points[i].x * noiseScale, points[i].y * noiseScale, time), 0, 1, 5, 20);
        
        // Keep points within window boundaries
        if(points[i].x < 0) points[i].x = ofGetWidth();
        if(points[i].x > ofGetWidth()) points[i].x = 0;
        if(points[i].y < 0) points[i].y = ofGetHeight();
        if(points[i].y > ofGetHeight()) points[i].y = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofNoFill();
    for(int i = 0; i < points.size(); i++){
        ofSetColor(colors[i]);
        ofDrawCircle(points[i].x, points[i].y, sizes[i]);
    }
    
    ofSetLineWidth(2);
    for(int i = 0; i < points.size(); i++){
        for(int j = i + 1; j < points.size(); j++){
            float dist = points[i].distance(points[j]);
            if(dist < 100){
                ofSetColor(255, ofMap(dist, 0, 100, 255, 0));
                ofDrawLine(points[i], points[j]);
            }
        }
    }
}
