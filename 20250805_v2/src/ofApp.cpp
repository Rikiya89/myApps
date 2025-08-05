#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0, 5, 15);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    // Initialize Pandora color palette
    biolumBlue = ofColor(0, 150, 255, 200);
    biolumPurple = ofColor(150, 50, 255, 200);
    biolumTeal = ofColor(0, 255, 200, 200);
    biolumGreen = ofColor(50, 255, 100, 200);
    biolumViolet = ofColor(100, 0, 255, 200);
    
    setupScene();
    
    time = 0;
    useAutoCam = true;
    cameraTarget = ofVec3f(0, 100, 0);
    cameraRadius = 600;
    cameraAngle = 0;
    masterPulse = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    masterPulse = sin(time * 0.7) * 0.5 + 0.5;
    
    // Update cinematic camera
    if(useAutoCam){
        cameraAngle += ofGetLastFrameTime() * 0.1;
        cameraRadius = 400 + sin(time * 0.3) * 200;
        
        float cameraHeight = 150 + sin(time * 0.2) * 100;
        ofVec3f cameraPos(
            cos(cameraAngle) * cameraRadius,
            cameraHeight,
            sin(cameraAngle) * cameraRadius
        );
        
        cameraTarget.x = sin(time * 0.1) * 100;
        cameraTarget.z = cos(time * 0.15) * 100;
        
        cam.setPosition(cameraPos);
        cam.lookAt(cameraTarget);
    }
    
    updateParticles();
    updateTrees();
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    // Enable additive blending for glow effect
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    drawGroundGlow();
    drawTrees();
    drawParticles();
    
    ofDisableBlendMode();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::setupScene(){
    // Create bioluminescent trees
    for(int i = 0; i < 25; i++){
        BioTree tree;
        tree.position = ofVec3f(
            ofRandom(-800, 800),
            0,
            ofRandom(-800, 800)
        );
        tree.height = ofRandom(100, 400);
        tree.pulseSpeed = ofRandom(0.5, 2.0);
        tree.phase = ofRandom(0, TWO_PI);
        tree.swayPhase = ofRandom(0, TWO_PI);
        tree.swayAmount = ofRandom(10, 30);
        tree.growthPhase = ofRandom(0, TWO_PI);
        tree.baseColor = getPandoraColor(i % 5);
        
        // Generate branch positions
        int numBranches = ofRandom(5, 15);
        for(int j = 0; j < numBranches; j++){
            float branchHeight = ofRandom(0.3, 0.9) * tree.height;
            float angle = ofRandom(0, TWO_PI);
            float radius = ofRandom(20, 80);
            tree.branches.push_back(ofVec3f(
                tree.position.x + cos(angle) * radius,
                branchHeight,
                tree.position.z + sin(angle) * radius
            ));
            tree.branchPhases.push_back(ofRandom(0, TWO_PI));
        }
        bioTrees.push_back(tree);
    }
    
    // Create floating particles
    for(int i = 0; i < 500; i++){
        Particle p;
        p.position = ofVec3f(
            ofRandom(-1000, 1000),
            ofRandom(0, 300),
            ofRandom(-1000, 1000)
        );
        p.originalPos = p.position;
        p.velocity = ofVec3f(
            ofRandom(-0.5, 0.5),
            ofRandom(-0.2, 0.2),
            ofRandom(-0.5, 0.5)
        );
        p.acceleration = ofVec3f(0, 0, 0);
        p.life = ofRandom(0.5, 1.0);
        p.maxLife = p.life;
        p.size = ofRandom(2, 8);
        p.phase = ofRandom(0, TWO_PI);
        p.swayAmount = ofRandom(20, 60);
        p.color = getPandoraColor(i % 5);
        particles.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
    for(int i = 0; i < particles.size(); i++){
        auto& p = particles[i];
        
        // Update phase for swaying motion
        p.phase += ofGetLastFrameTime() * 2.0;
        
        // Flowing swarm behavior - attraction to other nearby particles
        ofVec3f swarmForce(0, 0, 0);
        int nearbyCount = 0;
        
        for(int j = 0; j < particles.size(); j++){
            if(i != j){
                float dist = p.position.distance(particles[j].position);
                if(dist < 100 && dist > 0){
                    ofVec3f direction = particles[j].position - p.position;
                    direction.normalize();
                    swarmForce += direction * (50.0 / dist);
                    nearbyCount++;
                }
            }
        }
        
        if(nearbyCount > 0){
            swarmForce /= nearbyCount;
            swarmForce *= 0.02;
        }
        
        // Organic swaying motion
        ofVec3f swayForce;
        swayForce.x = sin(p.phase) * p.swayAmount * 0.01;
        swayForce.y = cos(p.phase * 0.7) * p.swayAmount * 0.005;
        swayForce.z = sin(p.phase * 1.3) * p.swayAmount * 0.01;
        
        // Wind-like global flow
        ofVec3f windForce;
        windForce.x = sin(time * 0.3 + p.position.z * 0.01) * 0.02;
        windForce.y = sin(time * 0.5 + p.position.x * 0.01) * 0.01;
        windForce.z = cos(time * 0.4 + p.position.y * 0.01) * 0.02;
        
        // Apply forces
        p.acceleration = swarmForce + swayForce + windForce;
        p.velocity += p.acceleration;
        p.velocity *= 0.98; // Damping
        p.position += p.velocity;
        
        // Life cycle
        p.life -= ofGetLastFrameTime() * 0.2;
        
        // Respawn particles with flowing entrance
        if(p.life <= 0){
            p.position = ofVec3f(
                ofRandom(-1000, 1000),
                ofRandom(0, 300),
                ofRandom(-1000, 1000)
            );
            p.originalPos = p.position;
            p.life = p.maxLife;
            p.velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.2, 0.2), ofRandom(-0.5, 0.5));
            p.color = getPandoraColor(ofRandom(0, 5));
        }
        
        // Update alpha and size based on life and master pulse
        float alpha = (p.life / p.maxLife) * 180 * (0.7 + 0.3 * masterPulse);
        p.color.a = alpha;
        p.size = p.size * (0.8 + 0.4 * sin(p.phase));
    }
}

