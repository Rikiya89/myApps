#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(41, 48, 57);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    palette.push_back(ofColor::fromHex(0x362d78));
    palette.push_back(ofColor::fromHex(0x523fa3));
    palette.push_back(ofColor::fromHex(0x916ccc));
    palette.push_back(ofColor::fromHex(0xbda1e5));
    palette.push_back(ofColor::fromHex(0xc8c0e9));
    palette.push_back(ofColor::fromHex(0x84bae7));
    palette.push_back(ofColor::fromHex(0x516ad4));
    palette.push_back(ofColor::fromHex(0x333f87));
    palette.push_back(ofColor::fromHex(0x293039));
    palette.push_back(ofColor::fromHex(0x283631));
    
    time = 0;
    maxParticles = 800;
    
    createFlowField();
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01f;
    
    createFlowField();
    
    if(particles.size() < maxParticles) {
        for(int i = 0; i < 3; i++) {
            addParticle(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        }
    }
    
    updateParticles();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(41, 48, 57, 255);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    for(int i = 0; i < 3; i++) {
        ofPushMatrix();
        ofRotateDeg(time * 10 + i * 120);
        ofScale(1 + sin(time + i) * 0.3);
        
        drawParticles();
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
    ofSetColor(255, 255, 255, 100);
    float waveHeight = 50;
    ofBeginShape();
    ofNoFill();
    for(int x = 0; x < ofGetWidth(); x += 5) {
        float y = ofGetHeight()/2 + sin(x * 0.01 + time * 2) * waveHeight * sin(time * 0.5);
        ofVertex(x, y);
    }
    ofEndShape();
}

//--------------------------------------------------------------
void ofApp::addParticle(float x, float y) {
    Particle p;
    p.position.set(x - ofGetWidth()/2, y - ofGetHeight()/2);
    p.velocity.set(ofRandom(-1, 1), ofRandom(-1, 1));
    p.size = ofRandom(2, 8);
    p.life = 1.0f;
    p.maxLife = ofRandom(100, 300);
    p.colorIndex = (int)ofRandom(palette.size());
    p.phase = ofRandom(TWO_PI);
    particles.push_back(p);
}

//--------------------------------------------------------------
void ofApp::updateParticles() {
    for(int i = particles.size() - 1; i >= 0; i--) {
        Particle& p = particles[i];
        
        int fieldX = (int)ofMap(p.position.x + ofGetWidth()/2, 0, ofGetWidth(), 0, 63, true);
        int fieldY = (int)ofMap(p.position.y + ofGetHeight()/2, 0, ofGetHeight(), 0, 63, true);
        
        ofVec2f force = flowField[fieldX][fieldY];
        p.velocity += force * 0.1f;
        p.velocity *= 0.98f;
        
        p.position += p.velocity;
        
        p.life -= 1.0f/p.maxLife;
        p.phase += 0.05f;
        
        if(p.life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles() {
    for(const auto& p : particles) {
        float alpha = p.life * 255 * (0.5 + 0.5 * sin(p.phase));
        ofColor color = palette[p.colorIndex];
        color.a = alpha;
        
        ofSetColor(color);
        
        ofPushMatrix();
        ofTranslate(p.position.x, p.position.y);
        
        float breathe = 1 + sin(time * 3 + p.phase) * 0.3;
        ofScale(breathe);
        
        ofDrawCircle(0, 0, p.size * p.life);
        
        ofSetColor(color.r, color.g, color.b, alpha * 0.3);
        ofDrawCircle(0, 0, p.size * p.life * 2);
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::createFlowField() {
    float noiseScale = 0.01f;
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 64; y++) {
            float angle = ofNoise(x * noiseScale, y * noiseScale, time) * TWO_PI * 2;
            flowField[x][y].set(cos(angle), sin(angle));
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        particles.clear();
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
    for(int i = 0; i < 5; i++) {
        addParticle(x + ofRandom(-20, 20), y + ofRandom(-20, 20));
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(int i = 0; i < 10; i++) {
        addParticle(x + ofRandom(-30, 30), y + ofRandom(-30, 30));
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
