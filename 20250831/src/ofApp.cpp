#include "ofApp.h"

//--------------------------------------------------------------
// Particle Implementation
//--------------------------------------------------------------
Particle::Particle(float x, float y) {
    pos.set(x, y);
    vel.set(ofRandom(-2, 2), ofRandom(-2, 2));
    acc.set(0, 0);
    maxLife = ofRandom(100, 200);
    lifespan = maxLife;
    size = ofRandom(1, 4);
}

void Particle::update() {
    vel += acc;
    pos += vel;
    acc *= 0;
    lifespan -= 1.0;
    
    // Wrap around edges
    if (pos.x < 0) pos.x = ofGetWidth();
    if (pos.x > ofGetWidth()) pos.x = 0;
    if (pos.y < 0) pos.y = ofGetHeight();
    if (pos.y > ofGetHeight()) pos.y = 0;
}

void Particle::applyForce(ofVec2f force) {
    acc += force;
}

void Particle::draw() {
    float alpha = ofMap(lifespan, 0, maxLife, 0, 255);
    ofSetColor(255, alpha);
    ofDrawCircle(pos.x, pos.y, size);
}

bool Particle::isDead() {
    return lifespan <= 0;
}

//--------------------------------------------------------------
// ofApp Implementation
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofSetCircleResolution(32);
    
    // Initialize variables
    maxParticles = 500;
    angleOffset = 0;
    radiusOffset = 0;
    numShapes = 12;
    time = 0;
    noiseScale = 0.01;
    noiseSpeed = 0.005;
    visualMode = 0;
    showParticles = true;
    showGeometry = true;
    invertColors = false;
    
    // Setup FBO for trails
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    fbo.end();
    
    // Create initial particles
    for (int i = 0; i < 50; i++) {
        addParticle(ofGetWidth()/2, ofGetHeight()/2);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    time += noiseSpeed;
    angleOffset += 0.01;
    radiusOffset = sin(time * 2) * 50;
    
    // Update particles
    for (int i = particles.size() - 1; i >= 0; i--) {
        // Apply noise-based force
        float noiseAngle = ofNoise(particles[i].pos.x * noiseScale,
                                  particles[i].pos.y * noiseScale,
                                  time) * TWO_PI * 2;
        ofVec2f force(cos(noiseAngle) * 0.5, sin(noiseAngle) * 0.5);
        particles[i].applyForce(force);
        
        particles[i].update();
        
        if (particles[i].isDead()) {
            particles.erase(particles.begin() + i);
        }
    }
    
    // Add new particles periodically
    if (ofGetFrameNum() % 2 == 0 && particles.size() < maxParticles) {
        float x = ofGetWidth()/2 + cos(angleOffset * 3) * 200;
        float y = ofGetHeight()/2 + sin(angleOffset * 2) * 200;
        addParticle(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    // Draw to FBO for trail effect
    fbo.begin();
    
    // Fade background slightly for trails
    ofSetColor(invertColors ? 255 : 0, 5);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    // Set drawing color based on invert mode
    if (invertColors) {
        ofSetColor(0);
    } else {
        ofSetColor(255);
    }
    
    // Draw based on visual mode
    switch(visualMode) {
        case 0: // All elements
            if (showGeometry) drawGeometricPattern();
            if (showParticles) {
                for (auto& p : particles) {
                    p.draw();
                }
            }
            drawNoiseField();
            break;
            
        case 1: // Geometric only
            drawGeometricPattern();
            drawRadialBurst();
            break;
            
        case 2: // Particles only
            for (auto& p : particles) {
                p.draw();
            }
            break;
            
        case 3: // Noise field
            drawNoiseField();
            break;
    }
    
    fbo.end();
    
    // Draw FBO to screen
    ofSetColor(255);
    fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::addParticle(float x, float y) {
    particles.push_back(Particle(x, y));
}

//--------------------------------------------------------------
void ofApp::drawGeometricPattern() {
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    for (int i = 0; i < numShapes; i++) {
        float angle = (TWO_PI / numShapes) * i + angleOffset;
        float radius = 100 + radiusOffset + sin(angle * 3 + time) * 30;
        
        ofPushMatrix();
        ofRotate(ofRadToDeg(angle));
        ofTranslate(radius, 0);
        
        // Draw rotating shapes
        float shapeSize = 20 + sin(time * 2 + i) * 10;
        ofNoFill();
        ofSetLineWidth(1);
        
        // Alternating shapes
        if (i % 2 == 0) {
            ofDrawRectangle(-shapeSize/2, -shapeSize/2, shapeSize, shapeSize);
        } else {
            ofDrawTriangle(0, -shapeSize/2,
                          -shapeSize/2, shapeSize/2,
                          shapeSize/2, shapeSize/2);
        }
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawNoiseField() {
    ofSetLineWidth(0.5);
    int step = 30;
    
    for (int x = 0; x < ofGetWidth(); x += step) {
        for (int y = 0; y < ofGetHeight(); y += step) {
            float noiseVal = ofNoise(x * noiseScale, y * noiseScale, time);
            float angle = noiseVal * TWO_PI * 2;
            float length = noiseVal * 20;
            
            ofPushMatrix();
            ofTranslate(x, y);
            ofRotate(ofRadToDeg(angle));
            
            float alpha = noiseVal * 100;
            ofSetColor(invertColors ? 0 : 255, alpha);
            ofDrawLine(0, 0, length, 0);
            
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawRadialBurst() {
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    int numLines = 36;
    for (int i = 0; i < numLines; i++) {
        float angle = (TWO_PI / numLines) * i;
        float length = 50 + sin(time * 3 + angle * 2) * 30;
        
        ofSetLineWidth(0.5 + sin(time + i) * 0.5);
        ofSetColor(invertColors ? 0 : 255, 150);
        
        ofDrawLine(0, 0,
                  cos(angle) * length,
                  sin(angle) * length);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::clearBackground() {
    fbo.begin();
    ofClear(invertColors ? 255 : 0, 255);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch(key) {
        case ' ':
            visualMode = (visualMode + 1) % 4;
            break;
        case 'c':
        case 'C':
            clearBackground();
            break;
        case 'i':
        case 'I':
            invertColors = !invertColors;
            clearBackground();
            break;
        case 'p':
        case 'P':
            showParticles = !showParticles;
            break;
        case 'g':
        case 'G':
            showGeometry = !showGeometry;
            break;
        case 'r':
        case 'R':
            particles.clear();
            setup();
            break;
        case 's':
        case 'S':
            ofSaveFrame();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    noiseScale = ofMap(x, 0, ofGetWidth(), 0.001, 0.05);
    noiseSpeed = ofMap(y, 0, ofGetHeight(), 0.001, 0.02);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    addParticle(x, y);
    addParticle(x + ofRandom(-10, 10), y + ofRandom(-10, 10));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    for (int i = 0; i < 10; i++) {
        addParticle(x + ofRandom(-20, 20), y + ofRandom(-20, 20));
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    fbo.allocate(w, h, GL_RGBA);
    clearBackground();
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    
}
