#include "ofApp.h"

//--------------------------------------------------------------
// Particle Implementation
//--------------------------------------------------------------
Particle::Particle() {
    // Spawn in a sphere for more interesting distribution
    float theta = ofRandom(0, TWO_PI);
    float phi = ofRandom(0, PI);
    float radius = ofRandom(50, 150);
    position = ofVec3f(
        radius * sin(phi) * cos(theta),
        radius * sin(phi) * sin(theta),
        radius * cos(phi)
    );
    
    velocity = ofVec3f(ofRandom(-0.1, 0.1), ofRandom(-0.1, 0.1), ofRandom(-0.1, 0.1));
    acceleration = ofVec3f(0, 0, 0);
    maxLife = ofRandom(400, 800);
    lifespan = maxLife;
    size = ofRandom(0.5, 3);
    noiseOffset = ofRandom(1000);
    phase = ofRandom(0, TWO_PI);
    glowIntensity = ofRandom(0.5, 1.0);
    
    // More varied color range
    float colorChoice = ofRandom(1);
    if (colorChoice < 0.3) {
        float hue = ofRandom(180, 220); // Blues
        color = ofColor::fromHsb(hue, ofRandom(100, 200), 255);
    } else if (colorChoice < 0.6) {
        float hue = ofRandom(250, 290); // Purples
        color = ofColor::fromHsb(hue, ofRandom(120, 180), 255);
    } else if (colorChoice < 0.8) {
        float hue = ofRandom(300, 340); // Pinks
        color = ofColor::fromHsb(hue, ofRandom(80, 150), 255);
    } else {
        float hue = ofRandom(160, 180); // Cyan
        color = ofColor::fromHsb(hue, ofRandom(140, 200), 255);
    }
}

void Particle::update(float time) {
    // Multi-layered noise for organic movement
    float noiseScale1 = 0.002;
    float noiseScale2 = 0.005;
    float timeScale = 0.015;
    
    // Primary movement
    float noiseX = ofNoise(position.x * noiseScale1 + time * timeScale + noiseOffset, position.y * noiseScale1);
    float noiseY = ofNoise(position.y * noiseScale1 + time * timeScale + noiseOffset, position.z * noiseScale1);
    float noiseZ = ofNoise(position.z * noiseScale1 + time * timeScale + noiseOffset, position.x * noiseScale1);
    
    // Secondary turbulence
    float turb = ofNoise(position.x * noiseScale2, position.y * noiseScale2, time * 0.1 + noiseOffset);
    
    acceleration.x += (noiseX - 0.5) * 0.06 * (1.0 + turb * 0.5);
    acceleration.y += (noiseY - 0.5) * 0.06 * (1.0 + turb * 0.5);
    acceleration.z += (noiseZ - 0.5) * 0.06 * (1.0 + turb * 0.5);
    
    // Add slight upward drift for ethereal effect
    acceleration.y += 0.001;
    
    velocity += acceleration;
    velocity.limit(0.6);
    position += velocity;
    acceleration *= 0.93;
    
    lifespan -= 0.25;
    
    // Update visual properties
    float lifePct = lifespan / maxLife;
    color.a = 255 * pow(lifePct, 0.7); // Non-linear fade
    
    // Pulsing glow
    glowIntensity = 0.5 + 0.5 * sin(time * 2.0 + phase);
    
    // Size oscillation
    size = (size * 0.98) + (sin(time * 3.0 + phase) * 0.1 + 0.5) * 0.02;
}

void Particle::applyForce(ofVec3f force) {
    acceleration += force;
}

bool Particle::isDead() {
    return lifespan <= 0;
}

void Particle::draw() {
    ofPushStyle();
    ofSetColor(color);
    ofDrawSphere(position, size);
    ofPopStyle();
}

