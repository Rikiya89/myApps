#include "ofApp.h"
#include <algorithm>

// Particle implementation
Particle::Particle(ofVec2f pos, ofVec2f vel, ofColor col, float lifetime, int chakra) {
    position = pos;
    velocity = vel;
    acceleration = ofVec2f(0, 0);
    life = lifetime;
    maxLife = lifetime;
    size = ofRandom(0.8, 3.0);
    color = col;
    energy = 1.0;
    chakraType = chakra;
    auraRadius = ofRandom(8, 20);
    rotationAngle = ofRandom(0, TWO_PI);
}

void Particle::update() {
    velocity += acceleration;
    velocity *= 0.69; // Even gentler damping
    position += velocity;
    acceleration *= 0;
    
    life -= 0.4; // Slower decay for longer presence
    energy = life / maxLife;
    rotationAngle += 0.02;
    
    // Breathing pulse effect
    float breathPulse = sin(life * 0.08) * 0.3 + 0.7;
    auraRadius = auraRadius * breathPulse;
    
    // Multi-layered alpha with spiritual glow
    float coreAlpha = ofMap(energy, 0, 1, 0, 200);
    float auraAlpha = ofMap(energy, 0, 1, 0, 80);
    color.a = coreAlpha;
}

void Particle::draw() {
    float currentSize = size * energy;
    float pulse = 1.0 + sin(rotationAngle * 3) * 0.2;
    
    ofPushMatrix();
    ofTranslate(position.x, position.y);
    ofRotate(ofRadToDeg(rotationAngle));
    
    // Outer spiritual aura
    ofSetColor(color.r, color.g, color.b, color.a * 0.15);
    ofDrawCircle(0, 0, auraRadius * pulse);
    
    // Middle energy field
    ofSetColor(color.r, color.g, color.b, color.a * 0.4);
    ofDrawCircle(0, 0, currentSize * 2.5 * pulse);
    
    // Inner light core
    ofSetColor(color);
    ofDrawCircle(0, 0, currentSize * pulse);
    
    // Sacred center spark
    ofSetColor(255, 255, 255, color.a);
    ofDrawCircle(0, 0, currentSize * 0.4);
    
    // Subtle geometric form based on chakra
    if (energy > 0.7) {
        ofSetColor(255, 255, 255, color.a * 0.6);
        ofNoFill();
        ofSetLineWidth(0.5);
        
        switch (chakraType) {
            case 0: // Root - Square
                ofDrawRectangle(-currentSize, -currentSize, currentSize*2, currentSize*2);
                break;
            case 1: // Sacral - Crescent
                ofDrawCircle(0, currentSize*0.5, currentSize*0.8);
                break;
            case 2: // Solar - Triangle
                ofDrawTriangle(-currentSize, currentSize, currentSize, currentSize, 0, -currentSize);
                break;
            case 3: // Heart - Star
                for(int i = 0; i < 6; i++) {
                    float angle = (TWO_PI/6) * i;
                    ofDrawLine(0, 0, cos(angle)*currentSize, sin(angle)*currentSize);
                }
                break;
            case 4: // Throat - Circle
                ofDrawCircle(0, 0, currentSize * 1.2);
                break;
            case 5: // Third Eye - Oval
                ofDrawEllipse(0, 0, currentSize*2, currentSize*1.5);
                break;
            case 6: // Crown - Lotus petals
                for(int i = 0; i < 8; i++) {
                    float angle = (TWO_PI/8) * i;
                    ofDrawLine(0, 0, cos(angle)*currentSize*0.8, sin(angle)*currentSize*0.8);
                }
                break;
        }
        ofFill();
    }
    
    ofPopMatrix();
}

bool Particle::isDead() {
    return life <= 0;
}

