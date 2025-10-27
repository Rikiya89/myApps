#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(12, 8, 25); // Deep sacred purple
    ofSetCircleResolution(64);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    // Initialize parameters
    time = 0;
    breathingPhase = 0;
    centerPoint = ofVec2f(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    baseRadius = ofGetWidth() * 0.10; // Smaller for Instagram - more elegant and mobile-friendly
    
    // Sacred color palette - clean and elegant
    sacredColors.push_back(ofColor(255, 215, 0, 200));   // Sacred gold
    sacredColors.push_back(ofColor(255, 255, 255, 180)); // Pure light
    sacredColors.push_back(ofColor(200, 255, 200, 160)); // Life green
    sacredColors.push_back(ofColor(180, 200, 255, 160)); // Celestial blue
    sacredColors.push_back(ofColor(255, 180, 255, 160)); // Divine violet
    
    // Setup clean FBOs
    glowFbo.allocate(ofGetWidth(), ofGetHeight());
    particleFbo.allocate(ofGetWidth(), ofGetHeight());
    
    glowFbo.begin(); ofClear(0, 0, 0, 0); glowFbo.end();
    particleFbo.begin(); ofClear(0, 0, 0, 0); particleFbo.end();
    
    setupFlowerOfLife();
}

//--------------------------------------------------------------
void ofApp::setupFlowerOfLife(){
    flowerCenters.clear();
    flowerNodes.clear();
    
    // Central circle
    flowerCenters.push_back(centerPoint);
    
    // First ring - 6 circles around center
    for(int i = 0; i < 6; i++) {
        float angle = (i / 6.0) * TWO_PI;
        ofVec2f pos = centerPoint + ofVec2f(cos(angle), sin(angle)) * baseRadius;
        flowerCenters.push_back(pos);
    }
    
    // Second ring - 12 circles for complete Flower of Life
    for(int i = 0; i < 12; i++) {
        float angle = (i / 12.0) * TWO_PI;
        float radius = baseRadius * sqrt(3); // Perfect geometric spacing
        ofVec2f pos = centerPoint + ofVec2f(cos(angle), sin(angle)) * radius;
        flowerCenters.push_back(pos);
    }
    
    // Create geometry nodes at intersection points
    for(int i = 0; i < flowerCenters.size(); i++) {
        GeometryNode node;
        node.position = flowerCenters[i];
        node.radius = baseRadius * 0.8;
        node.pulsePhase = i * PI / 6;
        node.intensity = 1.0;
        node.color = sacredColors[i % sacredColors.size()];
        flowerNodes.push_back(node);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01f;
    breathingPhase += 0.018f;
    
    updateParticles();
    updateGeometryNodes();
    
    // Add particles flowing toward flower geometry - Instagram optimized
    if (ofGetFrameNum() % 10 == 0 && particles.size() < 100) { // Perfect particle count for Instagram
        addFlowerParticle();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Gentle fade for trails
    particleFbo.begin();
    ofSetColor(12, 8, 25, 8);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    particleFbo.end();
    
    // Draw Flower of Life geometry
    drawFlowerOfLife();
    
    // Draw sacred center mandala
    drawCenterMandala();
    
    // Draw geometry nodes
    drawGeometryNodes();
    
    // Draw sacred connections
    drawSacredConnections();
    
    // Draw particles
    drawParticles();
    
    // Apply clean glow effects
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofSetColor(255, 150);
    particleFbo.draw(0, 0);
    
    ofSetColor(255, 120);
    glowFbo.draw(0, 0);
    
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::drawFlowerOfLife(){
    ofSetLineWidth(2);
    ofNoFill();
    
    // Draw the sacred circles of Flower of Life
    for(int i = 0; i < flowerCenters.size(); i++) {
        float breathScale = 1.0 + 0.1 * sin(breathingPhase + i * 0.3);
        ofColor circleColor = getSacredColor(time * 0.5 + i * 0.5);
        
        // Main circle
        ofSetColor(circleColor.r, circleColor.g, circleColor.b, 80);
        ofDrawCircle(flowerCenters[i], baseRadius * breathScale);
        
        // Subtle inner glow
        ofSetColor(circleColor.r, circleColor.g, circleColor.b, 40);
        ofDrawCircle(flowerCenters[i], (baseRadius * breathScale) * 0.8);
        
        // Outer subtle ring
        ofSetColor(circleColor.r, circleColor.g, circleColor.b, 20);
        ofDrawCircle(flowerCenters[i], (baseRadius * breathScale) * 1.2);
    }
}

//--------------------------------------------------------------
void ofApp::drawCenterMandala(){
    ofSetLineWidth(3);
    ofNoFill();
    
    // Central breathing mandala - adjusted for portrait
    for(int i = 0; i < 5; i++) { // Reduced layers for cleaner look in portrait
        float breathScale = 1.0 + 0.2 * sin(breathingPhase * 1.5 + i * 0.4);
        ofColor mandalaColor = getSacredColor(time + i * PI / 3);
        
        float radius = (15 + i * 8) * breathScale; // Smaller central mandala for Instagram
        
        // Multiple glow layers for depth
        for(int glow = 0; glow < 3; glow++) {
            float alpha = mandalaColor.a * (1.0 - glow * 0.4);
            ofSetColor(mandalaColor.r, mandalaColor.g, mandalaColor.b, alpha);
            ofDrawCircle(centerPoint, radius + glow * 2);
        }
        
        // Sacred geometry rays - adjusted spacing for portrait
        if(i % 2 == 0) {
            ofSetColor(mandalaColor.r, mandalaColor.g, mandalaColor.b, 60);
            for(int j = 0; j < 8; j++) { // Reduced rays for cleaner portrait look
                float angle = (j / 8.0) * TWO_PI + time * 0.3;
                ofVec2f start = centerPoint + ofVec2f(cos(angle), sin(angle)) * radius * 0.7;
                ofVec2f end = centerPoint + ofVec2f(cos(angle), sin(angle)) * radius * 1.3;
                ofDrawLine(start, end);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawGeometryNodes(){
    glowFbo.begin();
    
    for(auto& node : flowerNodes) {
        float pulse = sin(node.pulsePhase) * 0.3 + 0.7;
        
        // Node glow with breathing effect - Instagram optimized
        for(int i = 0; i < 3; i++) {
            float size = 4 + i * 1.5; // Smaller, more delicate nodes for Instagram
            float alpha = node.color.a * pulse * (1.0 - i * 0.3);
            ofSetColor(node.color.r, node.color.g, node.color.b, alpha);
            ofDrawCircle(node.position, size);
        }
    }
    
    glowFbo.end();
}

//--------------------------------------------------------------
void ofApp::drawSacredConnections(){
    ofSetLineWidth(1);
    
    // Draw connections between flower centers
    for(int i = 0; i < flowerCenters.size(); i++) {
        for(int j = i + 1; j < flowerCenters.size(); j++) {
            float distance = flowerCenters[i].distance(flowerCenters[j]);
            
            // Only connect geometrically significant distances
            if(abs(distance - baseRadius) < 5 || abs(distance - baseRadius * sqrt(3)) < 5) {
                ofColor connectionColor = getSacredColor(time * 2 + i + j);
                ofSetColor(connectionColor.r, connectionColor.g, connectionColor.b, 30);
                ofDrawLine(flowerCenters[i], flowerCenters[j]);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles(){
    particleFbo.begin();
    
    for(auto& particle : particles) {
        // Particle with soft glow
        for(int i = 0; i < 3; i++) {
            float size = particle.size * particle.brightness + i * 1.5;
            float alpha = particle.color.a * (1.0 - i * 0.35);
            ofSetColor(particle.color.r, particle.color.g, particle.color.b, alpha);
            ofDrawCircle(particle.position, size);
        }
        
        // Connect particles that are close to flower geometry
        if(particle.isNodeParticle) {
            ofSetColor(particle.color.r, particle.color.g, particle.color.b, 20);
            ofSetLineWidth(0.5);
            
            // Find nearest flower center
            float minDist = 1000;
            ofVec2f nearestCenter;
            for(auto& center : flowerCenters) {
                float dist = particle.position.distance(center);
                if(dist < minDist) {
                    minDist = dist;
                    nearestCenter = center;
                }
            }
            
            if(minDist < baseRadius) {
                ofDrawLine(particle.position, nearestCenter);
            }
        }
    }
    
    particleFbo.end();
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
    for (auto it = particles.begin(); it != particles.end();) {
        it->life -= 0.5f;
        
        if (it->life <= 0) {
            it = particles.erase(it);
            continue;
        }
        
        // Flow toward nearest flower geometry
        ofVec2f flowDirection = getFlowTowardsFlower(it->position);
        it->velocity += flowDirection * 0.02f;
        it->velocity *= 0.97f; // Gentle damping
        
        it->position += it->velocity;
        
        // Update brightness based on proximity to sacred geometry
        it->brightness = isInsideFlowerCircle(it->position) ? 1.2 : 0.8;
        
        // Check if particle becomes a node particle
        for(auto& center : flowerCenters) {
            if(it->position.distance(center) < baseRadius * 0.3) {
                it->isNodeParticle = true;
                break;
            }
        }
        
        ++it;
    }
}

//--------------------------------------------------------------
void ofApp::updateGeometryNodes(){
    for(auto& node : flowerNodes) {
        node.pulsePhase += 0.04f;
        
        // Breathing intensity
        node.intensity = 0.7 + 0.3 * sin(breathingPhase + node.pulsePhase);
    }
}

//--------------------------------------------------------------
void ofApp::addFlowerParticle(){
    FlowerParticle p;
    
    // Spawn from edges, flowing toward flower geometry
    int edge = (int)ofRandom(4);
    switch(edge) {
        case 0: p.position = ofVec2f(ofRandom(ofGetWidth()), 0); break;
        case 1: p.position = ofVec2f(ofGetWidth(), ofRandom(ofGetHeight())); break;
        case 2: p.position = ofVec2f(ofRandom(ofGetWidth()), ofGetHeight()); break;
        case 3: p.position = ofVec2f(0, ofRandom(ofGetHeight())); break;
    }
    
    p.velocity = getFlowTowardsFlower(p.position) * 0.5;
    p.life = ofRandom(400, 700);
    p.maxLife = p.life;
    p.size = ofRandom(1, 3);
    p.brightness = 1.0;
    p.color = sacredColors[(int)ofRandom(sacredColors.size())];
    p.isNodeParticle = false;
    
    particles.push_back(p);
}

//--------------------------------------------------------------
ofVec2f ofApp::getFlowTowardsFlower(ofVec2f pos){
    // Find nearest flower center
    float minDistance = 10000;
    ofVec2f nearestCenter = centerPoint;
    
    for(auto& center : flowerCenters) {
        float distance = pos.distance(center);
        if(distance < minDistance) {
            minDistance = distance;
            nearestCenter = center;
        }
    }
    
    // Flow toward nearest flower center with gentle spiral
    ofVec2f toCenter = (nearestCenter - pos).normalize();
    ofVec2f perpendicular(-toCenter.y, toCenter.x);
    
    float spiralStrength = 0.2 * sin(time * 1.5 + minDistance * 0.01);
    float noiseInfluence = ofNoise(pos.x * 0.01, pos.y * 0.01, time * 0.3) * 0.1;
    
    return toCenter + perpendicular * spiralStrength + ofVec2f(noiseInfluence, noiseInfluence);
}

//--------------------------------------------------------------
ofColor ofApp::getSacredColor(float phase){
    // Gentle color cycling through sacred spectrum
    float r = sin(phase) * 80 + 175;
    float g = sin(phase + TWO_PI / 3) * 80 + 175;
    float b = sin(phase + 4 * TWO_PI / 3) * 80 + 175;
    
    return ofColor(r, g, b, 150);
}

//--------------------------------------------------------------
bool ofApp::isInsideFlowerCircle(ofVec2f pos){
    for(auto& center : flowerCenters) {
        if(pos.distance(center) < baseRadius) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
void ofApp::gotMessage(ofMessage msg){}
