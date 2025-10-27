#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetSphereResolution(32);
    
    // Setup enhanced color palettes
    setupDreamyPalette();
    setupAuroraPalette();
    setupFlowerPalettes();
    
    // Initialize parameters - simpler for visibility
    numShapes = 20;
    numFlowers = 15;
    numBackgroundParticles = 50;
    numPollen = 30;
    baseRadius = 200;
    animationSpeed = 1.0;
    trailLength = 20;
    animationTime = 0;
    globalEnergy = 0.5;
    windStrength = 0.3;
    windDirection = ofVec3f(1, 0, 0.3);
    visualMode = 0; // Start with mixed mode
    
    // Enable effects
    enableLighting = true;
    enableGlow = true;
    enableTrails = true;
    enableBackgroundParticles = true;
    enableFlowers = true;
    enablePollen = true;
    
    // Setup camera - much simpler
    cam.setDistance(400);
    cam.setNearClip(1.0);
    cam.setFarClip(2000);
    
    // Setup one simple light
    lights.resize(1);
    lights[0].setPointLight();
    lights[0].setPosition(200, -200, 200);
    lights[0].setDiffuseColor(ofColor(255, 255, 255, 180));
    
    // Generate content
    generateFlowers();
    generateShapes();
    
    // Generate simple background particles
    backgroundParticles.clear();
    for(int i = 0; i < numBackgroundParticles; i++) {
        BackgroundParticle particle;
        particle.position = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        particle.velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        particle.color = getPetalColor(ofRandom(1.0));
        particle.color.a = 100;
        particle.life = 1.0;
        particle.size = 2;
        backgroundParticles.push_back(particle);
    }
    
    // Generate pollen
    pollen.clear();
    for(int i = 0; i < numPollen; i++) {
        FlowerPollen pollenParticle;
        pollenParticle.position = ofVec3f(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
        pollenParticle.velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        pollenParticle.color = ofColor(255, 255, 100, 150);
        pollenParticle.life = 1.0;
        pollenParticle.size = 3;
        pollenParticle.targetFlower = ofVec3f(0, 0, 0);
        pollen.push_back(pollenParticle);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    animationTime += ofGetLastFrameTime() * animationSpeed;
    globalEnergy = 0.6 + 0.4 * sin(animationTime * 1.5);
    
    updateFlowers();
    updateShapes();
    updateTrails();
    updateBackgroundParticles();
    updatePollen();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Simple background
    ofBackground(20, 15, 40);
    
    cam.begin();
    
    // Simple lighting
    if(enableLighting) {
        ofEnableLighting();
        lights[0].enable();
        ofSetGlobalAmbientColor(ofColor(50, 50, 50));
    }
    
    // Draw geometric shapes first - they're most visible
    for(const auto& shape : shapes) {
        // Draw trail
        if(enableTrails) {
            drawTrail(shape.trail);
        }
        
        // Draw main shape
        drawShape(shape);
        
        // Draw glow
        if(enableGlow) {
            drawGlowEffect(shape);
        }
    }
    
    // Draw flowers
    if(enableFlowers) {
        for(const auto& flower : flowers) {
            drawFlower(flower);
        }
    }
    
    // Draw particles
    if(enableBackgroundParticles) {
        drawBackgroundParticles();
    }
    
    if(enablePollen) {
        drawPollen();
    }
    
    // Disable lighting
    if(enableLighting) {
        lights[0].disable();
        ofDisableLighting();
    }
    
    cam.end();
    
    // UI
//    ofSetColor(255);
//    ofDrawBitmapString("🌸 Dreamy Flower Garden 🌸", 20, 20);
//    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 1), 20, 40);
//    ofDrawBitmapString("Shapes: " + ofToString(numShapes) + " | Flowers: " + ofToString(numFlowers), 20, 60);
//    ofDrawBitmapString("SPACE=regenerate | 1-6=modes | F=flowers | Mouse=rotate view", 20, 80);
//    ofDrawBitmapString("Current Mode: " + ofToString(visualMode), 20, 100);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case ' ':
            generateFlowers();
            generateShapes();
            break;
        case 'f':
        case 'F':
            enableFlowers = !enableFlowers;
            break;
        case 'p':
        case 'P':
            enablePollen = !enablePollen;
            break;
        case 'l':
        case 'L':
            enableLighting = !enableLighting;
            break;
        case 'g':
        case 'G':
            enableGlow = !enableGlow;
            break;
        case 't':
        case 'T':
            enableTrails = !enableTrails;
            break;
        case 'b':
        case 'B':
            enableBackgroundParticles = !enableBackgroundParticles;
            break;
        case '1':
            switchVisualMode(0);
            break;
        case '2':
            switchVisualMode(1);
            break;
        case '3':
            switchVisualMode(2);
            break;
        case '4':
            switchVisualMode(3);
            break;
        case '5':
            switchVisualMode(4);
            break;
        case '6':
            switchVisualMode(5);
            break;
        case '+':
        case '=':
            numFlowers = min(numFlowers + 3, 30);
            generateFlowers();
            break;
        case '-':
        case '_':
            numFlowers = max(numFlowers - 3, 3);
            generateFlowers();
            break;
        case 'r':
        case 'R':
            cam.reset();
            cam.setDistance(400);
            break;
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

//--------------------------------------------------------------
void ofApp::setupDreamyPalette() {
    dreamyColors.clear();
    dreamyColors.push_back(ofColor(255, 182, 193)); // Light pink
    dreamyColors.push_back(ofColor(173, 216, 230)); // Light blue
    dreamyColors.push_back(ofColor(221, 160, 221)); // Plum
    dreamyColors.push_back(ofColor(255, 218, 185)); // Peach
    dreamyColors.push_back(ofColor(152, 251, 152)); // Pale green
    dreamyColors.push_back(ofColor(255, 240, 245)); // Lavender blush
    dreamyColors.push_back(ofColor(230, 230, 250)); // Lavender
    dreamyColors.push_back(ofColor(255, 228, 225)); // Misty rose
}

//--------------------------------------------------------------
void ofApp::setupAuroraPalette() {
    auroraColors.clear();
    auroraColors.push_back(ofColor(0, 255, 127));   // Spring green
    auroraColors.push_back(ofColor(0, 191, 255));   // Deep sky blue
    auroraColors.push_back(ofColor(138, 43, 226));  // Blue violet
    auroraColors.push_back(ofColor(255, 20, 147));  // Deep pink
    auroraColors.push_back(ofColor(255, 140, 0));   // Dark orange
    auroraColors.push_back(ofColor(255, 105, 180)); // Hot pink
}

//--------------------------------------------------------------
void ofApp::setupFlowerPalettes() {
    flowerColors.clear();
    flowerColors.push_back(ofColor(255, 215, 0));   // Gold
    flowerColors.push_back(ofColor(255, 105, 180)); // Hot pink
    flowerColors.push_back(ofColor(255, 255, 0));   // Yellow
    flowerColors.push_back(ofColor(255, 20, 147));  // Deep pink
    
    petalColors.clear();
    petalColors.push_back(ofColor(255, 182, 193)); // Light pink
    petalColors.push_back(ofColor(255, 228, 225)); // Misty rose
    petalColors.push_back(ofColor(255, 240, 245)); // Lavender blush
    petalColors.push_back(ofColor(230, 230, 250)); // Lavender
    petalColors.push_back(ofColor(255, 218, 185)); // Peach
}

//--------------------------------------------------------------
ofColor ofApp::getDreamyColor(float t) {
    if(dreamyColors.size() == 0) return ofColor(255);
    int index = (int)(t * dreamyColors.size()) % dreamyColors.size();
    return dreamyColors[index];
}

//--------------------------------------------------------------
ofColor ofApp::getAuroraColor(float t) {
    if(auroraColors.size() == 0) return ofColor(255);
    int index = (int)(t * auroraColors.size()) % auroraColors.size();
    return auroraColors[index];
}

//--------------------------------------------------------------
ofColor ofApp::getFlowerColor(float t) {
    if(flowerColors.size() == 0) return ofColor(255);
    int index = (int)(t * flowerColors.size()) % flowerColors.size();
    return flowerColors[index];
}

//--------------------------------------------------------------
ofColor ofApp::getPetalColor(float t) {
    if(petalColors.size() == 0) return ofColor(255);
    int index = (int)(t * petalColors.size()) % petalColors.size();
    return petalColors[index];
}

//--------------------------------------------------------------
ofColor ofApp::getGradientColor(float t, float energy) {
    ofColor dreamy = getDreamyColor(t);
    ofColor aurora = getAuroraColor(t + 0.3);
    return dreamy.getLerped(aurora, energy);
}

//--------------------------------------------------------------
void ofApp::generateFlowers() {
    flowers.clear();
    
    for(int i = 0; i < numFlowers; i++) {
        Flower flower;
        
        float angle = (float)i / numFlowers * TWO_PI;
        float radius = 50 + i * 10;
        
        flower.centerPosition = ofVec3f(
            radius * cos(angle),
            ofRandom(-50, 50),
            radius * sin(angle)
        );
        
        flower.stemDirection = ofVec3f(0, -1, 0);
        flower.stemLength = 30;
        flower.centerColor = getFlowerColor((float)i / numFlowers);
        flower.petalColor = getPetalColor((float)i / numFlowers);
        flower.bloomProgress = 1.0;
        flower.rotationSpeed = 0.5;
        flower.petalCount = 6;
        flower.flowerSize = 20;
        flower.energy = 1.0;
        flower.flowerType = i % 5;
        flower.phase = ofRandom(TWO_PI);
        flower.swayOffset = ofVec3f(0, 0, 0);
        
        generatePetals(flower);
        flowers.push_back(flower);
    }
}

//--------------------------------------------------------------
void ofApp::generatePetals(Flower& flower) {
    flower.petals.clear();
    
    for(int i = 0; i < flower.petalCount; i++) {
        Petal petal;
        
        float angle = (float)i / flower.petalCount * TWO_PI;
        float radius = flower.flowerSize;
        
        petal.position = ofVec3f(
            radius * cos(angle),
            0,
            radius * sin(angle)
        );
        
        petal.normal = ofVec3f(0, 1, 0);
        petal.color = flower.petalColor;
        petal.size = flower.flowerSize * 0.3;
        petal.angle = angle;
        petal.curvature = 0.2;
        petal.life = 1.0;
        
        flower.petals.push_back(petal);
    }
}

//--------------------------------------------------------------
void ofApp::updateFlowers() {
    for(auto& flower : flowers) {
        updateBloomingAnimation(flower);
        
        // Simple sway
        flower.swayOffset.x = sin(animationTime + flower.phase) * 5;
        flower.swayOffset.z = cos(animationTime + flower.phase) * 3;
        
        // Update petals
        for(auto& petal : flower.petals) {
            petal.color = getPetalColor(animationTime * 0.1 + flower.phase);
        }
        
        flower.energy = 0.7 + 0.3 * sin(animationTime + flower.phase);
    }
}

//--------------------------------------------------------------
void ofApp::updateBloomingAnimation(Flower& flower) {
    flower.bloomProgress = 0.8 + 0.2 * sin(animationTime * 0.5 + flower.phase);
}

//--------------------------------------------------------------
void ofApp::drawFlower(const Flower& flower) {
    ofPushMatrix();
    
    ofTranslate(flower.centerPosition + flower.swayOffset);
    
    // Draw stem
    drawStem(flower);
    
    // Draw center
    ofSetColor(flower.centerColor);
    ofDrawSphere(0, 0, 0, 3);
    
    // Draw petals
    for(const auto& petal : flower.petals) {
        drawPetal(petal);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawPetal(const Petal& petal) {
    ofPushMatrix();
    
    ofTranslate(petal.position);
    ofRotateYDeg(petal.angle * RAD_TO_DEG);
    
    ofSetColor(petal.color);
    ofDrawSphere(0, 0, 0, petal.size);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawStem(const Flower& flower) {
    ofSetColor(50, 150, 50);
    ofSetLineWidth(2);
    ofDrawLine(ofVec3f(0, 0, 0), flower.stemDirection * flower.stemLength);
}

//--------------------------------------------------------------
void ofApp::drawFlowerGlow(const Flower& flower) {
    ofPushMatrix();
    ofTranslate(flower.centerPosition + flower.swayOffset);
    
    ofColor glowColor = flower.petalColor;
    glowColor.a = 50;
    ofSetColor(glowColor);
    ofDrawSphere(0, 0, 0, flower.flowerSize * 2);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::updatePollen() {
    for(auto& particle : pollen) {
        particle.position += particle.velocity * ofGetLastFrameTime() * 10;
        
        if(particle.position.length() > 300) {
            particle.position = ofVec3f(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
            particle.velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawPollen() {
    for(const auto& particle : pollen) {
        ofSetColor(particle.color);
        ofDrawSphere(particle.position, particle.size);
    }
}

//--------------------------------------------------------------
void ofApp::generateShapes() {
    shapes.clear();
    
    for(int i = 0; i < numShapes; i++) {
        GeometricShape shape;
        
        shape.position = calculatePosition(0, i, baseRadius);
        shape.prevPosition = shape.position;
        shape.rotation = ofVec3f(0, 0, 0);
        shape.scale = ofVec3f(1, 1, 1);
        shape.color = getGradientColor((float)i / numShapes, 0.5);
        shape.glowColor = getAuroraColor((float)i / numShapes);
        shape.phase = ofRandom(TWO_PI);
        shape.energy = 0.5;
        shape.shapeType = i % 4;
        
        if(visualMode == 0) {
            shape.attractorType = i % 5;
        } else if(visualMode == 5) {
            shape.attractorType = 4;
        } else {
            shape.attractorType = visualMode - 1;
        }
        
        shape.trail.maxLength = trailLength;
        shape.trail.positions.clear();
        shape.trail.colors.clear();
        shape.trail.ages.clear();
        
        shapes.push_back(shape);
    }
}

//--------------------------------------------------------------
void ofApp::updateShapes() {
    for(int i = 0; i < shapes.size(); i++) {
        GeometricShape& shape = shapes[i];
        shape.prevPosition = shape.position;
        
        switch(shape.attractorType) {
            case 0:
                shape.position = calculateLorenzAttractor(animationTime, i);
                break;
            case 4:
                shape.position = calculateFlowerSpiral(animationTime, i);
                break;
            default:
                shape.position = calculateLorenzAttractor(animationTime, i);
                break;
        }
        
        shape.rotation = calculateRotation(animationTime, i);
        shape.energy = calculateEnergy(animationTime, i);
        
        float scaleValue = calculateScale(animationTime, i);
        shape.scale = ofVec3f(scaleValue, scaleValue, scaleValue);
        
        float colorPhase = animationTime * 0.1 + shape.phase;
        shape.color = getGradientColor(colorPhase, shape.energy);
        shape.glowColor = getAuroraColor(colorPhase + 0.3);
        
        shape.color.a = 200;
        shape.glowColor.a = 100;
    }
}

//--------------------------------------------------------------
void ofApp::updateTrails() {
    for(auto& shape : shapes) {
        ParticleTrail& trail = shape.trail;
        
        trail.positions.push_back(shape.position);
        trail.colors.push_back(shape.color);
        trail.ages.push_back(1.0);
        
        for(int i = trail.ages.size() - 1; i >= 0; i--) {
            trail.ages[i] -= ofGetLastFrameTime() * 2.0;
            
            if(trail.ages[i] <= 0 || trail.positions.size() > trail.maxLength) {
                trail.positions.erase(trail.positions.begin() + i);
                trail.colors.erase(trail.colors.begin() + i);
                trail.ages.erase(trail.ages.begin() + i);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateBackgroundParticles() {
    for(auto& particle : backgroundParticles) {
        particle.position += particle.velocity * ofGetLastFrameTime() * 20;
        
        if(particle.position.length() > 400) {
            particle.position = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
            particle.velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawShape(const GeometricShape& shape) {
    ofPushMatrix();
    
    ofTranslate(shape.position);
    ofRotateXDeg(shape.rotation.x);
    ofRotateYDeg(shape.rotation.y);
    ofRotateZDeg(shape.rotation.z);
    ofScale(shape.scale.x, shape.scale.y, shape.scale.z);
    
    ofSetColor(shape.color);
    
    switch(shape.shapeType) {
        case 0:
            ofDrawSphere(0, 0, 0, 5);
            break;
        case 1:
            ofDrawBox(0, 0, 0, 10);
            break;
        case 2:
            ofDrawCone(0, 0, 0, 5, 10);
            break;
        case 3:
            ofDrawIcoSphere(0, 0, 0, 5);
            break;
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawTrail(const ParticleTrail& trail) {
    if(trail.positions.size() < 2) return;
    
    ofSetLineWidth(2);
    for(int i = 1; i < trail.positions.size(); i++) {
        ofColor trailColor = trail.colors[i];
        trailColor.a = trail.ages[i] * 150;
        ofSetColor(trailColor);
        ofDrawLine(trail.positions[i-1], trail.positions[i]);
    }
}

//--------------------------------------------------------------
void ofApp::drawBackgroundParticles() {
    for(const auto& particle : backgroundParticles) {
        ofSetColor(particle.color);
        ofDrawSphere(particle.position, particle.size);
    }
}

//--------------------------------------------------------------
void ofApp::drawGlowEffect(const GeometricShape& shape) {
    ofPushMatrix();
    ofTranslate(shape.position);
    
    ofColor glowColor = shape.glowColor;
    glowColor.a = 30;
    ofSetColor(glowColor);
    ofDrawSphere(0, 0, 0, shape.scale.x * 15);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateFlowerSpiral(float time, int index) {
    float t = time * 0.3;
    float indexNorm = (float)index / numShapes;
    
    float angle = indexNorm * AttractorParams::GOLDEN_ANGLE * DEG_TO_RAD + t;
    float radius = baseRadius * 0.5 * sqrt(indexNorm);
    
    float x = radius * cos(angle);
    float y = indexNorm * 100 - 50 + 20 * sin(t * 2);
    float z = radius * sin(angle);
    
    return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateLorenzAttractor(float time, int index) {
    float t = time * 0.1;
    float indexNorm = (float)index / numShapes;
    
    float x = indexNorm * 10 - 5;
    float y = indexNorm * 10 - 5;
    float z = indexNorm * 15;
    
    // Simple Lorenz-like movement
    float radius = 100 + 50 * sin(t + indexNorm * TWO_PI);
    float angle = t + indexNorm * TWO_PI;
    
    return ofVec3f(
        radius * cos(angle),
        indexNorm * 100 - 50 + 30 * sin(t * 2),
        radius * sin(angle)
    );
}

//--------------------------------------------------------------
ofVec3f ofApp::calculatePosition(float time, int index, float baseRadius) {
    return calculateLorenzAttractor(time, index);
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateRotation(float time, int index) {
    float t = time * 50;
    return ofVec3f(t + index * 10, t * 1.2 + index * 15, t * 0.8 + index * 5);
}

//--------------------------------------------------------------
float ofApp::calculateScale(float time, int index) {
    return 1.0 + 0.5 * sin(time * 2 + index * 0.5);
}

//--------------------------------------------------------------
float ofApp::calculateEnergy(float time, int index) {
    return 0.5 + 0.5 * sin(time * 1.5 + index * 0.3);
}

//--------------------------------------------------------------
void ofApp::setupPostProcessing() {
    // Empty for now
}

//--------------------------------------------------------------
void ofApp::beginGlowEffect() {
    // Empty for now
}

//--------------------------------------------------------------
void ofApp::endGlowEffect() {
    // Empty for now
}

//--------------------------------------------------------------
void ofApp::switchVisualMode(int mode) {
    visualMode = mode;
    generateShapes();
    if(mode == 5) {
        generateFlowers();
    }
}

// Dummy implementations for missing attractors
ofVec3f ofApp::calculateGoldenRatio(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculateFibonacciSpiral(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculatePetalPattern(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculateRosePattern(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculateRosslerAttractor(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculateChenAttractor(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculateFractalSpiral(float time, int index) { return ofVec3f(0,0,0); }
ofVec3f ofApp::calculatePetalPosition(const Flower& flower, int petalIndex, float bloomProgress) { return ofVec3f(0,0,0); }
