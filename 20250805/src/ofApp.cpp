#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetSphereResolution(30);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    // Setup 3D camera with smooth auto-movement
    cam.setDistance(600);
    cam.setNearClip(1);
    cam.setFarClip(10000);
    cam.setAutoDistance(false);
    
    // Setup much brighter dramatic lighting
    light.setPointLight();
    light.setPosition(400, -200, 600);
    light.setDiffuseColor(ofColor(180, 200, 255));
    light.setSpecularColor(ofColor(255, 255, 255));
    light.setAttenuation(0.1, 0.001, 0.0001);
    
    setupColors();
    generate3DStructure();
    
    cout << "Setup complete. Generated " << nodes.size() << " nodes." << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    updateParticles3D();
    
    float time = ofGetElapsedTimef();
    
    // Animate 3D nodes with organic movement
    for (int i = 0; i < nodes.size(); i++) {
        Node3D& node = nodes[i];
        
        // Multi-layered wave animation
        float wave1 = sin(time * 0.8 + node.phase);
        float wave2 = cos(time * 0.5 + node.phase * 1.3);
        float wave3 = sin(time * 0.3 + node.phase * 0.7);
        
        // Apply waves to position
        node.position.x = node.originalPos.x + wave1 * 30;
        node.position.y = node.originalPos.y + wave2 * 25;
        node.position.z = node.originalPos.z + wave3 * 35;
        
        // Animate radius
        node.radius = 15 + wave1 * 8 + wave2 * 5;
        
        // Spawn more particles from active nodes
        if (abs(wave1) > 0.5 && ofRandom(1.0) < 0.4) {
            spawnParticles3D(node.position, (abs(wave1) - 0.5) * 2.0);
        }
        
        // Extra particle bursts from highly active nodes
        if (abs(wave1) > 0.9 && ofRandom(1.0) < 0.6) {
            spawnParticles3D(node.position, (abs(wave1) - 0.9) * 3.0);
        }
    }
    
    // Update light position for dramatic movement
    light.setPosition(cos(time * 0.2) * 600, sin(time * 0.15) * 400, 300 + sin(time * 0.1) * 200);
    
    // Auto-rotate camera for cinematic viewing
    float camX = cos(time * 0.1) * 800;
    float camY = sin(time * 0.08) * 300;
    float camZ = 400 + sin(time * 0.05) * 200;
    
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(ofVec3f(0, 0, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    
    // Set deep navy background
    ofBackground(deepNavy);
    
    float time = ofGetElapsedTimef();
    
    // Add screen-space glow effects
    ofFill();
    for (int i = 0; i < 3; i++) {
        float alpha = ofMap(i, 0, 2, 8, 2);
        float size = ofMap(i, 0, 2, ofGetWidth() * 1.2, ofGetWidth() * 0.6);
        ofSetColor(accentBlue, alpha);
        ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, size + sin(time * 0.5 + i) * 100);
    }
    
    ofEnableLighting();
    cam.begin();
    light.enable();
    
    // Draw atmospheric background with enhanced effects
    drawAtmosphere();
    
    // Draw connecting neural network
    drawConnections();
    
    // Draw 3D nodes with enhanced visuals
    for (const Node3D& node : nodes) {
        ofPushMatrix();
        ofTranslate(node.position);
        
        float pulse = sin(time * 2.0 + node.phase) * 0.3 + 1.0;
        
        // Massive outer glow
        ofSetColor(node.color, 60 * pulse);
        ofDrawSphere(0, 0, 0, node.radius * 4.0 * pulse);
        
        // Secondary glow
        ofSetColor(node.color, 120 * pulse);
        ofDrawSphere(0, 0, 0, node.radius * 2.5 * pulse);
        
        // Main sphere - much brighter
        ofSetColor(node.color, 255);
        ofDrawSphere(0, 0, 0, node.radius * pulse);
        
        // Bright pulsing core
        ofSetColor(glowBlue, 255);
        ofDrawSphere(0, 0, 0, node.radius * 0.7 * pulse);
        
        // Ultra bright center
        ofSetColor(255, 255, 255, 255);
        ofDrawSphere(0, 0, 0, node.radius * 0.4 * pulse);
        
        // Sparkling center dot
        if (pulse > 1.2) {
            ofSetColor(255, 255, 255, 255);
            ofDrawSphere(0, 0, 0, node.radius * 0.1);
        }
        
        ofPopMatrix();
    }
    
    // Draw 3D particles
    drawParticles3D();
    
    light.disable();
    cam.end();
    
    ofDisableLighting();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::setupColors(){
    // Enhanced Avatar-inspired navy blue palette - much brighter!
    deepNavy = ofColor(5, 15, 35);        // Deep ocean blue
    mediumNavy = ofColor(60, 100, 180);   // Brighter navy
    lightNavy = ofColor(120, 160, 220);   // Much lighter navy
    accentBlue = ofColor(160, 200, 255);  // Bright accent blue
    glowBlue = ofColor(200, 230, 255);    // Ultra bright glow blue
}

//--------------------------------------------------------------
void ofApp::generate3DStructure(){
    nodes.clear();
    
    // Create multiple layers of 3D nodes
    for (int layer = 0; layer < 5; layer++) {
        float layerZ = (layer - 2) * 120;
        int nodesInLayer = 8 + layer * 2;
        
        for (int i = 0; i < nodesInLayer; i++) {
            Node3D node;
            
            // Arrange in spiraling pattern
            float angle = (float)i / nodesInLayer * TWO_PI + layer * 0.5;
            float radius = 100 + layer * 80 + sin(layer * 0.8) * 40;
            
            node.originalPos.x = cos(angle) * radius;
            node.originalPos.y = sin(angle) * radius;
            node.originalPos.z = layerZ + sin(angle * 3) * 30;
            
            node.position = node.originalPos;
            node.phase = ofRandom(TWO_PI);
            node.radius = 15 + ofRandom(-5, 5);
            
            // Assign colors based on layer
            float colorMix = (float)layer / 4.0;
            node.color = mediumNavy.getLerped(accentBlue, colorMix);
            
            nodes.push_back(node);
        }
    }
    
    // Add central core nodes
    for (int i = 0; i < 6; i++) {
        Node3D node;
        node.originalPos = ofVec3f(ofRandom(-50, 50), ofRandom(-50, 50), ofRandom(-50, 50));
        node.position = node.originalPos;
        node.phase = ofRandom(TWO_PI);
        node.radius = 20 + ofRandom(-5, 8);
        node.color = glowBlue;
        
        nodes.push_back(node);
    }
}

//--------------------------------------------------------------
void ofApp::updateParticles3D(){
    for (int i = particles.size() - 1; i >= 0; i--) {
        Particle3D& p = particles[i];
        
        p.position += p.velocity;
        p.velocity *= 0.95; // Slight damping
        p.life -= ofGetLastFrameTime();
        p.alpha = ofMap(p.life, 0, p.maxLife, 0, 255);
        
        if (p.life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles3D(){
    ofDisableLighting();
    
    for (const Particle3D& p : particles) {
        ofPushMatrix();
        ofTranslate(p.position);
        
        float size = p.size * (p.life / p.maxLife);
        float lifeFactor = p.life / p.maxLife;
        
        // Main particle with glow
        ofSetColor(p.color, p.alpha * lifeFactor);
        ofDrawSphere(0, 0, 0, size);
        
        // Outer glow
        ofSetColor(p.color, p.alpha * 0.3 * lifeFactor);
        ofDrawSphere(0, 0, 0, size * 2.5);
        
        // Bright core
        if (p.color.r > 200) { // For white sparkles
            ofSetColor(255, 255, 255, p.alpha * lifeFactor);
            ofDrawSphere(0, 0, 0, size * 0.5);
        }
        
        // Enhanced sparkle effects
        if (ofRandom(1.0) < 0.3) {
            ofSetColor(glowBlue, p.alpha * 0.8 * lifeFactor);
            for (int i = 0; i < 3; i++) {
                ofDrawSphere(ofRandom(-size*3, size*3), 
                            ofRandom(-size*3, size*3),
                            ofRandom(-size*3, size*3), 
                            size * 0.3);
            }
        }
        
        ofPopMatrix();
    }
    
    ofEnableLighting();
}

//--------------------------------------------------------------
void ofApp::drawConnections(){
    ofDisableLighting();
    ofSetLineWidth(1.0);
    
    float time = ofGetElapsedTimef();
    
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            float dist = nodes[i].position.distance(nodes[j].position);
            
            if (dist < 200) {
                float alpha = ofMap(dist, 0, 200, 60, 10);
                alpha *= sin(time * 1.2 + i * 0.1 + j * 0.1) * 0.5 + 0.5;
                
                ofSetColor(accentBlue, alpha);
                ofDrawLine(nodes[i].position, nodes[j].position);
            }
        }
    }
    
    ofEnableLighting();
}

//--------------------------------------------------------------
void ofApp::drawAtmosphere(){
    ofDisableLighting();
    
    float time = ofGetElapsedTimef();
    
    // Create layered atmospheric spheres
    for (int i = 0; i < 4; i++) {
        ofPushMatrix();
        
        float scale = 1.0 + i * 0.8;
        float alpha = ofMap(i, 0, 3, 20, 5);
        
        ofScale(scale);
        ofRotateYDeg(time * 5 + i * 30);
        ofRotateXDeg(time * 3 + i * 20);
        
        ofSetColor(deepNavy, alpha);
        ofDrawSphere(0, 0, 0, 400);
        
        ofPopMatrix();
    }
    
    ofEnableLighting();
}

//--------------------------------------------------------------
void ofApp::spawnParticles3D(ofVec3f position, float intensity){
    int numParticles = ofRandom(8, 15) * intensity; // Much more particles!
    
    for (int i = 0; i < numParticles; i++) {
        Particle3D p;
        p.position = position + ofVec3f(ofRandom(-20, 20), ofRandom(-20, 20), ofRandom(-20, 20));
        
        // Random 3D direction with more dramatic spread
        float theta = ofRandom(TWO_PI);
        float phi = ofRandom(-PI/2, PI/2);
        float speed = ofRandom(2.0, 6.0) * intensity;
        
        p.velocity = ofVec3f(
            cos(theta) * cos(phi) * speed,
            sin(theta) * cos(phi) * speed,
            sin(phi) * speed
        );
        
        p.maxLife = ofRandom(4.0, 8.0); // Longer lasting particles
        p.life = p.maxLife;
        p.size = ofRandom(3.0, 8.0); // Larger particles
        p.alpha = 255;
        
        // More varied and brighter colors
        if (ofRandom(1.0) < 0.3) {
            p.color = ofColor(255, 255, 255); // Pure white sparkles
        } else {
            p.color = lightNavy.getLerped(glowBlue, ofRandom(0.5, 1.0));
        }
        
        particles.push_back(p);
    }
}
