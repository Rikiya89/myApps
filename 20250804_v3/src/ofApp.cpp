#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);
    ofEnableSmoothing();
    ofSetCircleResolution(64);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    goldenRatio = 1.618033988749;
    time = 0;
    currentColorIndex = 0;
    colorTransition = 0;
    breathingPhase = 0;
    pulseIntensity = 1.0;
    masterRotation = 0;
    rotationSpeed = 0.1;
    scaleOscillation = 0;
    centerPulse = 0;
    maxParticles = 300;
    
    colorPalette.push_back(ofColor(255, 100, 100));  // Soft Red
    colorPalette.push_back(ofColor(255, 180, 100));  // Warm Orange
    colorPalette.push_back(ofColor(255, 255, 120));  // Golden Yellow
    colorPalette.push_back(ofColor(120, 255, 120));  // Fresh Green
    colorPalette.push_back(ofColor(100, 255, 255));  // Sky Cyan
    colorPalette.push_back(ofColor(120, 120, 255));  // Royal Blue
    colorPalette.push_back(ofColor(255, 120, 255));  // Vivid Magenta
    colorPalette.push_back(ofColor(255, 230, 100));  // Bright Gold
    colorPalette.push_back(ofColor(200, 150, 255));  // Lavender
    colorPalette.push_back(ofColor(150, 255, 200));  // Mint
    
    for(int i = 0; i < 12; i++){
        FlowerPattern pattern;
        float angle = (TWO_PI / 12) * i;
        float distance = 200 + (i % 3) * 80;
        pattern.position = ofVec2f(cos(angle) * distance, sin(angle) * distance);
        pattern.radius = 40 + (i % 4) * 15;
        pattern.rotation = 0;
        pattern.rotationSpeed = ofRandom(-1, 1) * 0.1;
        pattern.scalePhase = ofRandom(TWO_PI);
        pattern.alpha = 80 + i * 10;
        pattern.colorIndex = i % colorPalette.size();
        flowerPatterns.push_back(pattern);
    }
}

void ofApp::update(){
    time += 0.008;
    colorTransition += 0.005;
    breathingPhase += 0.015;
    masterRotation += rotationSpeed;
    scaleOscillation += 0.01;
    centerPulse += 0.02;
    
    if(colorTransition >= 1.0){
        colorTransition = 0;
        currentColorIndex = (currentColorIndex + 1) % colorPalette.size();
    }
    
    pulseIntensity = 0.8 + sin(breathingPhase) * 0.3;
    
    updateFlowerPatterns();
    updateParticles();
    
    if(particles.size() < maxParticles && ofRandom(1.0) < 0.2){
        ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
        float angle = ofRandom(TWO_PI);
        float distance = ofRandom(50, 300);
        ofVec2f pos = center + ofVec2f(cos(angle) * distance, sin(angle) * distance);
        ofColor particleColor = colorPalette[ofRandom(colorPalette.size())];
        addParticle(pos, particleColor);
    }
}

void ofApp::draw(){
    ofBackground(5, 5, 10, 30);
    
    float centerX = ofGetWidth() / 2;
    float centerY = ofGetHeight() / 2;
    
    drawParticles();
    
    ofPushMatrix();
    ofTranslate(centerX, centerY);
    
    float mainScale = pulseIntensity * (1.0 + sin(centerPulse) * 0.2);
    ofScale(mainScale, mainScale);
    ofRotate(masterRotation * 5);
    
    for(int layer = 0; layer < 7; layer++){
        float layerRadius = (layer + 1) * 45 * goldenRatio / 4;
        float layerScale = 1.0 + sin(scaleOscillation + layer * 0.5) * 0.15;
        float alpha = (120 - layer * 15) * pulseIntensity;
        
        int colorIndex1 = (currentColorIndex + layer) % colorPalette.size();
        int colorIndex2 = (currentColorIndex + layer + 1) % colorPalette.size();
        
        ofColor color1 = colorPalette[colorIndex1];
        ofColor color2 = colorPalette[colorIndex2];
        ofColor currentColor = color1.lerp(color2, colorTransition);
        currentColor.a = alpha;
        
        ofSetColor(currentColor);
        
        ofPushMatrix();
        ofScale(layerScale, layerScale);
        drawFlowerOfLife(0, 0, layerRadius, 3, layer * 15 + time * 8);
        ofPopMatrix();
        
        ofRotate(360.0 / goldenRatio + sin(time + layer) * 2);
    }
    
    ofPopMatrix();
    
    drawFlowerPatterns();
    
    for(int ring = 0; ring < 3; ring++){
        int numElements = 6 + ring * 3;
        for(int i = 0; i < numElements; i++){
            float angle = (TWO_PI / numElements) * i + time * (0.1 + ring * 0.05);
            float distance = 250 + ring * 120 + sin(time * 0.5 + i + ring) * 40;
            float x = centerX + cos(angle) * distance;
            float y = centerY + sin(angle) * distance;
            
            float elementScale = 0.6 + sin(scaleOscillation + i * 0.3 + ring) * 0.3;
            float elementRotation = angle * 57.2958 + time * 15;
            
            ofColor spiralColor = colorPalette[(currentColorIndex + i + ring) % colorPalette.size()];
            spiralColor.a = 80 + sin(breathingPhase + i * 0.2) * 30;
            ofSetColor(spiralColor);
            
            drawFlowerOfLife(x, y, 25 * elementScale, 2, elementRotation);
        }
    }
}