//--------------------------------------------------------------
// ofApp Implementation
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetSmoothLighting(true);
    ofEnableAntiAliasing();
    
    // Initialize parameters
    maxParticles = 400;
    time = 0;
    rotationSpeed = 0.03;
    attractorStrength = 0.08;
    attractorPos = ofVec3f(0, 0, 0);
    useFbo = true;
    maxTrailLength = 50;
    
    // Setup expanded color palette
    color1 = ofColor(20, 50, 120);    // Deep blue
    color2 = ofColor(150, 20, 180);   // Purple
    color3 = ofColor(50, 200, 250);   // Cyan
    color4 = ofColor(255, 100, 150);  // Pink
    color5 = ofColor(100, 50, 200);   // Violet
    
    colorPalette.push_back(color1);
    colorPalette.push_back(color2);
    colorPalette.push_back(color3);
    colorPalette.push_back(color4);
    colorPalette.push_back(color5);
    
    // Setup lighting with colored lights
    light1.setPosition(400, 400, 400);
    light1.setDiffuseColor(ofColor(180, 180, 255));
    light1.setSpecularColor(ofColor(255, 255, 255));
    light1.setAmbientColor(ofColor(50, 50, 80));
    
    light2.setPosition(-400, -400, 400);
    light2.setDiffuseColor(ofColor(255, 180, 200));
    light2.setSpecularColor(ofColor(255, 200, 220));
    
    light3.setPosition(0, -600, 0);
    light3.setDiffuseColor(ofColor(200, 150, 255));
    light3.setSpotlight();
    light3.setSpotlightCutOff(45);
    light3.setSpotConcentration(20);
    
    // Setup camera
    cam.setDistance(600);
    cam.setNearClip(0.1);
    cam.setFarClip(5000);
    
    // Setup FBOs for post-processing
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    glowFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
    
    glowFbo.begin();
    ofClear(0, 0, 0, 0);
    glowFbo.end();
    
    // Initialize meshes
    particleMesh.setMode(OF_PRIMITIVE_POINTS);
    ribbonMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    glPointSize(2);
}