// ofApp implementation
void ofApp::setup() {
    ofSetFrameRate(60);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    time = 0;
    breathingCycle = 0;
    meditationPhase = 0;
    cosmicRotation = 0;
    maxParticles = 250;
    energyFlow = 2;
    harmonyLevel = 1.0;
    showSacredGeometry = true;
    showChakras = true;
    breathingMode = true;
    showLotus = true;
    showMandala = true;
    showAura = true;
    
    // Set up 7 chakra points in elegant vertical alignment
    float centerX = ofGetWidth() * 0.5;
    float startY = ofGetHeight() * 0.12;
    float spacing = ofGetHeight() * 0.11;
    
    for (int i = 0; i < 7; i++) {
        chakraPoints.push_back(ofVec2f(centerX, startY + i * spacing));
    }
    
    // Enhanced chakra colors with deeper spiritual significance
    chakraColors.resize(7);
    chakraColors[0] = ofColor(220, 20, 60);    // Root - Deep Crimson
    chakraColors[1] = ofColor(255, 140, 0);    // Sacral - Sacred Orange
    chakraColors[2] = ofColor(255, 215, 0);    // Solar Plexus - Golden Yellow
    chakraColors[3] = ofColor(50, 205, 50);    // Heart - Emerald Green
    chakraColors[4] = ofColor(30, 144, 255);   // Throat - Sky Blue
    chakraColors[5] = ofColor(138, 43, 226);   // Third Eye - Royal Purple
    chakraColors[6] = ofColor(238, 130, 238);  // Crown - Divine Violet
}

void ofApp::update() {
    time += 0.016f;
    breathingCycle += 0.006f; // Slower, deeper breathing
    meditationPhase += 0.001f;
    cosmicRotation += 0.003f;
    
    updateBreathing();
    spawnSpiritualParticles();
    
    // More graceful chakra movement with cosmic rhythm
    float cosmicBreath = sin(breathingCycle) * 12;
    float celestialFlow = sin(cosmicRotation * 2) * 5;
    
    for (int i = 0; i < chakraPoints.size(); i++) {
        // Gentle figure-8 motion
        float phase = time * 0.2 + i * 0.3;
        chakraPoints[i].x = ofGetWidth() * 0.5 + sin(phase) * 20 + celestialFlow;
        chakraPoints[i].y += cosmicBreath * 0.08;
        
        // Subtle spiral ascension
        float spiralMotion = sin(time * 0.1 + i * 0.5) * 3;
        chakraPoints[i].x += spiralMotion;
    }
    
    // Update particles with enhanced spiritual physics
    for (int i = particles.size() - 1; i >= 0; i--) {
        Particle& p = particles[i];
        
        // Multi-chakra attraction with harmonic resonance
        ofVec2f totalForce(0, 0);
        for (int j = 0; j < chakraPoints.size(); j++) {
            ofVec2f chakraForce = chakraPoints[j] - p.position;
            float distance = chakraForce.length();
            
            if (distance > 0) {
                chakraForce.normalize();
                // Harmonic attraction based on chakra resonance
                float resonance = 1.0 + sin(time + j + p.chakraType) * 0.5;
                float strength = (harmonyLevel * 20 * resonance) / (distance * 0.2 + 1);
                totalForce += chakraForce * strength * 0.02;
            }
        }
        p.acceleration += totalForce;
        
        // Enhanced kundalini rising with spiral motion
        float kundaliniStrength = 0.15 + sin(breathingCycle) * 0.05;
        p.acceleration += ofVec2f(0, -kundaliniStrength) * harmonyLevel;
        
        // Sacred spiral motion around central axis
        float distanceFromCenter = p.position.distance(ofVec2f(ofGetWidth()*0.5, ofGetHeight()*0.5));
        float spiralAngle = atan2(p.position.y - ofGetHeight()*0.5, p.position.x - ofGetWidth()*0.5);
        ofVec2f spiralForce = ofVec2f(
            cos(spiralAngle + PI/2),
            sin(spiralAngle + PI/2)
        ) * (0.8 / (distanceFromCenter * 0.01 + 1));
        p.acceleration += spiralForce;
        
        // Cosmic turbulence for natural flow
        float cosmicNoise = 0.3;
        p.acceleration += ofVec2f(
            sin(p.position.x * 0.01 + time * 0.5) * cosmicNoise,
            cos(p.position.y * 0.01 + time * 0.3) * cosmicNoise
        );
        
        p.update();
        
        // Gentle edge wrapping with mystical transition
        if (p.position.x < -30) p.position.x = ofGetWidth() + 30;
        if (p.position.x > ofGetWidth() + 30) p.position.x = -30;
        if (p.position.y < -30) p.position.y = ofGetHeight() + 30;
        if (p.position.y > ofGetHeight() + 30) p.position.y = -30;
        
        if (p.isDead()) {
            particles.erase(particles.begin() + i);
        }
    }
    
    while (particles.size() > maxParticles) {
        particles.erase(particles.begin());
    }
}

