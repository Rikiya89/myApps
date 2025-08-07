#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    
    time = 0;
    maxParticles = 1200;
    noiseScale = 0.006;
    showTrails = true;
    waveAmplitude = 120;
    spiralRadius = 180;
    numAttractors = 7;
    colorShift = 0;
    geometryRadius = 200;
    currentPattern = 0;
    
    hexColorPalette = {
        hexToColor("#46A9D0"),
        hexToColor("#0083BB"),
        hexToColor("#13529F"),
        hexToColor("#021F72"),
        hexToColor("#00214B"),
        hexToColor("#002427"),
        hexToColor("#03604A"),
        hexToColor("#017777"),
        hexToColor("#009BAA"),
        hexToColor("#73CBD4")
    };
    
    attractors.resize(numAttractors);
    for(int i = 0; i < numAttractors; i++) {
        float angle = (TWO_PI / numAttractors) * i;
        attractors[i].set(
            ofGetWidth() / 2 + cos(angle) * spiralRadius,
            ofGetHeight() / 2 + sin(angle) * spiralRadius
        );
    }
    
    geometryPoints.resize(8);
    
    particles.clear();
    particles.reserve(maxParticles);
}

//--------------------------------------------------------------
ofColor ofApp::hexToColor(string hex) {
    if(hex[0] == '#') hex = hex.substr(1);
    
    unsigned int r, g, b;
    sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
    
    return ofColor(r, g, b);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    colorShift += ofGetLastFrameTime() * 20;
    
    // Dynamic pattern switching every 15 seconds
    currentPattern = (int)(time / 15) % 4;
    
    // Update morphing geometry
    for(int i = 0; i < geometryPoints.size(); i++) {
        float angle = (TWO_PI / geometryPoints.size()) * i;
        float morphTime = time * 0.4 + angle;
        float morphRadius = geometryRadius + sin(time * 0.3 + i) * 80;
        
        if(currentPattern == 0) { // Star pattern
            float starRadius = (i % 2 == 0) ? morphRadius : morphRadius * 0.5;
            geometryPoints[i].set(
                ofGetWidth() / 2 + cos(morphTime) * starRadius,
                ofGetHeight() / 2 + sin(morphTime) * starRadius
            );
        } else if(currentPattern == 1) { // Flower pattern
            float petalRadius = morphRadius * (1 + 0.5 * sin(morphTime * 3));
            geometryPoints[i].set(
                ofGetWidth() / 2 + cos(morphTime) * petalRadius,
                ofGetHeight() / 2 + sin(morphTime) * petalRadius
            );
        } else if(currentPattern == 2) { // Spiral pattern
            float spiralAngle = morphTime + i * 0.5;
            float spiralR = morphRadius * (0.5 + 0.5 * (i / (float)geometryPoints.size()));
            geometryPoints[i].set(
                ofGetWidth() / 2 + cos(spiralAngle) * spiralR,
                ofGetHeight() / 2 + sin(spiralAngle) * spiralR
            );
        } else { // Wave pattern
            geometryPoints[i].set(
                ofGetWidth() / 2 + cos(morphTime) * morphRadius,
                ofGetHeight() / 2 + sin(morphTime * 2) * morphRadius * 0.7
            );
        }
    }
    
    // Update attractors with complex motion
    for(int i = 0; i < numAttractors; i++) {
        float baseAngle = (TWO_PI / numAttractors) * i;
        float spiralTime = time * 0.25 + baseAngle;
        float breathe = sin(time * 0.8 + i) * 60;
        
        // Multi-layer movement
        float layer1X = cos(spiralTime) * (spiralRadius + breathe);
        float layer1Y = sin(spiralTime) * (spiralRadius + breathe);
        
        float layer2X = cos(spiralTime * 1.618) * waveAmplitude * 0.4;
        float layer2Y = sin(spiralTime * 1.618 + PI/3) * waveAmplitude * 0.3;
        
        float layer3X = sin(time * 1.5 + i * 2) * 40;
        float layer3Y = cos(time * 1.2 + i * 1.7) * 35;
        
        attractors[i].set(
            ofGetWidth() / 2 + layer1X + layer2X + layer3X,
            ofGetHeight() / 2 + layer1Y + layer2Y + layer3Y
        );
    }
    
    for(int i = particles.size() - 1; i >= 0; i--) {
        Particle& p = particles[i];
        
        // Update trail
        p.trail.push_back(p.pos);
        if(p.trail.size() > 15) {
            p.trail.erase(p.trail.begin());
        }
        
        // Enhanced multi-layer flow field with turbulence
        float angle1 = ofNoise(p.pos.x * noiseScale, p.pos.y * noiseScale, time * 0.3) * TWO_PI * 8;
        float angle2 = ofNoise(p.pos.x * noiseScale * 3, p.pos.y * noiseScale * 3, time * 0.7) * TWO_PI * 4;
        float angle3 = ofNoise(p.pos.x * noiseScale * 0.5, p.pos.y * noiseScale * 0.5, time * 0.1) * TWO_PI * 2;
        
        float force1 = ofNoise(p.pos.x * noiseScale * 0.8, p.pos.y * noiseScale * 0.8, time * 0.4) * 4;
        float force2 = ofNoise(p.pos.x * noiseScale * 2.5, p.pos.y * noiseScale * 2.5, time * 0.9) * 2;
        float force3 = ofNoise(p.pos.x * noiseScale * 0.2, p.pos.y * noiseScale * 0.2, time * 0.15) * 6;
        
        ofVec2f turbulence1(cos(angle1) * force1, sin(angle1) * force1);
        ofVec2f turbulence2(cos(angle2) * force2, sin(angle2) * force2);
        ofVec2f turbulence3(cos(angle3) * force3, sin(angle3) * force3);
        
        ofVec2f combinedTurbulence = (turbulence1 * 0.4 + turbulence2 * 0.4 + turbulence3 * 0.2);
        
        // Enhanced attractor forces with geometry influence
        ofVec2f attractorForce(0, 0);
        ofVec2f geometryForce(0, 0);
        
        for(int j = 0; j < numAttractors; j++) {
            ofVec2f diff = attractors[j] - p.pos;
            float distance = diff.length();
            if(distance > 0) {
                float strength = ofMap(sin(time * 0.6 + j), -1, 1, 0.03, 0.18);
                float falloff = 1.0 / (distance * 0.008 + 1);
                attractorForce += diff.getNormalized() * strength * falloff;
            }
        }
        
        // Geometry point attraction
        for(int j = 0; j < geometryPoints.size(); j++) {
            ofVec2f diff = geometryPoints[j] - p.pos;
            float distance = diff.length();
            if(distance > 10 && distance < 300) {
                float strength = ofMap(distance, 10, 300, 0.12, 0.02);
                geometryForce += diff.getNormalized() * strength;
            }
        }
        
        // Enhanced vortex with multiple centers
        ofVec2f vortexForce(0, 0);
        for(int j = 0; j < numAttractors; j++) {
            ofVec2f diff = p.pos - attractors[j];
            float distance = diff.length();
            if(distance > 8 && distance < 250) {
                float vortexStrength = ofMap(distance, 8, 250, 1.2, 0.05);
                ofVec2f perpendicular(-diff.y, diff.x);
                vortexForce += perpendicular.getNormalized() * vortexStrength * sin(time * 2 + j);
            }
        }
        
        // Particle-to-particle interaction
        ofVec2f socialForce(0, 0);
        if(p.type == 0) { // Only for special particles
            for(int j = 0; j < min(50, (int)particles.size()); j++) {
                if(j != i) {
                    ofVec2f diff = particles[j].pos - p.pos;
                    float distance = diff.length();
                    if(distance > 0 && distance < 80) {
                        float strength = ofMap(distance, 0, 80, -0.5, 0.3);
                        socialForce += diff.getNormalized() * strength;
                    }
                }
            }
        }
        
        // Apply all forces
        p.vel += combinedTurbulence * 0.5 + attractorForce * 0.8 + vortexForce * 0.4 + 
                geometryForce * 0.6 + socialForce * 0.2;
        p.vel *= 0.94;
        p.pos += p.vel;
        
        // Update rotation
        p.rotation += p.rotationSpeed;
        p.energy = ofClamp(p.energy + ofRandom(-0.1, 0.1), 0, 1);
        
        // Enhanced particle lifecycle
        p.life -= ofGetLastFrameTime();
        float lifeRatio = p.life / p.maxLife;
        
        // Complex pulsing based on multiple factors
        float pulse1 = sin(time * 5 + p.pos.x * 0.01) * 0.2 + 0.8;
        float pulse2 = cos(time * 3.7 + p.pos.y * 0.008) * 0.15 + 0.85;
        float energyPulse = sin(p.energy * PI) * 0.3 + 0.7;
        
        float combinedPulse = pulse1 * pulse2 * energyPulse;
        
        p.color.a = lifeRatio * 255 * combinedPulse;
        p.size = (lifeRatio * 15 + 3) * combinedPulse;
        
        if(p.life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }
    
    // Enhanced particle spawning with variety
    while(particles.size() < maxParticles) {
        Particle p;
        
        // Random spawn location choice
        float spawnChoice = ofRandom(1.0);
        if(spawnChoice < 0.6) {
            // Spawn from attractors
            int attractorIndex = ofRandom(numAttractors);
            ofVec2f spawnPos = attractors[attractorIndex];
            float spawnRadius = ofRandom(15, 100);
            float spawnAngle = ofRandom(TWO_PI);
            p.pos.set(
                spawnPos.x + cos(spawnAngle) * spawnRadius,
                spawnPos.y + sin(spawnAngle) * spawnRadius
            );
        } else {
            // Spawn from geometry points
            int geoIndex = ofRandom(geometryPoints.size());
            ofVec2f spawnPos = geometryPoints[geoIndex];
            float spawnRadius = ofRandom(5, 60);
            float spawnAngle = ofRandom(TWO_PI);
            p.pos.set(
                spawnPos.x + cos(spawnAngle) * spawnRadius,
                spawnPos.y + sin(spawnAngle) * spawnRadius
            );
        }
        
        p.vel.set(ofRandom(-2, 2), ofRandom(-2, 2));
        p.maxLife = ofRandom(5, 18);
        p.life = p.maxLife;
        p.size = ofRandom(1, 12);
        p.energy = ofRandom(0.3, 1.0);
        p.type = (ofRandom(1.0) < 0.15) ? 1 : 0; // 15% special particles
        p.rotation = ofRandom(TWO_PI);
        p.rotationSpeed = ofRandom(-0.1, 0.1);
        
        // Use hex color palette with dynamic selection
        float colorSelector = ofNoise(p.pos.x * 0.003, p.pos.y * 0.003, time * 0.05);
        int colorIndex1 = (int)(colorSelector * hexColorPalette.size()) % hexColorPalette.size();
        int colorIndex2 = (colorIndex1 + 1) % hexColorPalette.size();
        
        // Blend between adjacent colors in palette
        float blendAmount = fmod(colorSelector * hexColorPalette.size(), 1.0);
        ofColor color1 = hexColorPalette[colorIndex1];
        ofColor color2 = hexColorPalette[colorIndex2];
        
        p.color.r = color1.r * (1.0 - blendAmount) + color2.r * blendAmount;
        p.color.g = color1.g * (1.0 - blendAmount) + color2.g * blendAmount;
        p.color.b = color1.b * (1.0 - blendAmount) + color2.b * blendAmount;
        p.color.a = 200;
        
        particles.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(showTrails) {
        ofFill();
        ofSetColor(0, 0, 0, 6);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    } else {
        ofClear(0);
    }
    
    // Enhanced background with depth
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    for(int i = 0; i < 5; i++) {
        float layerAlpha = 15 - i * 2;
        float layerTime = time * (0.1 + i * 0.05);
        ofSetColor(5 + i * 3, 8 + i * 2, 15 + i * 4, layerAlpha);
        
        float offsetX = sin(layerTime) * (20 + i * 10);
        float offsetY = cos(layerTime * 1.3) * (15 + i * 8);
        ofDrawRectangle(offsetX, offsetY, ofGetWidth(), ofGetHeight());
    }
    
    // Draw morphing geometry pattern using hex palette
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    int geometryColorIndex = (int)(time * 0.3) % hexColorPalette.size();
    ofColor geoColor = hexColorPalette[geometryColorIndex];
    ofSetColor(geoColor.r, geoColor.g, geoColor.b, 60);
    for(int i = 0; i < geometryPoints.size(); i++) {
        ofVec2f current = geometryPoints[i];
        ofVec2f next = geometryPoints[(i + 1) % geometryPoints.size()];
        
        ofSetLineWidth(2);
        ofDrawLine(current, next);
        
        float geometryPulse = sin(time * 4 + i) * 0.4 + 0.6;
        int pulseColorIndex = (geometryColorIndex + i) % hexColorPalette.size();
        ofColor pulseColor = hexColorPalette[pulseColorIndex];
        ofSetColor(pulseColor.r, pulseColor.g, pulseColor.b, 40 * geometryPulse);
        ofDrawCircle(current, 15 * geometryPulse);
    }
    
    // Enhanced attractor visualization with connections
    for(int i = 0; i < numAttractors; i++) {
        float pulse = sin(time * 1.8 + i * 0.8) * 0.4 + 0.6;
        
        // Attractor cores with multiple layers using hex palette
        int attractorColorIndex = (i * 2) % hexColorPalette.size();
        ofColor attractorColor = hexColorPalette[attractorColorIndex];
        
        ofSetColor(attractorColor.r, attractorColor.g, attractorColor.b, 60 * pulse);
        ofDrawCircle(attractors[i], 25 * pulse);
        
        int attractorColor2Index = (attractorColorIndex + 1) % hexColorPalette.size();
        ofColor attractorColor2 = hexColorPalette[attractorColor2Index];
        ofSetColor(attractorColor2.r, attractorColor2.g, attractorColor2.b, 35 * pulse);
        ofDrawCircle(attractors[i], 50 * pulse);
        
        int attractorColor3Index = (attractorColorIndex + 2) % hexColorPalette.size();
        ofColor attractorColor3 = hexColorPalette[attractorColor3Index];
        ofSetColor(attractorColor3.r, attractorColor3.g, attractorColor3.b, 15 * pulse);
        ofDrawCircle(attractors[i], 90 * pulse);
        
        // Draw connections between attractors
        for(int j = i + 1; j < numAttractors; j++) {
            float distance = attractors[i].distance(attractors[j]);
            if(distance < 400) {
                float connectionAlpha = ofMap(distance, 0, 400, 40, 5);
                int connectionColorIndex = (i + j) % hexColorPalette.size();
                ofColor connectionColor = hexColorPalette[connectionColorIndex];
                ofSetColor(connectionColor.r, connectionColor.g, connectionColor.b, connectionAlpha * pulse);
                ofSetLineWidth(1);
                ofDrawLine(attractors[i], attractors[j]);
            }
        }
    }
    
    // Draw particle trails with fading
    ofSetLineWidth(1);
    for(const auto& p : particles) {
        if(p.trail.size() > 1) {
            for(int i = 1; i < p.trail.size(); i++) {
                float trailAlpha = (i / (float)p.trail.size()) * (p.color.a / 255.0) * 0.6;
                ofSetColor(p.color.r, p.color.g, p.color.b, trailAlpha * 255);
                ofDrawLine(p.trail[i-1], p.trail[i]);
            }
        }
    }
    
    // Draw particle connections (flocking behavior visualization)
    for(int i = 0; i < particles.size(); i++) {
        const Particle& p1 = particles[i];
        if(p1.type == 1) { // Special particles create connections
            for(int j = i + 1; j < min(i + 20, (int)particles.size()); j++) {
                const Particle& p2 = particles[j];
                float distance = p1.pos.distance(p2.pos);
                if(distance < 120) {
                    float connectionAlpha = ofMap(distance, 0, 120, 25, 2);
                    ofSetColor(p1.color.r * 0.5 + p2.color.r * 0.5, 
                              p1.color.g * 0.5 + p2.color.g * 0.5, 
                              p1.color.b * 0.5 + p2.color.b * 0.5, 
                              connectionAlpha);
                    ofDrawLine(p1.pos, p2.pos);
                }
            }
        }
    }
    
    // Ultra-enhanced particle rendering
    for(const auto& p : particles) {
        float intensity = p.color.a / 255.0;
        
        // Rotating particle shapes for special particles
        if(p.type == 1) {
            ofPushMatrix();
            ofTranslate(p.pos.x, p.pos.y);
            ofRotateDeg(ofRadToDeg(p.rotation));
            
            // Draw rotating star/cross
            ofSetColor(p.color.r, p.color.g, p.color.b, p.color.a * 0.8);
            float starSize = p.size * 0.8;
            for(int i = 0; i < 6; i++) {
                float angle = (TWO_PI / 6) * i;
                float x1 = cos(angle) * starSize * 0.3;
                float y1 = sin(angle) * starSize * 0.3;
                float x2 = cos(angle) * starSize;
                float y2 = sin(angle) * starSize;
                ofDrawLine(x1, y1, x2, y2);
            }
            ofPopMatrix();
        }
        
        // Multi-layered particle cores
        // Outer cosmic halo
        ofSetColor(p.color.r, p.color.g, p.color.b, p.color.a * 0.08);
        ofDrawCircle(p.pos.x, p.pos.y, p.size * 4.5);
        
        // Medium glow
        ofSetColor(p.color.r, p.color.g, p.color.b, p.color.a * 0.25);
        ofDrawCircle(p.pos.x, p.pos.y, p.size * 2.2);
        
        // Inner bright glow
        ofSetColor(p.color.r, p.color.g, p.color.b, p.color.a * 0.6);
        ofDrawCircle(p.pos.x, p.pos.y, p.size * 1.3);
        
        // Bright core
        ofSetColor(p.color);
        ofDrawCircle(p.pos.x, p.pos.y, p.size * 0.7);
        
        // Ultra-bright center
        if(intensity > 0.7) {
            ofSetColor(200, 200, 200, (intensity - 0.7) * 150);
            ofDrawCircle(p.pos.x, p.pos.y, p.size * 0.3);
        }
        
        // Enhanced sparkle effects
        if(intensity > 0.8) {
            ofSetColor(180, 180, 180, (intensity - 0.8) * 200);
            float sparkleSize = p.size * 0.5;
            
            // Cross sparkle
            ofDrawLine(p.pos.x - sparkleSize, p.pos.y, p.pos.x + sparkleSize, p.pos.y);
            ofDrawLine(p.pos.x, p.pos.y - sparkleSize, p.pos.x, p.pos.y + sparkleSize);
            
            // Diagonal sparkle
            float diagSize = sparkleSize * 0.7;
            ofDrawLine(p.pos.x - diagSize, p.pos.y - diagSize, p.pos.x + diagSize, p.pos.y + diagSize);
            ofDrawLine(p.pos.x - diagSize, p.pos.y + diagSize, p.pos.x + diagSize, p.pos.y - diagSize);
        }
        
        // Energy field visualization for high-energy particles
        if(p.energy > 0.8) {
            ofSetColor(p.color.r, p.color.g, p.color.b, p.energy * 60);
            float fieldRadius = p.size * (2 + p.energy * 3);
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawCircle(p.pos.x, p.pos.y, fieldRadius);
            ofFill();
        }
    }
    
    ofDisableBlendMode();
    
    // Ultra-enhanced UI with pattern info
    // ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    // ofSetColor(0, 0, 0, 120);
    // ofDrawRectangle(10, 10, 360, 90);
    
    // ofSetColor(255, 230);
    // string patternNames[] = {"Star", "Flower", "Spiral", "Wave"};
    // string info = "FPS: " + ofToString(ofGetFrameRate(), 1) + 
    //               "   Particles: " + ofToString(particles.size()) +
    //               "\nPattern: " + patternNames[currentPattern] +
    //               "   Time: " + ofToString((int)(time / 15) + 1) + "/∞" +
    //               "\nPress 't' trails   'r' reset   's' save";
    // ofDrawBitmapString(info, 20, 30);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 't' || key == 'T') {
        showTrails = !showTrails;
    }
    if(key == 'r' || key == 'R') {
        particles.clear();
        time = 0;
    }
    if(key == 's' || key == 'S') {
        string filename = "generative_art_" + ofGetTimestampString() + ".png";
        ofSaveScreen(filename);
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