//--------------------------------------------------------------
void ofApp::update() {
    time += 0.004;
    
    // Complex attractor movement pattern
    float t1 = time * 0.3;
    float t2 = time * 0.2;
    float t3 = time * 0.15;
    
    attractorPos.x = sin(t1) * 60 + sin(t1 * 3) * 20;
    attractorPos.y = cos(t2) * 60 + sin(t2 * 2) * 15;
    attractorPos.z = sin(t3) * 60 + cos(t3 * 4) * 10;
    
    // Update attractor trail
    attractorTrail.push_back(attractorPos);
    if (attractorTrail.size() > maxTrailLength) {
        attractorTrail.erase(attractorTrail.begin());
    }
    
    // Update color palette dynamically
    updateColorPalette(time);
    
    // Add new particles with variation
    while (particles.size() < maxParticles) {
        addParticle();
    }
    
    // Update particles
    particleMesh.clear();
    ribbonMesh.clear();
    
    for (int i = particles.size() - 1; i >= 0; i--) {
        particles[i].update(time);
        
        // Apply attraction with distance falloff
        ofVec3f attraction = calculateAttraction(particles[i].position);
        particles[i].applyForce(attraction);
        
        // Apply slight spiral force
        ofVec3f spiralForce;
        spiralForce.x = -particles[i].position.z * 0.001;
        spiralForce.z = particles[i].position.x * 0.001;
        particles[i].applyForce(spiralForce);
        
        // Remove dead particles
        if (particles[i].isDead()) {
            particles.erase(particles.begin() + i);
        } else {
            // Add to mesh with color modulation
            particleMesh.addVertex(particles[i].position);
            ofColor modulatedColor = particles[i].color;
            modulatedColor.setBrightness(modulatedColor.getBrightness() * particles[i].glowIntensity);
            particleMesh.addColor(modulatedColor);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (useFbo) {
        fbo.begin();
        
        // Fade effect with color tint
        ofEnableAlphaBlending();
        ofSetColor(0, 0, 5, 3); // Slight blue tint
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    // Gradient background
    ofBackgroundGradient(ofColor(0, 0, 10), ofColor(10, 0, 20), OF_GRADIENT_CIRCULAR);
    
    cam.begin();
    
    // Enable depth testing for 3D
    ofEnableDepthTest();
    
    // Subtle scene rotation
    ofRotateY(time * rotationSpeed * 10);
    ofRotateX(time * rotationSpeed * 5);
    ofRotateZ(time * rotationSpeed * 2);
    
    // Draw attractor trail
    ofSetColor(255, 100, 150, 40);
    ofNoFill();
    ofSetLineWidth(2);
    ofBeginShape();
    for (int i = 0; i < attractorTrail.size(); i++) {
        float alpha = ofMap(i, 0, attractorTrail.size(), 0, 40);
        ofSetColor(255, 100, 150, alpha);
        ofVertex(attractorTrail[i]);
    }
    ofEndShape(false);
    ofFill();
    
    // Draw glowing attractor with multiple layers
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < 5; i++) {
        float size = 15 + i * 8;
        float alpha = ofMap(i, 0, 5, 100, 10);
        ofSetColor(255, 150, 200, alpha);
        ofDrawSphere(attractorPos, size);
    }
    
    // Draw particle connections with gradient
    ofSetLineWidth(0.5);
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            float dist = particles[i].position.distance(particles[j].position);
            if (dist < 35) {
                float alpha = ofMap(dist, 0, 35, 25, 0);
                
                // Color based on particle colors
                ofColor connectionColor = particles[i].color.getLerped(particles[j].color, 0.5);
                connectionColor.a = alpha;
                ofSetColor(connectionColor);
                ofDrawLine(particles[i].position, particles[j].position);
            }
        }
    }
    
    // Draw particles with glow
    ofDisableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // First pass: larger glow
    glPointSize(8);
    for (auto& p : particles) {
        ofSetColor(p.color, p.color.a * 0.3);
        ofDrawSphere(p.position, p.size * 3);
    }
    
    // Second pass: medium glow
    glPointSize(4);
    particleMesh.draw();
    
    // Third pass: bright core
    glPointSize(1);
    for (auto& p : particles) {
        ofSetColor(255, 255, 255, p.color.a * 0.8);
        ofDrawSphere(p.position, p.size * 0.5);
    }
    
    // Draw flowing ribbons between some particles
    drawRibbons();
    
    // Draw extra glowing spheres
    drawGlowingSpheres();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableDepthTest();
    
    cam.end();
    
    if (useFbo) {
        fbo.end();
        
        // Apply glow effect
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255, 255, 255, 200);
        fbo.draw(0, 0);
        
        // Draw again with slight blur for glow
        ofSetColor(255, 255, 255, 100);
        fbo.draw(1, 1);
        fbo.draw(-1, -1);
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }
    
    // Draw UI info
    ofSetColor(200, 200, 255);
    stringstream ss;
