#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetCircleResolution(100);
    ofBackground(0);

    // -----------------------------
    // Spirograph / Polygons Setup
    R = 120;       // fixed circle radius
    r = 40;        // rolling circle radius
    d = 70;        // offset from rolling circle center
    thetaStep = 0.01f;
    time = 0.0f;

    RADIUS_STEP = 60.0f;
    layerRotationOffsets.resize(NUM_LAYERS);
    for(int i = 0; i < NUM_LAYERS; i++){
        layerRotationOffsets[i] = ofRandom(360.0f);
    }

    // -----------------------------
    // FBO allocations (match window size)
    mainFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    blurFbo1.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    blurFbo2.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    // Clear them once
    mainFBO.begin();    ofClear(0,0,0,0);   mainFBO.end();
    blurFbo1.begin();   ofClear(0,0,0,0);   blurFbo1.end();
    blurFbo2.begin();   ofClear(0,0,0,0);   blurFbo2.end();

    // -----------------------------
    // Shaders (place .frag files in bin/data/shaders/)
    blurX.load("shaders/blurHorizontal");
    blurY.load("shaders/blurVertical");

    // -----------------------------
    // Parameters
    blurAmount   = 4.0;     // tweak this to get more or less glow
    globalScale  = 1.2f;    // scale the entire scene slightly
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01f;

    // Use noise to animate spirograph parameters
    float noise1 = ofNoise(time * 0.3f);
    float noise2 = ofNoise(time * 0.4f + 100.0f);
    float noise3 = ofNoise(time * 0.5f + 200.0f);

    // Map noise to a range
    r = ofMap(noise1, 0, 1, 35, 45);
    d = ofMap(noise2, 0, 1, 50, 80);
    R = ofMap(noise3, 0, 1, 110, 130);

    // Animate each polygon layer’s rotation
    for(int i = 0; i < NUM_LAYERS; i++){
        float layerNoise = ofNoise(time + i * 0.1f);
        layerRotationOffsets[i] += ofMap(layerNoise, 0, 1, 0.2, 0.5);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 1) Draw main scene into mainFBO
    mainFBO.begin();
    ofClear(0,0,0,0);
    drawScene();
    mainFBO.end();

    // 2) Blur passes
    applyBlur();

    // 3) Draw final result to screen
    ofBackground(0);
    ofSetColor(255);

    // Draw original scene
    mainFBO.draw(0,0);

    // Add blurred version on top
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    blurFbo2.draw(0,0);
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::drawScene(){
    // Center and scale
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofScale(globalScale, globalScale);

    // Draw spirograph
    drawSpirograph();

    // Draw mandala polygons
    drawPolygons();

    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawSpirograph(){
    ofSetColor(255);
    ofNoFill();

    // Multiple revolutions
    ofBeginShape();
    for(float theta = 0; theta < TWO_PI * 6; theta += thetaStep){
        float x = (R + r) * cos(theta) - d * cos(((R + r)/r) * theta);
        float y = (R + r) * sin(theta) - d * sin(((R + r)/r) * theta);
        ofVertex(x, y);
    }
    ofEndShape();
}

//--------------------------------------------------------------
void ofApp::drawPolygons(){
    ofSetColor(255);
    ofNoFill();
    
    for(int layer = 0; layer < NUM_LAYERS; ++layer) {
        ofPushMatrix();
        
        float rotationAngle = layerRotationOffsets[layer];
        ofRotateDeg(rotationAngle);

        float currentRadius = (layer + 1) * RADIUS_STEP;
        
        // Draw polygon
        ofBeginShape();
        for(int i = 0; i < SIDES_PER_LAYER; ++i){
            float angle = TWO_PI * i / (float)SIDES_PER_LAYER;
            float x = currentRadius * cos(angle);
            float y = currentRadius * sin(angle);
            ofVertex(x, y);
        }
        ofEndShape(true);
        
        // Optional circle at center
        ofDrawCircle(0, 0, currentRadius * 0.15);

        // Radial lines
        for(int i = 0; i < SIDES_PER_LAYER; ++i){
            float angle = TWO_PI * i / (float)SIDES_PER_LAYER;
            float x = currentRadius * cos(angle);
            float y = currentRadius * sin(angle);
            ofDrawLine(0, 0, x, y);
        }

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::applyBlur(){
    // 1) Horizontal blur pass
    blurFbo1.begin();
    ofClear(0,0,0,0);
    blurX.begin();
    blurX.setUniform1f("blurAmount", blurAmount);
    mainFBO.draw(0,0);
    blurX.end();
    blurFbo1.end();

    // 2) Vertical blur pass
    blurFbo2.begin();
    ofClear(0,0,0,0);
    blurY.begin();
    blurY.setUniform1f("blurAmount", blurAmount);
    blurFbo1.draw(0,0);
    blurY.end();
    blurFbo2.end();
}