//--------------------------------------------------------------
void ofApp::updateTrees(){
    for(auto& tree : bioTrees){
        tree.phase += tree.pulseSpeed * ofGetLastFrameTime();
        tree.swayPhase += ofGetLastFrameTime() * 0.8;
        tree.growthPhase += ofGetLastFrameTime() * 0.5;
        
        // Update branch phases for individual movement
        for(auto& branchPhase : tree.branchPhases){
            branchPhase += ofGetLastFrameTime() * ofRandom(0.5, 1.5);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles(){
    ofSetLineWidth(2);
    
    for(const auto& p : particles){
        ofSetColor(p.color);
        ofDrawSphere(p.position, p.size);
        
        // Add glow effect
        ofColor glowColor = p.color;
        glowColor.a *= 0.3;
        ofSetColor(glowColor);
        ofDrawSphere(p.position, p.size * 2);
    }
}

//--------------------------------------------------------------
void ofApp::drawTrees(){
    for(const auto& tree : bioTrees){
        float pulse = sin(tree.phase) * 0.5 + 0.5;
        float sway = sin(tree.swayPhase) * tree.swayAmount;
        float growth = sin(tree.growthPhase) * 0.2 + 0.8;
        
        ofColor treeColor = tree.baseColor;
        treeColor.a = 80 + pulse * 120;
        
        // Calculate swaying trunk position
        ofVec3f trunkTop = ofVec3f(
            tree.position.x + sway,
            tree.height * growth,
            tree.position.z + cos(tree.swayPhase * 0.7) * tree.swayAmount * 0.5
        );
        
        ofSetColor(treeColor);
        
        // Draw swaying trunk with multiple segments for smooth curve
        int segments = 8;
        for(int i = 0; i < segments; i++){
            float t = (float)i / segments;
            float nextT = (float)(i + 1) / segments;
            
            float currentSway = sway * t * t; // Quadratic falloff from base
            float nextSway = sway * nextT * nextT;
            
            ofVec3f currentPos(
                tree.position.x + currentSway,
                tree.position.y + tree.height * t * growth,
                tree.position.z + cos(tree.swayPhase * 0.7) * tree.swayAmount * 0.5 * t * t
            );
            
            ofVec3f nextPos(
                tree.position.x + nextSway,
                tree.position.y + tree.height * nextT * growth,
                tree.position.z + cos(tree.swayPhase * 0.7) * tree.swayAmount * 0.5 * nextT * nextT
            );
            
            float radius = (15 + pulse * 8) * (1.0 - t * 0.7);
            
            ofPushMatrix();
            ofTranslate(currentPos);
            
            ofVec3f direction = nextPos - currentPos;
            float height = direction.length();
            direction.normalize();
            
            // Align cylinder with direction vector
            ofVec3f up(0, 1, 0);
            ofVec3f axis = up.getCrossed(direction);
            float angle = acos(up.dot(direction)) * 180.0 / PI;
            
            if(axis.length() > 0.001) {
                ofRotateDeg(angle, axis.x, axis.y, axis.z);
            }
            
            ofDrawCylinder(0, 0, 0, radius, height);
            ofPopMatrix();
        }
        
        // Draw animated branches  
        for(int i = 0; i < tree.branches.size(); i++){
            const auto& branch = tree.branches[i];
            float branchPulse = sin(tree.branchPhases[i]) * 0.5 + 0.5;
            float branchSway = sin(tree.branchPhases[i] * 0.8) * 15;
            
            // Calculate branch connection point on swaying trunk
            float branchT = branch.y / tree.height;
            float connectionSway = sway * branchT * branchT;
            ofVec3f connectionPoint(
                tree.position.x + connectionSway,
                branch.y * growth,
                tree.position.z + cos(tree.swayPhase * 0.7) * tree.swayAmount * 0.5 * branchT * branchT
            );
            
            // Calculate swaying branch end
            ofVec3f branchEnd(
                branch.x + branchSway,
                branch.y * growth,
                branch.z + sin(tree.branchPhases[i] * 0.6) * 10
            );
            
            // Draw branch
            ofColor branchColor = treeColor;
            branchColor.a = 120 + branchPulse * 80;
            ofSetColor(branchColor);
            
            ofSetLineWidth(3 + branchPulse * 2);
            ofDrawLine(connectionPoint, branchEnd);
            
            // Branch endpoint glow with pulsing
            ofColor branchGlow = branchColor;
            branchGlow.a = 180 + branchPulse * 75;
            ofSetColor(branchGlow);
            ofDrawSphere(branchEnd, 6 + branchPulse * 8);
            
            // Add smaller glowing orbs around branch ends
            for(int j = 0; j < 3; j++){
                float orbAngle = tree.branchPhases[i] + j * TWO_PI / 3;
                ofVec3f orbPos = branchEnd + ofVec3f(
                    cos(orbAngle) * 20,
                    sin(orbAngle * 0.7) * 10,
                    sin(orbAngle) * 20
                );
                
                ofColor orbColor = getPandoraColor((i + j) % 5);
                orbColor.a = 100 + sin(tree.branchPhases[i] + j) * 50;
                ofSetColor(orbColor);
                ofDrawSphere(orbPos, 3 + sin(tree.branchPhases[i] + j) * 2);
            }
        }
        
        // Animated tree top with multiple glowing elements
        ofColor topGlow = treeColor;
        topGlow.a = 150 + pulse * 105;
        ofSetColor(topGlow);
        
        // Main top glow
        ofDrawSphere(trunkTop, 12 + pulse * 15);
        
        // Floating energy orbs around tree top
        for(int i = 0; i < 5; i++){
            float orbAngle = tree.phase + i * TWO_PI / 5;
            float orbRadius = 30 + sin(tree.phase + i) * 10;
            ofVec3f orbPos = trunkTop + ofVec3f(
                cos(orbAngle) * orbRadius,
                sin(orbAngle * 0.8) * 20,
                sin(orbAngle) * orbRadius
            );
            
            ofColor orbColor = getPandoraColor(i);
            orbColor.a = 120 + sin(tree.phase + i * 1.3) * 80;
            ofSetColor(orbColor);
            ofDrawSphere(orbPos, 4 + sin(tree.phase + i * 2) * 3);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawGroundGlow(){
    // Dynamic ground patterns with multiple wave layers
    for(int x = -1000; x < 1000; x += 80){
        for(int z = -1000; z < 1000; z += 80){
            float dist = sqrt(x*x + z*z);
            
            // Multiple wave layers for complex animation
            float wave1 = sin(time * 0.6 + dist * 0.008) * 25;
            float wave2 = cos(time * 0.4 + dist * 0.012 + PI/2) * 15;
            float wave3 = sin(time * 0.8 + x * 0.01 + z * 0.01) * 10;
            
            float combinedWave = (wave1 + wave2 + wave3) * masterPulse;
            
            // Create ripple effect from center
            float ripple = sin(time * 2.0 - dist * 0.02) * 30;
            
            // Color cycling through Pandora palette
            int colorIndex = (int)(time * 0.5 + dist * 0.1) % 5;
            ofColor groundColor = getPandoraColor(colorIndex);
            groundColor.a = 20 + abs(combinedWave) + abs(ripple) * 0.5;
            
            ofSetColor(groundColor);
            
            // Draw glowing circles with varying sizes
            float circleSize = 40 + abs(combinedWave) * 0.8;
            ofDrawCircle(x, combinedWave * 0.15, z, circleSize);
            
            // Add smaller accent circles for detail
            if((x + z) % 160 == 0){
                ofColor accentColor = getPandoraColor((colorIndex + 2) % 5);
                accentColor.a = 60 + sin(time * 1.5 + dist * 0.01) * 40;
                ofSetColor(accentColor);
                ofDrawCircle(x, combinedWave * 0.15 + 5, z, 15);
            }
        }
    }
    
    // Add flowing energy streams across the ground
    for(int i = 0; i < 8; i++){
        float streamAngle = i * TWO_PI / 8 + time * 0.1;
        float streamRadius = 300 + sin(time * 0.3 + i) * 200;
        
        for(int j = 0; j < 20; j++){
            float t = j / 20.0;
            float currentRadius = streamRadius * (0.3 + t * 0.7);
            
            ofVec3f streamPos(
                cos(streamAngle) * currentRadius,
                sin(time * 2.0 + i + j * 0.5) * 15,
                sin(streamAngle) * currentRadius
            );
            
            ofColor streamColor = getPandoraColor(i % 5);
            streamColor.a = (100 * (1.0 - t)) * masterPulse;
            ofSetColor(streamColor);
            
            ofDrawSphere(streamPos, 8 - t * 6);
        }
    }
}

//--------------------------------------------------------------
ofColor ofApp::getPandoraColor(int type){
    switch(type % 5){
        case 0: return biolumBlue;
        case 1: return biolumPurple;
        case 2: return biolumTeal;
        case 3: return biolumGreen;
        case 4: return biolumViolet;
        default: return biolumBlue;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        useAutoCam = !useAutoCam;
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