//    ss << "Ethereal Particle Flow" << endl;
//    ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl;
//    ss << "Particles: " << particles.size() << endl;
//    ss << "[SPACE] Toggle trails | [R] Reset | [C] Cycle colors" << endl;
    ofDrawBitmapString(ss.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::addParticle() {
    particles.push_back(Particle());
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateAttraction(ofVec3f pos) {
    ofVec3f force = attractorPos - pos;
    float distance = force.length();
    distance = ofClamp(distance, 10, 200);
    
    // Non-linear attraction with smooth falloff
    float strength = (attractorStrength * 8) / pow(distance, 1.5);
    force.normalize();
    force *= strength;
    
    return force;
}

//--------------------------------------------------------------
ofColor ofApp::interpolateColor(float t) {
    t = ofClamp(t, 0, 1);
    if (t < 0.5) {
        return color1.getLerped(color2, t * 2);
    } else {
        return color2.getLerped(color3, (t - 0.5) * 2);
    }
}

//--------------------------------------------------------------
void ofApp::drawRibbons() {
    ofSetLineWidth(1);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Draw flowing ribbons between selected particles
    for (int i = 0; i < particles.size() - 1; i += 5) {
        if (i + 1 < particles.size()) {
            ofMesh ribbon;
            ribbon.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            
            ofVec3f p1 = particles[i].position;
            ofVec3f p2 = particles[i + 1].position;
            
            // Create ribbon vertices
            ofVec3f direction = (p2 - p1).getNormalized();
            ofVec3f perpendicular = direction.getCrossed(ofVec3f(0, 1, 0)).getNormalized();
            
            float width = 2.0;
            
            // Add vertices with color gradient
            ribbon.addVertex(p1 + perpendicular * width);
            ribbon.addVertex(p1 - perpendicular * width);
            ribbon.addColor(ofColor(particles[i].color, 30));
            ribbon.addColor(ofColor(particles[i].color, 30));
            
            ribbon.addVertex(p2 + perpendicular * width);
            ribbon.addVertex(p2 - perpendicular * width);
            ribbon.addColor(ofColor(particles[i + 1].color, 30));
            ribbon.addColor(ofColor(particles[i + 1].color, 30));
            
            ribbon.draw();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawGlowingSpheres() {
    // Draw larger glowing spheres at strategic positions
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (int i = 0; i < particles.size(); i += 20) {
        float pulseSize = sin(time * 3.0 + particles[i].phase) * 0.3 + 1.0;
        
        // Multi-layer glow
        for (int layer = 0; layer < 3; layer++) {
            float layerSize = particles[i].size * (4 + layer * 2) * pulseSize;
            float layerAlpha = ofMap(layer, 0, 3, 20, 5);
            
            ofColor glowColor = particles[i].color;
            glowColor.a = layerAlpha;
            ofSetColor(glowColor);
            ofDrawSphere(particles[i].position, layerSize);
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateColorPalette(float time) {
    // Slowly shift colors over time
    float hueShift = sin(time * 0.1) * 10;
    
    for (int i = 0; i < colorPalette.size(); i++) {
        float h = colorPalette[i].getHue() + hueShift;
        if (h > 255) h -= 255;
        if (h < 0) h += 255;
        colorPalette[i].setHue(h);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == ' ') {
        useFbo = !useFbo;
        if (useFbo) {
            fbo.begin();
            ofClear(0, 0, 0, 0);
            fbo.end();
        }
    } else if (key == 'r' || key == 'R') {
        particles.clear();
        attractorTrail.clear();
        fbo.begin();
        ofClear(0, 0, 0, 0);
        fbo.end();
    } else if (key == 'c' || key == 'C') {
        // Cycle through different color schemes
        static int colorScheme = 0;
        colorScheme = (colorScheme + 1) % 3;
        
        if (colorScheme == 0) {
            // Cool blues and purples
            color1 = ofColor(20, 50, 120);
            color2 = ofColor(150, 20, 180);
            color3 = ofColor(50, 200, 250);
            color4 = ofColor(255, 100, 150);
            color5 = ofColor(100, 50, 200);
        } else if (colorScheme == 1) {
            // Warm sunset colors
            color1 = ofColor(255, 100, 50);
            color2 = ofColor(255, 50, 100);
            color3 = ofColor(255, 150, 50);
            color4 = ofColor(200, 100, 255);
            color5 = ofColor(255, 200, 100);
        } else if (colorScheme == 2) {
            // Northern lights
            color1 = ofColor(50, 255, 150);
            color2 = ofColor(100, 255, 200);
            color3 = ofColor(150, 100, 255);
            color4 = ofColor(50, 200, 255);
            color5 = ofColor(200, 255, 150);
        }
        
        colorPalette.clear();
        colorPalette.push_back(color1);
        colorPalette.push_back(color2);
        colorPalette.push_back(color3);
        colorPalette.push_back(color4);
        colorPalette.push_back(color5);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    if (useFbo) {
        fbo.allocate(w, h, GL_RGBA32F_ARB);
        glowFbo.allocate(w, h, GL_RGBA32F_ARB);
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