void ofApp::drawFlowerOfLife(float centerX, float centerY, float radius, int depth, float rotation){
    if(depth <= 0) return;
    
    ofPushMatrix();
    ofTranslate(centerX, centerY);
    ofRotate(rotation);
    
    float currentAlpha = ofGetStyle().color.a;
    drawCircle(0, 0, radius, currentAlpha);
    
    if(depth > 1){
        for(int i = 0; i < 6; i++){
            float angle = (TWO_PI / 6) * i;
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            drawCircle(x, y, radius, currentAlpha * 0.8);
            
            if(depth > 2){
                drawFlowerOfLife(x, y, radius / goldenRatio, depth - 1, rotation * 0.5);
            }
        }
    }
    
    ofPopMatrix();
}

void ofApp::drawCircle(float x, float y, float radius, float alpha){
    ofColor currentColor = ofGetStyle().color;
    currentColor.a = alpha;
    ofSetColor(currentColor);
    
    ofNoFill();
    ofSetLineWidth(1.5 + sin(time * 2) * 0.5);
    ofDrawCircle(x, y, radius);
}

void ofApp::updateParticles(){
    for(int i = particles.size() - 1; i >= 0; i--){
        particles[i].life -= 0.01;
        particles[i].position += particles[i].velocity;
        particles[i].velocity *= 0.995;
        particles[i].rotation += particles[i].rotationSpeed;
        particles[i].size *= 0.998;
        
        ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
        ofVec2f toCenter = center - particles[i].position;
        toCenter.normalize();
        particles[i].velocity += toCenter * 0.02;
        
        if(particles[i].life <= 0 || particles[i].size < 1){
            particles.erase(particles.begin() + i);
        }
    }
}

void ofApp::drawParticles(){
    for(auto& particle : particles){
        float alpha = (particle.life / particle.maxLife) * 100;
        ofColor particleColor = particle.color;
        particleColor.a = alpha;
        ofSetColor(particleColor);
        
        ofPushMatrix();
        ofTranslate(particle.position.x, particle.position.y);
        ofRotate(particle.rotation);
        ofScale(particle.size * 0.02, particle.size * 0.02);
        
        ofNoFill();
        ofSetLineWidth(1);
        for(int i = 0; i < 6; i++){
            float angle = (TWO_PI / 6) * i;
            float x = cos(angle) * 10;
            float y = sin(angle) * 10;
            ofDrawCircle(x, y, 8);
        }
        ofDrawCircle(0, 0, 10);
        
        ofPopMatrix();
    }
}

void ofApp::addParticle(ofVec2f pos, ofColor color){
    Particle particle;
    particle.position = pos;
    particle.velocity = ofVec2f(ofRandom(-2, 2), ofRandom(-2, 2));
    particle.life = 1.0;
    particle.maxLife = 1.0;
    particle.color = color;
    particle.size = ofRandom(20, 50);
    particle.rotation = 0;
    particle.rotationSpeed = ofRandom(-3, 3);
    particles.push_back(particle);
}

void ofApp::updateFlowerPatterns(){
    for(auto& pattern : flowerPatterns){
        pattern.rotation += pattern.rotationSpeed;
        pattern.scalePhase += 0.02;
        
        float breathingEffect = sin(breathingPhase + pattern.scalePhase) * 0.3;
        pattern.radius = (40 + breathingEffect * 20) + sin(pattern.scalePhase) * 10;
        pattern.alpha = 60 + sin(breathingPhase * 1.5 + pattern.scalePhase) * 40;
    }
}

void ofApp::drawFlowerPatterns(){
    float centerX = ofGetWidth() / 2;
    float centerY = ofGetHeight() / 2;
    
    for(auto& pattern : flowerPatterns){
        ofVec2f worldPos = pattern.position + ofVec2f(centerX, centerY);
        
        ofColor patternColor = colorPalette[pattern.colorIndex];
        patternColor.a = pattern.alpha;
        ofSetColor(patternColor);
        
        drawFlowerOfLife(worldPos.x, worldPos.y, pattern.radius, 2, pattern.rotation);
    }
}