void ofApp::updateBreathing() {
    // Deep meditative breathing pattern
    float breathPattern = sin(breathingCycle);
    harmonyLevel = 0.7 + breathPattern * 0.5;
    
    // Cosmic energy cycles
    if (breathingMode) {
        energyFlow = 1.5 + sin(breathingCycle * 0.3) * 1.5;
    }
}

void ofApp::draw() {
    // Cosmic meditation background
    drawCosmicBackground();
    
    // Gentle energy trails
    ofSetColor(1, 2, 8, 35);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    // Draw aura field
    if (showAura) {
        drawAuraField();
    }
    
    // Draw sacred geometry layers
    if (showSacredGeometry) {
        drawSacredGeometry();
    }
    
    // Draw lotus patterns
    if (showLotus) {
        drawLotusPattern();
    }
    
    // Draw mandala
    if (showMandala) {
        drawMandala();
    }
    
    // Draw energy flow between chakras
    drawEnergyFlow();
    
    // Draw spiritual particles
    for (auto& particle : particles) {
        particle.draw();
    }
    
    // Draw chakras with lotus forms
    if (showChakras) {
        drawChakras();
    }
    
}

void ofApp::spawnSpiritualParticles() {
    for (int i = 0; i < energyFlow; i++) {
        // Spawn from chakras with sacred timing
        int chakraIndex = (int)ofRandom(chakraPoints.size());
        ofVec2f spawnPos = chakraPoints[chakraIndex];
        
        // Sacred circle spawning pattern
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(15, 45);
        spawnPos += ofVec2f(cos(angle) * radius, sin(angle) * radius);
        
        // Gentle upward-flowing initial velocity
        ofVec2f vel = ofVec2f(
            ofRandom(-0.4, 0.4),
            ofRandom(-0.8, -0.2)
        );
        
        // Enhanced chakra colors with cosmic variation
        ofColor chakraColor = getChakraColor(chakraIndex);
        
        particles.push_back(Particle(spawnPos, vel, chakraColor, ofRandom(300, 600), chakraIndex));
    }
}

ofColor ofApp::getChakraColor(int chakra) {
    if (chakra >= 0 && chakra < chakraColors.size()) {
        ofColor baseColor = chakraColors[chakra];
        
        // Cosmic breathing color variation
        float cosmicShift = sin(breathingCycle * 2 + chakra * 0.7) * 0.15;
        float hue = baseColor.getHue() + cosmicShift * 30;
        float saturation = baseColor.getSaturation() + cosmicShift * 40;
        float brightness = baseColor.getBrightness() + cosmicShift * 25;
        
        ofColor color;
        color.setHsb(
            fmod(hue + 360, 360),
            ofClamp(saturation, 120, 255),
            ofClamp(brightness, 180, 255)
        );
        return color;
    }
    return ofColor(255);
}

void ofApp::drawCosmicBackground() {
    // Multi-layered cosmic background
    ofBackgroundGradient(
        ofColor(0, 2, 8),
        ofColor(8, 15, 30),
        OF_GRADIENT_CIRCULAR
    );
    
    // Subtle stars
    ofSetColor(255, 255, 255, 30);
    for (int i = 0; i < 50; i++) {
        float x = (sin(time * 0.1 + i) * 0.5 + 0.5) * ofGetWidth();
        float y = (cos(time * 0.07 + i * 1.5) * 0.5 + 0.5) * ofGetHeight();
        float twinkle = sin(time * 2 + i) * 0.5 + 0.5;
        ofSetColor(255, 255, 255, 30 * twinkle);
        ofDrawCircle(x, y, 1);
    }
}

void ofApp::drawSacredGeometry() {
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofRotate(cosmicRotation * 5);
    
    // Enhanced Flower of Life with sacred proportions
    drawFlowerOfLife();
    
    // Sri Yantra overlay
    ofRotate(45);
    drawSriYantra();
    
    ofPopMatrix();
}

void ofApp::drawFlowerOfLife() {
    ofSetLineWidth(0.8);
    ofNoFill();
    
    float radius = 35 + sin(time * 0.3) * 8;
    float breathingPulse = 1.0 + sin(breathingCycle) * 0.2;
    
    // Multiple layers with different sacred colors
    for (int layer = 0; layer < 3; layer++) {
        float layerRadius = radius * (1 + layer * 0.3) * breathingPulse;
        float alpha = 50 - layer * 15;
        
        // Central sacred circle
        ofSetColor(120, 180, 255, alpha);
        ofDrawCircle(0, 0, layerRadius);
        
        // Six-fold sacred pattern
        for (int i = 0; i < 6; i++) {
            float angle = (TWO_PI / 6) * i;
            float x = cos(angle) * layerRadius;
            float y = sin(angle) * layerRadius;
            
            ofSetColor(180, 120, 255, alpha);
            ofDrawCircle(x, y, layerRadius);
        }
        
        // Twelve-fold outer sacred ring
        for (int i = 0; i < 12; i++) {
            float angle = (TWO_PI / 12) * i;
            float x = cos(angle) * layerRadius * 1.73;
            float y = sin(angle) * layerRadius * 1.73;
            
            ofSetColor(255, 180, 120, alpha * 0.7);
            ofDrawCircle(x, y, layerRadius * 0.8);
        }
    }
}

