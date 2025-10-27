#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    // Enforce window size at runtime (some macOS setups ignore initial GLFW size)
    ofSetWindowShape(720, 1080);
    ofSetWindowPosition(60, 40);
    
    time = 0;
    numParticles = 600;
    noiseScale = 0.008;
    cameraRotationSpeed = 0.3;
    autoRotateCamera = true;
    showFlowField = false;
    showConnections = true;
    
    // Initialize waves for interference patterns
    waves.clear();
    for(int i = 0; i < 5; i++) {
        Wave wave;
        wave.frequency = ofRandom(0.5, 2.0);
        wave.amplitude = ofRandom(20, 60);
        wave.phase = ofRandom(0, TWO_PI);
        wave.direction = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)).normalize();
        waves.push_back(wave);
    }
    
    createFlowField();
    createParticles();
    
    cam.setDistance(600);
    cam.setFov(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    updateFlowField();
    updateParticles();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Black background for monochrome theme
    ofBackground(0);
    
    if(autoRotateCamera) {
        float camX = cos(time * cameraRotationSpeed) * cam.getDistance();
        float camZ = sin(time * cameraRotationSpeed) * cam.getDistance();
        cam.setPosition(camX, cam.getPosition().y, camZ);
        cam.lookAt(ofVec3f(0, 0, 0));
    }
    
    cam.begin();
    
    // Draw wave interference patterns
    drawWaveInterference();
    
    // Draw geometric patterns
    drawGeometricPatterns();
    
    // Draw flow field if enabled
    if(showFlowField) {
        drawFlowField();
    }
    
    // Draw particle connections
    if(showConnections) {
        drawConnections();
    }
    
    // Draw particles
    drawParticles();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        autoRotateCamera = !autoRotateCamera;
    }
    if(key == 'r' || key == 'R') {
        createParticles();
        createFlowField();
    }
    if(key == 'f' || key == 'F') {
        showFlowField = !showFlowField;
    }
    if(key == 'c' || key == 'C') {
        showConnections = !showConnections;
    }
    if(key == 'w' || key == 'W') {
        // Regenerate waves
        waves.clear();
        for(int i = 0; i < 5; i++) {
            Wave wave;
            wave.frequency = ofRandom(0.5, 2.0);
            wave.amplitude = ofRandom(20, 60);
            wave.phase = ofRandom(0, TWO_PI);
            wave.direction = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)).normalize();
            waves.push_back(wave);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(!autoRotateCamera) {
        float mouseInfluence = 0.001;
        cameraRotationSpeed = (x - ofGetWidth()/2) * mouseInfluence;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    autoRotateCamera = false;
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
void ofApp::createFlowField(){
    flowField.cols = 20;
    flowField.rows = 20;
    flowField.depth = 20;
    flowField.scale = 40.0;
    
    flowField.vectors.clear();
    flowField.vectors.resize(flowField.cols * flowField.rows * flowField.depth);
    
    updateFlowField();
}

//--------------------------------------------------------------
void ofApp::updateFlowField(){
    for(int z = 0; z < flowField.depth; z++) {
        for(int y = 0; y < flowField.rows; y++) {
            for(int x = 0; x < flowField.cols; x++) {
                int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
                
                float xPos = (x - flowField.cols/2) * flowField.scale;
                float yPos = (y - flowField.rows/2) * flowField.scale;
                float zPos = (z - flowField.depth/2) * flowField.scale;
                
                // Create complex flow field using multiple noise octaves
                float angle = ofNoise(xPos * noiseScale, yPos * noiseScale, zPos * noiseScale, time * 0.1) * TWO_PI * 4;
                float angle2 = ofNoise(xPos * noiseScale * 2, yPos * noiseScale * 2, zPos * noiseScale * 2, time * 0.2) * TWO_PI * 2;
                
                ofVec3f vector;
                vector.x = cos(angle) * cos(angle2);
                vector.y = sin(angle) * cos(angle2);
                vector.z = sin(angle2);
                
                // Add wave influence
                for(auto& wave : waves) {
                    float waveContrib = sin(wave.phase + time * wave.frequency + 
                                          (xPos * wave.direction.x + yPos * wave.direction.y + zPos * wave.direction.z) * 0.01);
                    vector += wave.direction * waveContrib * 0.3;
                }
                
                flowField.vectors[index] = vector.normalize();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::createParticles(){
    particles.clear();
    
    for(int i = 0; i < numParticles; i++){
        Particle p;
        p.position.set(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        p.velocity.set(0, 0, 0);
        p.acceleration.set(0, 0, 0);
        p.size = ofRandom(1, 4);
        p.age = 0;
        p.maxAge = ofRandom(300, 800);
        p.alive = true;
        p.trail.clear();
        
        particles.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
    for(int i = 0; i < particles.size(); i++){
        Particle& p = particles[i];
        
        if(!p.alive) continue;
        
        // Get flow field force
        ofVec3f flowForce = getFlowFieldVector(p.position) * 2.0;
        
        // Add some randomness
        ofVec3f randomForce;
        randomForce.x = ofRandom(-0.5, 0.5);
        randomForce.y = ofRandom(-0.5, 0.5);
        randomForce.z = ofRandom(-0.5, 0.5);
        
        // Combine forces
        p.acceleration = flowForce + randomForce * 0.1;
        
        // Update physics
        p.velocity += p.acceleration;
        p.velocity *= 0.98; // damping
        p.position += p.velocity;
        
        // Update trail
        if(p.trail.size() > 0) {
            float dist = p.position.distance(p.trail.back());
            if(dist > 3.0) {
                p.trail.push_back(p.position);
                if(p.trail.size() > 20) {
                    p.trail.erase(p.trail.begin());
                }
            }
        } else {
            p.trail.push_back(p.position);
        }
        
        // Age and respawn
        p.age++;
        if(p.age > p.maxAge) {
            p.position.set(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
            p.velocity.set(0, 0, 0);
            p.age = 0;
            p.maxAge = ofRandom(300, 800);
            p.trail.clear();
        }
        
        // Boundary wrapping
        if(p.position.x > 400) p.position.x = -400;
        if(p.position.x < -400) p.position.x = 400;
        if(p.position.y > 400) p.position.y = -400;
        if(p.position.y < -400) p.position.y = 400;
        if(p.position.z > 400) p.position.z = -400;
        if(p.position.z < -400) p.position.z = 400;
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for(int i = 0; i < particles.size(); i++){
        Particle& p = particles[i];
        if(!p.alive) continue;
        
        // Calculate alpha based on age
        float ageAlpha = ofMap(p.age, 0, p.maxAge, 255, 30);
        
        // Draw particle
        ofSetColor(255, ageAlpha);
        ofPushMatrix();
        ofTranslate(p.position);
        ofDrawSphere(0, 0, 0, p.size);
        ofPopMatrix();
        
        // Draw trail
        if(p.trail.size() > 1) {
            for(int j = 1; j < p.trail.size(); j++) {
                float trailAlpha = ofMap(j, 0, p.trail.size() - 1, 10, ageAlpha * 0.5);
                ofSetColor(255, trailAlpha);
                ofDrawLine(p.trail[j-1], p.trail[j]);
            }
        }
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawFlowField(){
    ofSetColor(80);
    ofNoFill();
    
    for(int z = 0; z < flowField.depth; z += 2) {
        for(int y = 0; y < flowField.rows; y += 2) {
            for(int x = 0; x < flowField.cols; x += 2) {
                int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
                
                float xPos = (x - flowField.cols/2) * flowField.scale;
                float yPos = (y - flowField.rows/2) * flowField.scale;
                float zPos = (z - flowField.depth/2) * flowField.scale;
                
                ofVec3f start(xPos, yPos, zPos);
                ofVec3f end = start + flowField.vectors[index] * 20;
                
                ofDrawLine(start, end);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawConnections(){
    ofSetColor(120, 80);
    
    for(int i = 0; i < particles.size(); i += 3) {
        for(int j = i + 1; j < min(i + 8, (int)particles.size()); j++) {
            if(!particles[i].alive || !particles[j].alive) continue;
            
            float distance = particles[i].position.distance(particles[j].position);
            if(distance < 80) {
                float alpha = ofMap(distance, 0, 80, 60, 5);
                ofSetColor(255, alpha);
                ofDrawLine(particles[i].position, particles[j].position);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawWaveInterference(){
    ofSetColor(40);
    ofNoFill();
    
    // Draw wave interference patterns as concentric circles
    for(int w = 0; w < waves.size(); w++) {
        ofPushMatrix();
        
        // Position wave center based on time
        float centerX = cos(time * waves[w].frequency * 0.3 + waves[w].phase) * 100;
        float centerY = sin(time * waves[w].frequency * 0.2 + waves[w].phase) * 100;
        float centerZ = cos(time * waves[w].frequency * 0.4 + waves[w].phase) * 80;
        
        ofTranslate(centerX, centerY, centerZ);
        
        // Draw expanding rings
        for(int r = 1; r <= 8; r++) {
            float radius = (r * 30) + sin(time * waves[w].frequency + waves[w].phase) * 20;
            float alpha = ofMap(r, 1, 8, 60, 10);
            
            ofSetColor(255, alpha);
            ofDrawSphere(0, 0, 0, radius);
        }
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawGeometricPatterns(){
    ofSetColor(30);
    ofNoFill();
    
    // Draw rotating geometric wireframes
    ofPushMatrix();
    ofRotateDeg(time * 10, 1, 0.5, 0.3);
    
    // Nested cubes
    for(int i = 1; i <= 5; i++) {
        float size = i * 50 + sin(time + i) * 20;
        ofSetColor(255, 80 - i * 10);
        ofDrawBox(0, 0, 0, size);
    }
    
    ofPopMatrix();
    
    // Draw spiraling lines
    ofSetColor(60);
    for(int i = 0; i < 360; i += 10) {
        float angle = ofDegToRad(i);
        float radius = 150 + sin(time * 0.5 + angle) * 50;
        float height = cos(time * 0.3 + angle) * 100;
        
        ofVec3f pos1(cos(angle) * radius, height, sin(angle) * radius);
        ofVec3f pos2(cos(angle + 0.1) * radius, height + 5, sin(angle + 0.1) * radius);
        
        ofDrawLine(pos1, pos2);
    }
}

//--------------------------------------------------------------
ofVec3f ofApp::getFlowFieldVector(ofVec3f pos){
    // Convert world position to flow field coordinates
    int x = (int)ofMap(pos.x, -400, 400, 0, flowField.cols - 1);
    int y = (int)ofMap(pos.y, -400, 400, 0, flowField.rows - 1);
    int z = (int)ofMap(pos.z, -400, 400, 0, flowField.depth - 1);
    
    // Clamp to valid indices
    x = ofClamp(x, 0, flowField.cols - 1);
    y = ofClamp(y, 0, flowField.rows - 1);
    z = ofClamp(z, 0, flowField.depth - 1);
    
    int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
    
    if(index >= 0 && index < flowField.vectors.size()) {
        return flowField.vectors[index];
    }
    
    return ofVec3f(0, 0, 0);
}
