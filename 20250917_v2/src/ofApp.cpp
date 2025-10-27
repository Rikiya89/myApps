#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    numPoints = 200;
    noiseScale = 0.008f;
    speed = 0.015f;
    time = 0;
    showWireframe = false;
    trailLength = 15;
    globalPulse = 0;
    
    generatePoints();
    
    cam.setDistance(450);
    cam.setPosition(0, 0, 450);
    cam.lookAt(ofVec3f(0, 0, 0));
    
    ofSetSphereResolution(12);
    ofSetCircleResolution(16);
}

void ofApp::update(){
    time += speed;
    globalPulse = sin(time * 2.0) * 0.5 + 0.5;
    updatePoints();
    createMesh();
}

void ofApp::draw(){
    ofEnableAlphaBlending();
    
    cam.begin();
    
    ofPushMatrix();
    ofRotateYDeg(time * 8);
    ofRotateXDeg(sin(time * 0.3) * 15);
    ofRotateZDeg(cos(time * 0.4) * 5);
    
    drawTrails();
    
    if(showWireframe) {
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(255, 80 + globalPulse * 100);
        mesh.drawWireframe();
    } else {
        ofFill();
        ofSetColor(255, 60 + globalPulse * 80);
        mesh.draw();
    }
    
    drawParticles();
    drawConnections();
    
    ofPopMatrix();
    
    cam.end();
    
    ofSetColor(255, 180);
//    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 2), 10, 20);
//    ofDrawBitmapString("Points: " + ofToString(numPoints), 10, 40);
//    ofDrawBitmapString("Press 'w' for wireframe", 10, 60);
//    ofDrawBitmapString("Press 'r' to regenerate", 10, 80);
//    ofDrawBitmapString("Press 't' for trails", 10, 100);
}

void ofApp::generatePoints(){
    points.clear();
    velocities.clear();
    noiseSeeds.clear();
    trails.clear();
    particleLife.clear();
    particleSize.clear();
    
    for(int i = 0; i < numPoints; i++) {
        float radius = ofRandom(50, 180);
        float theta = ofRandom(TWO_PI);
        float phi = ofRandom(PI);
        
        float x = radius * sin(phi) * cos(theta);
        float y = radius * sin(phi) * sin(theta);
        float z = radius * cos(phi);
        
        points.push_back(ofVec3f(x, y, z));
        velocities.push_back(ofVec3f(0, 0, 0));
        noiseSeeds.push_back(ofRandom(1000));
        trails.push_back(vector<ofVec3f>());
        particleLife.push_back(ofRandom(1.0));
        particleSize.push_back(ofRandom(0.5, 3.0));
    }
}

void ofApp::updatePoints(){
    for(int i = 0; i < points.size(); i++) {
        float seed = noiseSeeds[i];
        
        float flowX = ofNoise(points[i].x * noiseScale, points[i].y * noiseScale, time + seed) - 0.5;
        float flowY = ofNoise(points[i].y * noiseScale, points[i].z * noiseScale, time + seed + 100) - 0.5;
        float flowZ = ofNoise(points[i].z * noiseScale, points[i].x * noiseScale, time + seed + 200) - 0.5;
        
        float wave = sin(time * 3 + i * 0.1) * 0.02;
        velocities[i] += ofVec3f(flowX, flowY, flowZ) * (0.08 + wave);
        velocities[i] *= 0.98;
        
        trails[i].push_back(points[i]);
        if(trails[i].size() > trailLength) {
            trails[i].erase(trails[i].begin());
        }
        
        points[i] += velocities[i];
        
        particleLife[i] += 0.02;
        if(particleLife[i] > 1.0) particleLife[i] = 0.0;
        
        float attraction = 0.0008 + sin(time + i * 0.05) * 0.0002;
        ofVec3f toCenter = ofVec3f(0, 0, 0) - points[i];
        velocities[i] += toCenter * attraction;
        
        float maxDist = 220;
        if(points[i].length() > maxDist) {
            points[i] = points[i].getNormalized() * maxDist;
        }
    }
}

void ofApp::createMesh(){
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for(int i = 0; i < points.size(); i++) {
        mesh.addVertex(points[i]);
        
        float brightness = ofMap(sin(time + i * 0.1), -1, 1, 50, 255);
        mesh.addColor(ofColor(brightness));
    }
    
    for(int i = 0; i < points.size() - 2; i += 2) {
        for(int j = i + 1; j < min(i + 20, (int)points.size() - 1); j++) {
            for(int k = j + 1; k < min(j + 10, (int)points.size()); k++) {
                float dist1 = points[i].distance(points[j]);
                float dist2 = points[j].distance(points[k]);
                float dist3 = points[k].distance(points[i]);
                
                if(dist1 < 60 && dist2 < 60 && dist3 < 60) {
                    mesh.addTriangle(i, j, k);
                }
            }
        }
    }
}

void ofApp::keyPressed(int key){
    switch(key) {
        case 'w':
        case 'W':
            showWireframe = !showWireframe;
            break;
        case 'r':
        case 'R':
            generatePoints();
            break;
        case '+':
            numPoints = min(numPoints + 25, 500);
            generatePoints();
            break;
        case '-':
            numPoints = max(numPoints - 25, 50);
            generatePoints();
            break;
        case 't':
        case 'T':
            trailLength = (trailLength == 15) ? 0 : 15;
            break;
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::drawTrails(){
    ofSetLineWidth(1);
    for(int i = 0; i < trails.size(); i += 2) {
        if(trails[i].size() > 1) {
            for(int j = 0; j < trails[i].size() - 1; j++) {
                float alpha = ofMap(j, 0, trails[i].size() - 1, 20, 120);
                alpha *= (0.7 + sin(particleLife[i] * TWO_PI) * 0.3);
                ofSetColor(255, alpha);
                ofDrawLine(trails[i][j], trails[i][j + 1]);
            }
        }
    }
}

void ofApp::drawParticles(){
    for(int i = 0; i < points.size(); i += 2) {
        float life = particleLife[i];
        float pulse = sin(life * TWO_PI * 2) * 0.5 + 0.5;
        float size = particleSize[i] * (1.0 + pulse * 0.8) * (0.8 + globalPulse * 0.4);
        float brightness = 150 + pulse * 80 + globalPulse * 50;
        
        ofSetColor(255, brightness);
        ofDrawSphere(points[i], size);
        
        if(i % 4 == 0) {
            ofSetColor(255, brightness * 0.3);
            ofDrawSphere(points[i], size * 2.5);
        }
    }
}

void ofApp::drawConnections(){
    ofSetLineWidth(0.8);
    for(int i = 0; i < points.size() - 1; i += 2) {
        for(int j = i + 1; j < min(i + 15, (int)points.size()); j++) {
            float dist = points[i].distance(points[j]);
            if(dist < 80) {
                float alpha = ofMap(dist, 0, 80, 100, 10);
                alpha *= (0.6 + sin(time * 2 + i * 0.1) * 0.4);
                ofSetColor(255, alpha);
                ofDrawLine(points[i], points[j]);
            }
        }
    }
}