void ofApp::drawSriYantra() {
    ofSetLineWidth(0.6);
    ofSetColor(255, 215, 0, 25); // Golden sacred color
    ofNoFill();
    
    float size = 60 + sin(time * 0.4) * 10;
    
    // Nine interlocking triangles (simplified)
    for (int i = 0; i < 4; i++) {
        float triangleSize = size * (0.6 + i * 0.15);
        
        // Upward triangles (Shiva)
        ofDrawTriangle(
            0, -triangleSize,
            -triangleSize * 0.866, triangleSize * 0.5,
            triangleSize * 0.866, triangleSize * 0.5
        );
        
        // Downward triangles (Shakti)
        ofDrawTriangle(
            0, triangleSize,
            -triangleSize * 0.866, -triangleSize * 0.5,
            triangleSize * 0.866, -triangleSize * 0.5
        );
    }
}

void ofApp::drawLotusPattern() {
    // Lotus petals around each chakra
    for (int i = 0; i < chakraPoints.size(); i++) {
        int petalCount[] = {4, 6, 10, 12, 16, 96, 1000}; // Traditional chakra petal counts (simplified)
        int petals = petalCount[i];
        if (petals > 20) petals = 20; // Limit for performance
        
        drawChakraLotus(chakraPoints[i], getChakraColor(i), petals, 30 + i * 3);
    }
}

