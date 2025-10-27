// ofApp.cpp
#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(64);
    
    // Initialize parameters
    noiseScale = 0.008;
    noiseStrength = 1.0;
    attractorStrength = 0.5;
    particleSpawnRate = 0.3;
    time = 0;
    showTrails = true;
    showField = false;
    isPaused = false;
    colorMode = 0;
    backgroundOpacity = 10;
    
    attractorPos.set(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    // Initialize flow field
    flowField = new FlowField(ofGetWidth(), ofGetHeight(), 20);
    
    // Initialize FBOs
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    glowFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    
    fbo.begin();
    ofClear(0, 0, 0, 255);
    fbo.end();
    
    glowFbo.begin();
    ofClear(0, 0, 0, 255);
    glowFbo.end();
    
    // Create initial particles
    for (int i = 0; i < 200; i++) {
        particles.push_back(Particle(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
    }
}

void ofApp::update() {
    if (isPaused) return;
    
    time += 0.01;
    
    // Update flow field
    flowField->update(noiseScale, time);
    
    // Spawn new particles
    if (ofRandom(1) < particleSpawnRate) {
        createParticleBurst(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(1, 5));
    }
    
    // Update particles
    for (int i = particles.size() - 1; i >= 0; i--) {
        // Apply flow field force
        ofVec3f fieldForce = flowField->lookup(particles[i].pos);
        fieldForce *= noiseStrength;
        particles[i].applyForce(fieldForce);
        
        // Apply attractor force with smooth falloff
        ofVec3f attractorForce = attractorPos - particles[i].pos;
        float distance = attractorForce.length();
        attractorForce.normalize();
        
        // Smooth attraction curve
        float strength = 100 / (distance + 100);
        strength *= attractorStrength;
        attractorForce *= strength;
        particles[i].applyForce(attractorForce);
        
        // Apply slight repulsion from other nearby particles
        for (int j = 0; j < particles.size(); j++) {
            if (i != j) {
                ofVec3f repel = particles[i].pos - particles[j].pos;
                float d = repel.length();
                if (d < 50 && d > 0) {
                    repel.normalize();
                    repel *= (50 - d) / 50 * 0.5;
                    particles[i].applyForce(repel);
                }
            }
        }
        
        // Update particle color
        particles[i].color = getParticleColor(particles[i]);
        
        particles[i].update();
        
        // Remove dead particles
        if (particles[i].isDead()) {
            particles.erase(particles.begin() + i);
        }
    }
    
    // Limit particle count
    while (particles.size() > 1000) {
        particles.erase(particles.begin());
    }
}

void ofApp::draw() {
    // Draw to main FBO
    fbo.begin();
    
    // Fade background
    ofSetColor(0, 0, 0, backgroundOpacity);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    // Draw flow field visualization if enabled
    if (showField) {
        ofSetColor(100, 100, 150, 30);
        ofSetLineWidth(0.5);
        for (int i = 0; i < flowField->cols; i += 2) {
            for (int j = 0; j < flowField->rows; j += 2) {
                ofVec3f v = flowField->field[i][j];
                float x = i * flowField->scale;
                float y = j * flowField->scale;
                ofDrawLine(x, y, x + v.x * 10, y + v.y * 10);
            }
        }
        ofSetLineWidth(1);
    }
    
    // Draw particles
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (auto& p : particles) {
        p.display();
    }
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    fbo.end();
    
    // Apply glow effect
    glowFbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    fbo.draw(0, 0);
    glowFbo.end();
    
    // Draw everything to screen
    ofSetColor(255);
    fbo.draw(0, 0);
    
    // Draw attractor
    ofPushMatrix();
    ofTranslate(attractorPos.x, attractorPos.y);
    ofRotate(time * 50);
    
    // Animated attractor rings
    for (int i = 0; i < 3; i++) {
        float phase = time * 2 + i * TWO_PI / 3;
        float radius = 20 + sin(phase) * 10;
        ofSetColor(200, 150, 255, 100 - i * 30);
        ofNoFill();
        ofSetLineWidth(2 - i * 0.5);
        ofDrawCircle(0, 0, radius);
    }
    ofFill();
    ofPopMatrix();
    
    // Draw UI
    ofSetColor(255);
    stringstream ss;
    ss << "== Beautiful Generative Art ==\n";
    ss << "Particles: " << particles.size() << " | FPS: " << int(ofGetFrameRate()) << "\n\n";
    ss << "Controls:\n";
    ss << "[Mouse] Move attractor | [Drag] Create particle stream\n";
    ss << "[Space] Pause | [T] Trails opacity (" << backgroundOpacity << ")\n";
    ss << "[C] Color mode (" << colorMode << ") | [F] Show flow field\n";
    ss << "[1-5] Adjust parameters | [R] Reset | [S] Screenshot\n\n";
    ss << "Parameters:\n";
    ss << "[1] Noise Scale: " << noiseScale << "\n";
    ss << "[2] Flow Strength: " << noiseStrength << "\n";
    ss << "[3] Attractor: " << attractorStrength << "\n";
    ss << "[4] Spawn Rate: " << particleSpawnRate << "\n";
    
    ofDrawBitmapStringHighlight(ss.str(), 20, 20, ofColor(0, 150), ofColor(255));
}

void ofApp::keyPressed(int key) {
    switch(key) {
        case ' ':
            isPaused = !isPaused;
            break;
        case 't':
        case 'T':
            backgroundOpacity = (backgroundOpacity == 10) ? 5 : (backgroundOpacity == 5) ? 20 : 10;
            break;
        case 'c':
        case 'C':
            colorMode = (colorMode + 1) % 5;
            break;
        case 'f':
        case 'F':
            showField = !showField;
            break;
        case 'r':
        case 'R':
            particles.clear();
            fbo.begin();
            ofClear(0, 0, 0, 255);
            fbo.end();
            break;
        case 's':
        case 'S':
            ofSaveScreen("beautiful_generative_" + ofGetTimestampString() + ".png");
            break;
        case '1':
            noiseScale = ofClamp(noiseScale + ((ofGetKeyPressed(OF_KEY_SHIFT)) ? -0.001 : 0.001), 0.001, 0.05);
            break;
        case '2':
            noiseStrength = ofClamp(noiseStrength + ((ofGetKeyPressed(OF_KEY_SHIFT)) ? -0.1 : 0.1), 0, 3);
            break;
        case '3':
            attractorStrength = ofClamp(attractorStrength + ((ofGetKeyPressed(OF_KEY_SHIFT)) ? -0.1 : 0.1), 0, 2);
            break;
        case '4':
            particleSpawnRate = ofClamp(particleSpawnRate + ((ofGetKeyPressed(OF_KEY_SHIFT)) ? -0.05 : 0.05), 0, 1);
            break;
        case '5':
            createParticleBurst(ofGetWidth()/2, ofGetHeight()/2, 100);
            break;
    }
}

void ofApp::mouseMoved(int x, int y) {
    attractorPos.set(x, y, 0);
}

void ofApp::mouseDragged(int x, int y, int button) {
    // Create particles along mouse drag path
    createParticleBurst(x, y, 5);
    attractorPos.set(x, y, 0);
}

void ofApp::mousePressed(int x, int y, int button) {
    if (button == 0) {  // Left click
        createParticleBurst(x, y, 50);
    } else if (button == 2) {  // Right click
        createParticleBurst(x, y, 200);
    }
}

void ofApp::createParticleBurst(float x, float y, int count) {
    for (int i = 0; i < count; i++) {
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(30);
        Particle p(x + cos(angle) * radius, y + sin(angle) * radius);
        
        // Give burst particles initial velocity
        float speed = ofRandom(1, 4);
        p.vel.set(cos(angle) * speed, sin(angle) * speed, 0);
        
        particles.push_back(p);
    }
}

ofColor ofApp::getParticleColor(Particle& p) {
    float vel = p.vel.length();
    float angle = atan2(p.vel.y, p.vel.x);
    float hue, saturation, brightness;
    
    switch(colorMode) {
        case 0: // Aurora - Northern lights colors
            hue = ofMap(sin(time + p.pos.x * 0.01), -1, 1, 80, 180);
            saturation = ofMap(vel, 0, 5, 150, 255);
            brightness = ofMap(p.lifespan, 0, p.maxLife, 100, 255);
            break;
            
        case 1: // Sunset - Warm gradient
            hue = ofMap(p.pos.y, 0, ofGetHeight(), 0, 60);
            saturation = 255;
            brightness = ofMap(vel, 0, 5, 150, 255);
            break;
            
        case 2: // Ocean - Deep blues to cyan
            hue = ofMap(ofNoise(p.pos.x * 0.005, p.pos.y * 0.005, time), 0, 1, 140, 200);
            saturation = ofMap(vel, 0, 5, 180, 255);
            brightness = 220;
            break;
            
        case 3: // Galaxy - Purple to pink
            hue = ofMap(angle, -PI, PI, 270, 330);
            saturation = ofMap(p.pos.distance(attractorPos), 0, 300, 255, 150);
            brightness = ofMap(vel, 0, 5, 100, 255);
            break;
            
        case 4: // Rainbow vortex
            hue = ofMap(angle + time * 0.5, -PI, PI, 0, 255);
            saturation = 255;
            brightness = ofMap(p.lifespan, 0, p.maxLife, 50, 255);
            break;
    }
    
    return ofColor::fromHsb(hue, saturation, brightness);
}
