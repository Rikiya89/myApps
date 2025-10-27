#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetCircleResolution(32);
    ofSetSphereResolution(16);
    
    // Set up camera
    cam.setDistance(300);
    cam.setNearClip(1);
    cam.setFarClip(5000);
    
    // Initialize parameters
    animate = true;
    time = 0.0f;
    timeSpeed = 1.0f;
    blackOnWhite = false;
    
    // Enhanced visual modes
    showTrails = true;
    showBloom = true;
    showFractals = true;
    showFlowFields = true;
    smoothCameraMode = false; // Start with manual camera
    
    maxNodes = 80;
    nodeSpeed = 20.0f;
    connectionDistance = 60.0f;
    spawnRate = 0.4f;
    nodeLife = 10.0f;
    
    strokeWeight = 1.0f;
    nodeSize = 2.0f;
    showNodes = true;
    showConnections = true;
    
    noiseScale = 0.008f;
    noiseStrength = 10.0f;
    
    // Setup trail system
    setupTrails();
    
    // Spawn initial nodes
    for(int i = 0; i < 10; i++) {
        spawnNode();
    }
    
    // Initialize simple ribbons
    for(int i = 0; i < 4; i++) {
        FlowRibbon ribbon;
        ribbon.direction = glm::normalize(glm::vec3(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)));
        ribbon.thickness = ofRandom(1.0f, 3.0f);
        ribbon.life = 0.0f;
        ribbon.maxLife = ofRandom(6.0f, 12.0f);
        ribbons.push_back(ribbon);
    }
    
    // Initialize crystals
    for(int i = 0; i < 6; i++) {
        Crystal crystal;
        float angle = TWO_PI * i / 6.0f;
        crystal.pos = glm::vec3(80 * cos(angle), ofRandom(-30, 30), 80 * sin(angle));
        crystal.rotation = glm::vec3(0, 0, 0);
        crystal.scale = 0.0f;
        crystal.growth = 0.0f;
        crystal.maxGrowth = ofRandom(5.0f, 10.0f);
        crystal.type = i % 3;
        crystals.push_back(crystal);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if(animate) {
        time += ofGetLastFrameTime() * timeSpeed;
        
        if(smoothCameraMode) {
            updateSmoothCamera();
        }
        
        updateNodes();
        updateConnections();
        updateParticles3D();
        updateRibbons();
        updateCrystals();
        
        // Spawn new nodes occasionally
        if(nodes.size() < maxNodes && ofRandom(1.0f) < spawnRate * ofGetLastFrameTime()) {
            spawnNode();
        }
        
        // Spawn particles
        if(ofRandom(1.0f) < 1.0f * ofGetLastFrameTime()) {
            if(!nodes.empty()) {
                int idx = (int)ofRandom(nodes.size());
                spawnParticle3D(nodes[idx].pos);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Simple background
    if(blackOnWhite) {
        ofBackground(240);
    } else {
        ofBackground(15);
    }
    
    cam.begin();
    
    // Draw a simple reference grid to see if 3D is working
    ofSetColor(blackOnWhite ? 100 : 150);
    ofSetLineWidth(0.5f);
    for(int x = -200; x <= 200; x += 50) {
        ofDrawLine(x, 0, -200, x, 0, 200);
        ofDrawLine(-200, 0, x, 200, 0, x);
    }
    
    // Draw simple coordinate axes
    ofSetLineWidth(2.0f);
    ofSetColor(blackOnWhite ? ofColor(200, 0, 0) : ofColor(255, 100, 100)); // X axis - red
    ofDrawLine(0, 0, 0, 50, 0, 0);
    ofSetColor(blackOnWhite ? ofColor(0, 200, 0) : ofColor(100, 255, 100)); // Y axis - green
    ofDrawLine(0, 0, 0, 0, 50, 0);
    ofSetColor(blackOnWhite ? ofColor(0, 0, 200) : ofColor(100, 100, 255)); // Z axis - blue
    ofDrawLine(0, 0, 0, 0, 0, 50);
    
    // Draw some basic 3D shapes
    if(blackOnWhite) {
        ofSetColor(60, 60, 60);
    } else {
        ofSetColor(200, 200, 200);
    }
    
    // Rotating cube
    ofPushMatrix();
    ofTranslate(50, 30, 0);
    ofRotateYDeg(time * 30);
    ofRotateXDeg(time * 20);
    ofNoFill();
    ofSetLineWidth(1.5f);
    ofDrawBox(40);
    ofPopMatrix();
    
    // Pulsing sphere
    ofPushMatrix();
    ofTranslate(-50, 20, 30);
    float sphereSize = 20 + 10 * sin(time * 2);
    ofDrawSphere(sphereSize);
    ofPopMatrix();
    
    // Moving icosahedron
    ofPushMatrix();
    ofTranslate(0, -40, 60 * sin(time * 0.5f));
    ofRotateYDeg(time * 45);
    ofNoFill();
    drawIcosahedron(15);
    ofPopMatrix();
    
    // Draw enhanced structures if enabled
    if(showFractals) {
        drawSimpleFractals();
    }
    
    if(showFlowFields) {
        drawFlowingRibbons();
    }
    
    drawCrystalStructures();
    
    // Draw node system
    if(showConnections) {
        drawConnections();
    }
    
    if(showNodes) {
        drawNodes();
    }
    
    // Draw particle effects
    drawParticles3D();
    
    cam.end();
    
    // Draw UI info
    if(blackOnWhite) {
        ofSetColor(60, 60, 60);
    } else {
        ofSetColor(200, 200, 200);
    }
    
    ofDrawBitmapString("3D Generative Art - Enhanced Beauty", 20, 30);
    ofDrawBitmapString("Mouse: Rotate camera | Space: Animation | C: Colors | M: Auto camera", 20, 50);
    ofDrawBitmapString("F: Fractals | T: Trails | R: Reset", 20, 70);
    ofDrawBitmapString("Nodes: " + ofToString(nodes.size()) + " | Particles: " + ofToString(particles3d.size()), 20, 90);
}

//--------------------------------------------------------------
void ofApp::updateNodes(){
    for(auto& node : nodes) {
        if(!node.isAlive) continue;
        
        node.age += ofGetLastFrameTime();
        if(node.age >= node.maxAge) {
            node.isAlive = false;
            continue;
        }
        
        applyNoiseForce(node);
        node.pos += node.vel * ofGetLastFrameTime();
        node.vel *= 0.98f;
    }
    
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(), 
        [](const Node3D& n) { return !n.isAlive; }), nodes.end());
}

//--------------------------------------------------------------
void ofApp::updateConnections(){
    connections.clear();
    
    for(int i = 0; i < nodes.size(); i++) {
        if(!nodes[i].isAlive) continue;
        
        for(int j = i + 1; j < nodes.size(); j++) {
            if(!nodes[j].isAlive) continue;
            
            float dist = glm::distance(nodes[i].pos, nodes[j].pos);
            if(dist < connectionDistance) {
                connections.push_back(std::make_pair(i, j));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawNodes(){
    for(const auto& node : nodes) {
        if(!node.isAlive) continue;
        
        float alpha = getAlphaByAge(node.age, node.maxAge);
        
        if(blackOnWhite) {
            ofSetColor(30, 30, 30, alpha * 255);
        } else {
            ofSetColor(255, 255, 255, alpha * 255);
        }
        
        ofPushMatrix();
        ofTranslate(node.pos);
        ofDrawSphere(node.size);
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawConnections(){
    ofSetLineWidth(strokeWeight);
    
    for(const auto& conn : connections) {
        if(conn.first >= nodes.size() || conn.second >= nodes.size()) continue;
        if(!nodes[conn.first].isAlive || !nodes[conn.second].isAlive) continue;
        
        float alpha1 = getAlphaByAge(nodes[conn.first].age, nodes[conn.first].maxAge);
        float alpha2 = getAlphaByAge(nodes[conn.second].age, nodes[conn.second].maxAge);
        float alpha = (alpha1 + alpha2) * 0.5f * 0.7f;
        
        if(blackOnWhite) {
            ofSetColor(80, 80, 80, alpha * 255);
        } else {
            ofSetColor(180, 180, 180, alpha * 255);
        }
        
        ofDrawLine(nodes[conn.first].pos, nodes[conn.second].pos);
    }
}

//--------------------------------------------------------------
void ofApp::spawnNode(){
    Node3D node;
    
    float radius = ofRandom(30, 150);
    float theta = ofRandom(TWO_PI);
    float phi = ofRandom(PI);
    
    node.pos.x = radius * sin(phi) * cos(theta);
    node.pos.y = radius * sin(phi) * sin(theta);
    node.pos.z = radius * cos(phi);
    
    node.vel = glm::vec3(ofRandom(-nodeSpeed, nodeSpeed), 
                         ofRandom(-nodeSpeed, nodeSpeed), 
                         ofRandom(-nodeSpeed, nodeSpeed));
    
    node.size = ofRandom(nodeSize * 0.7f, nodeSize * 1.5f);
    node.age = 0.0f;
    node.maxAge = ofRandom(nodeLife * 0.7f, nodeLife * 1.3f);
    node.isAlive = true;
    node.connections = 0;
    
    nodes.push_back(node);
}

//--------------------------------------------------------------
void ofApp::applyNoiseForce(Node3D& node){
    float noiseX = ofNoise(node.pos.x * noiseScale, node.pos.y * noiseScale, node.pos.z * noiseScale, time * 0.1f);
    float noiseY = ofNoise(node.pos.x * noiseScale + 1000, node.pos.y * noiseScale, node.pos.z * noiseScale, time * 0.1f);
    float noiseZ = ofNoise(node.pos.x * noiseScale, node.pos.y * noiseScale + 1000, node.pos.z * noiseScale, time * 0.1f);
    
    glm::vec3 force;
    force.x = (noiseX - 0.5f) * noiseStrength;
    force.y = (noiseY - 0.5f) * noiseStrength;
    force.z = (noiseZ - 0.5f) * noiseStrength;
    
    node.vel += force * ofGetLastFrameTime();
}

//--------------------------------------------------------------
float ofApp::getAlphaByAge(float age, float maxAge){
    float normalizedAge = age / maxAge;
    if(normalizedAge < 0.2f) {
        return normalizedAge / 0.2f;
    } else if(normalizedAge > 0.8f) {
        return 1.0f - (normalizedAge - 0.8f) / 0.2f;
    }
    return 1.0f;
}

//--------------------------------------------------------------
void ofApp::drawSimpleFractals(){
    ofSetLineWidth(strokeWeight * 0.6f);
    
    for(int i = 0; i < 3; i++) {
        float angle = TWO_PI * i / 3.0f;
        glm::vec3 pos = glm::vec3(100 * cos(angle), 0, 100 * sin(angle));
        
        float alpha = 0.4f;
        if(blackOnWhite) {
            ofSetColor(100, 100, 100, alpha * 255);
        } else {
            ofSetColor(160, 160, 160, alpha * 255);
        }
        
        drawFractalBranch(pos, glm::vec3(0, 1, 0), 25.0f, 3, angle + time * 0.1f);
    }
}

//--------------------------------------------------------------
void ofApp::drawFractalBranch(glm::vec3 pos, glm::vec3 dir, float length, int depth, float rotation){
    if(depth <= 0 || length < 3.0f) return;
    
    glm::vec3 endPos = pos + dir * length;
    ofDrawLine(pos, endPos);
    
    if(depth > 1) {
        float angleStep = PI / 4.0f;
        for(int i = 0; i < 2; i++) {
            float branchAngle = rotation + angleStep * (i - 0.5f);
            glm::vec3 branchDir = glm::vec3(
                dir.x * cos(branchAngle) - dir.z * sin(branchAngle),
                dir.y * 0.8f,
                dir.x * sin(branchAngle) + dir.z * cos(branchAngle)
            );
            branchDir = glm::normalize(branchDir);
            
            drawFractalBranch(endPos, branchDir, length * 0.7f, depth - 1, rotation);
        }
    }
}

// Add all the other methods with simplified implementations...
void ofApp::setupTrails(){
    trailFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    trailFbo.begin();
    ofClear(0, 0, 0, 0);
    trailFbo.end();
}

void ofApp::updateTrails(){}

void ofApp::updateSmoothCamera(){
    if(!smoothCameraMode) return;
    
    float cameraAngle = time * cameraSpeed;
    float cameraX = cameraOrbitRadius * cos(cameraAngle);
    float cameraZ = cameraOrbitRadius * sin(cameraAngle);
    float cameraY = cameraHeight * sin(time * cameraSpeed * 0.3f);
    
    glm::vec3 cameraPos = glm::vec3(cameraX, cameraY, cameraZ);
    cam.setPosition(cameraPos);
    cam.lookAt(glm::vec3(0, 0, 0));
}

void ofApp::updateRibbons(){
    for(auto& ribbon : ribbons) {
        ribbon.life += ofGetLastFrameTime();
        
        if(ribbon.life >= ribbon.maxLife) {
            ribbon.points.clear();
            ribbon.ages.clear();
            ribbon.life = 0.0f;
            ribbon.direction = glm::normalize(glm::vec3(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)));
            continue;
        }
        
        if(ribbon.points.empty()) {
            ribbon.points.push_back(glm::vec3(ofRandom(-80, 80), ofRandom(-80, 80), ofRandom(-80, 80)));
            ribbon.ages.push_back(0.0f);
        } else {
            glm::vec3 lastPoint = ribbon.points.back();
            
            float noiseX = ofNoise(lastPoint.x * 0.02f, time * 0.3f) - 0.5f;
            float noiseY = ofNoise(lastPoint.y * 0.02f, time * 0.3f) - 0.5f;
            float noiseZ = ofNoise(lastPoint.z * 0.02f, time * 0.3f) - 0.5f;
            
            ribbon.direction += glm::vec3(noiseX, noiseY, noiseZ) * 0.1f;
            ribbon.direction = glm::normalize(ribbon.direction);
            
            glm::vec3 nextPoint = lastPoint + ribbon.direction * 2.0f;
            ribbon.points.push_back(nextPoint);
            ribbon.ages.push_back(0.0f);
            
            if(ribbon.points.size() > 30) {
                ribbon.points.erase(ribbon.points.begin());
                ribbon.ages.erase(ribbon.ages.begin());
            }
        }
        
        for(float& age : ribbon.ages) {
            age += ofGetLastFrameTime();
        }
    }
}

void ofApp::updateCrystals(){
    for(auto& crystal : crystals) {
        crystal.growth += ofGetLastFrameTime() * 0.3f;
        crystal.scale = ofMap(crystal.growth, 0, crystal.maxGrowth, 0.0f, 1.0f, true);
        
        crystal.rotation.x += ofGetLastFrameTime() * 0.3f;
        crystal.rotation.y += ofGetLastFrameTime() * 0.2f;
        crystal.rotation.z += ofGetLastFrameTime() * 0.1f;
        
        if(crystal.growth >= crystal.maxGrowth) {
            crystal.growth = 0.0f;
        }
    }
}

void ofApp::drawFlowingRibbons(){
    ofSetLineWidth(strokeWeight * 1.2f);
    
    for(const auto& ribbon : ribbons) {
        if(ribbon.points.size() < 2) continue;
        
        for(int i = 0; i < ribbon.points.size() - 1; i++) {
            float normalizedAge = i / float(ribbon.points.size());
            float alpha = (1.0f - normalizedAge) * 0.6f;
            
            if(blackOnWhite) {
                ofSetColor(50, 50, 50, alpha * 255);
            } else {
                ofSetColor(200, 200, 200, alpha * 255);
            }
            
            ofDrawLine(ribbon.points[i], ribbon.points[i + 1]);
        }
    }
}

void ofApp::drawCrystalStructures(){
    for(const auto& crystal : crystals) {
        if(crystal.scale <= 0) continue;
        
        float alpha = crystal.scale * 0.7f;
        if(blackOnWhite) {
            ofSetColor(40, 40, 40, alpha * 255);
        } else {
            ofSetColor(220, 220, 220, alpha * 255);
        }
        
        drawCrystal(crystal);
    }
}

void ofApp::drawCrystal(const Crystal& crystal){
    ofPushMatrix();
    ofTranslate(crystal.pos);
    ofRotateXDeg(crystal.rotation.x * RAD_TO_DEG);
    ofRotateYDeg(crystal.rotation.y * RAD_TO_DEG);
    ofRotateZDeg(crystal.rotation.z * RAD_TO_DEG);
    ofScale(crystal.scale * (6 + 3 * sin(time + crystal.type)));
    
    ofSetLineWidth(strokeWeight);
    ofNoFill();
    
    switch(crystal.type) {
        case 0: ofDrawBox(10); break;
        case 1: drawOctahedron(5); break;
        case 2: drawIcosahedron(4); break;
    }
    
    ofPopMatrix();
}

void ofApp::updateParticles3D(){
    for(auto& particle : particles3d) {
        particle.life += ofGetLastFrameTime();
        
        if(particle.life >= particle.maxLife) {
            particle.life = particle.maxLife;
            continue;
        }
        
        particle.vel.y -= 15.0f * ofGetLastFrameTime();
        
        float noiseX = ofNoise(particle.pos.x * 0.015f, particle.pos.y * 0.015f, time * 0.4f) - 0.5f;
        float noiseZ = ofNoise(particle.pos.x * 0.015f + 100, particle.pos.z * 0.015f, time * 0.4f) - 0.5f;
        
        particle.vel.x += noiseX * 8.0f * ofGetLastFrameTime();
        particle.vel.z += noiseZ * 8.0f * ofGetLastFrameTime();
        
        particle.pos += particle.vel * ofGetLastFrameTime();
    }
    
    particles3d.erase(std::remove_if(particles3d.begin(), particles3d.end(),
        [](const Particle3D& p) { return p.life >= p.maxLife; }), particles3d.end());
}

void ofApp::drawParticles3D(){
    for(const auto& particle : particles3d) {
        float normalizedLife = particle.life / particle.maxLife;
        float alpha = 1.0f - normalizedLife;
        alpha *= alpha;
        
        if(blackOnWhite) {
            ofSetColor(60, 60, 60, alpha * 255);
        } else {
            ofSetColor(200, 200, 200, alpha * 255);
        }
        
        ofPushMatrix();
        ofTranslate(particle.pos);
        ofDrawSphere(particle.size * (1.0f - normalizedLife * 0.3f));
        ofPopMatrix();
    }
}

void ofApp::spawnParticle3D(glm::vec3 position){
    Particle3D particle;
    particle.pos = position;
    particle.vel = glm::vec3(ofRandom(-20, 20), ofRandom(5, 30), ofRandom(-20, 20));
    particle.life = 0.0f;
    particle.maxLife = ofRandom(1.5f, 3.0f);
    particle.size = ofRandom(0.8f, 2.0f);
    
    particles3d.push_back(particle);
}

// Simple geometric shapes
void ofApp::drawOctahedron(float size){
    glm::vec3 vertices[6] = {
        glm::vec3(size, 0, 0), glm::vec3(-size, 0, 0),
        glm::vec3(0, size, 0), glm::vec3(0, -size, 0),
        glm::vec3(0, 0, size), glm::vec3(0, 0, -size)
    };
    
    int edges[][2] = {{0,2}, {0,3}, {0,4}, {0,5}, {1,2}, {1,3}, {1,4}, {1,5}, {2,4}, {2,5}, {3,4}, {3,5}};
    for(int i = 0; i < 12; i++) {
        ofDrawLine(vertices[edges[i][0]], vertices[edges[i][1]]);
    }
}

void ofApp::drawIcosahedron(float size){
    float phi = (1.0f + sqrt(5.0f)) * 0.5f;
    float a = size / sqrt(phi * phi + 1);
    
    glm::vec3 vertices[12] = {
        glm::vec3(-a, phi*a, 0), glm::vec3(a, phi*a, 0), glm::vec3(-a, -phi*a, 0), glm::vec3(a, -phi*a, 0),
        glm::vec3(0, -a, phi*a), glm::vec3(0, a, phi*a), glm::vec3(0, -a, -phi*a), glm::vec3(0, a, -phi*a),
        glm::vec3(phi*a, 0, -a), glm::vec3(phi*a, 0, a), glm::vec3(-phi*a, 0, -a), glm::vec3(-phi*a, 0, a)
    };
    
    int edges[][2] = {{0,1}, {1,5}, {5,11}, {11,0}, {0,7}, {1,8}, {2,3}, {3,4}, {4,9}, {9,2}};
    for(int i = 0; i < 10; i++) {
        ofDrawLine(vertices[edges[i][0]], vertices[edges[i][1]]);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case ' ':
            animate = !animate;
            break;
        case 'c':
        case 'C':
            blackOnWhite = !blackOnWhite;
            break;
        case 'n':
        case 'N':
            showNodes = !showNodes;
            break;
        case 'l':
        case 'L':
            showConnections = !showConnections;
            break;
        case 't':
        case 'T':
            showTrails = !showTrails;
            break;
        case 'f':
        case 'F':
            showFractals = !showFractals;
            break;
        case 'm':
        case 'M':
            smoothCameraMode = !smoothCameraMode;
            if(!smoothCameraMode) {
                cam.setDistance(300);
            }
            break;
        case 'r':
        case 'R':
            nodes.clear();
            particles3d.clear();
            ribbons.clear();
            crystals.clear();
            time = 0;
            // Reinitialize
            for(int i = 0; i < 10; i++) {
                spawnNode();
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
void ofApp::exit(){}