void ofApp::drawChakraLotus(ofVec2f center, ofColor color, int petals, float size) {
    ofPushMatrix();
    ofTranslate(center.x, center.y);
    ofRotate(time * 3 + center.x * 0.01);
    
    ofSetColor(color.r, color.g, color.b, 20);
    ofNoFill();
    ofSetLineWidth(0.5);
    
    float breathingSize = size * (1.0 + sin(breathingCycle) * 0.3);
    
    // Draw lotus petals
    for (int i = 0; i < petals; i++) {
        float angle = (TWO_PI / petals) * i;
        float petalLength = breathingSize * 0.8;
        
        ofPushMatrix();
        ofRotate(ofRadToDeg(angle));
        
        // Petal shape using curves
        ofBeginShape();
        ofVertex(0, 0);
        ofBezierVertex(petalLength * 0.3, -petalLength * 0.1,
                      petalLength * 0.7, -petalLength * 0.3,
                      petalLength, 0);
        ofBezierVertex(petalLength * 0.7, petalLength * 0.3,
                      petalLength * 0.3, petalLength * 0.1,
                      0, 0);
        ofEndShape();
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

void ofApp::drawMandala() {
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    ofRotate(cosmicRotation * 10);
    
    ofSetColor(200, 150, 255, 15);
    ofSetLineWidth(0.4);
    ofNoFill();
    
    // Sacred geometric mandala
    for (int ring = 1; ring <= 6; ring++) {
        float radius = ring * 25 + sin(time + ring) * 5;
        int divisions = ring * 6;
        
        ofDrawCircle(0, 0, radius);
        
        // Radial lines
        for (int i = 0; i < divisions; i++) {
            float angle = (TWO_PI / divisions) * i;
            float x1 = cos(angle) * radius * 0.8;
            float y1 = sin(angle) * radius * 0.8;
            float x2 = cos(angle) * radius * 1.2;
            float y2 = sin(angle) * radius * 1.2;
            
            ofDrawLine(x1, y1, x2, y2);
        }
    }
    
    ofPopMatrix();
}

void ofApp::drawAuraField() {
    // Subtle aura field around the entire composition
    ofSetColor(100, 150, 255, 8);
    
    for (int i = 0; i < 20; i++) {
        float radius = 100 + i * 30 + sin(time * 0.5 + i) * 20;
        ofNoFill();
        ofSetLineWidth(0.3);
        ofDrawCircle(ofGetWidth() * 0.5, ofGetHeight() * 0.5, radius);
    }
}

void ofApp::drawChakras() {
    for (int i = 0; i < chakraPoints.size(); i++) {
        ofColor chakraColor = getChakraColor(i);
        
        // Enhanced breathing pulse with harmonic overtones
        float mainPulse = 1.0 + sin(breathingCycle * 3 + i * 0.8) * 0.4;
        float harmonicPulse = 1.0 + sin(breathingCycle * 7 + i * 1.2) * 0.15;
        float pulse = mainPulse * harmonicPulse;
        
        // Expanded aura layers
        ofSetColor(chakraColor.r, chakraColor.g, chakraColor.b, 15);
        ofDrawCircle(chakraPoints[i], 40 * pulse);
        
        ofSetColor(chakraColor.r, chakraColor.g, chakraColor.b, 25);
        ofDrawCircle(chakraPoints[i], 28 * pulse);
        
        ofSetColor(chakraColor.r, chakraColor.g, chakraColor.b, 50);
        ofDrawCircle(chakraPoints[i], 18 * pulse);
        
        ofSetColor(chakraColor.r, chakraColor.g, chakraColor.b, 100);
        ofDrawCircle(chakraPoints[i], 10);
        
        // Sacred geometric center
        ofSetColor(255, 255, 255, 180);
        ofDrawCircle(chakraPoints[i], 4);
        
        // Divine spark
        ofSetColor(255, 255, 255, 255);
        ofDrawCircle(chakraPoints[i], 1.5);
    }
}

void ofApp::drawEnergyFlow() {
    // Enhanced flowing energy between chakras
    ofSetColor(150, 200, 255, 30);
    ofSetLineWidth(1.2);
    
    for (int i = 0; i < chakraPoints.size() - 1; i++) {
        ofVec2f start = chakraPoints[i];
        ofVec2f end = chakraPoints[i + 1];
        
        ofPolyline line;
        for (float t = 0; t <= 1; t += 0.05) {
            ofVec2f point = start.interpolate(end, t);
            
            // Multi-layered wave pattern
            float wave1 = sin(time * 3 + t * 15) * 12;
            float wave2 = sin(time * 5 + t * 25 + PI/3) * 6;
            float wave3 = sin(time * 7 + t * 35 + PI/2) * 3;
            
            point.x += wave1 + wave2 + wave3;
            line.addVertex(point.x, point.y);
        }
        line.draw();
        
        // Secondary harmonic flow
        ofSetColor(255, 150, 200, 20);
        ofPolyline line2;
        for (float t = 0; t <= 1; t += 0.05) {
            ofVec2f point = start.interpolate(end, t);
            float wave = sin(time * 2 + t * 12 + PI) * 8;
            point.x -= wave;
            line2.addVertex(point.x, point.y);
        }
        line2.draw();
    }
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case ' ':
            showSacredGeometry = !showSacredGeometry;
            break;
        case 'c':
        case 'C':
            showChakras = !showChakras;
            break;
        case 'l':
        case 'L':
            showLotus = !showLotus;
            break;
        case 'm':
        case 'M':
            showMandala = !showMandala;
            break;
        case 'a':
        case 'A':
            showAura = !showAura;
            break;
        case 'b':
        case 'B':
            breathingMode = !breathingMode;
            break;
        case '+':
        case '=':
            if (energyFlow < 8) energyFlow++;
            break;
        case '-':
            if (energyFlow > 1) energyFlow--;
            break;
        case 'r':
            particles.clear();
            breathingCycle = 0;
            meditationPhase = 0;
            cosmicRotation = 0;
            break;
    }
}

void ofApp::keyReleased(int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {
    if (!chakraPoints.empty()) {
        float minDist = FLT_MAX;
        int nearestIndex = 0;
        
        for (int i = 0; i < chakraPoints.size(); i++) {
            float dist = chakraPoints[i].distance(ofVec2f(x, y));
            if (dist < minDist) {
                minDist = dist;
                nearestIndex = i;
            }
        }
        
        chakraPoints[nearestIndex] = ofVec2f(x, y);
    }
}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::windowResized(int w, int h) {
    // Recenter chakras with sacred proportions
    float centerX = w * 0.5;
    float startY = h * 0.12;
    float spacing = h * 0.11;
    
    for (int i = 0; i < chakraPoints.size(); i++) {
        chakraPoints[i] = ofVec2f(centerX, startY + i * spacing);
    }
}
