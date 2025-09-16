#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofBackground(20, 25, 35);
    
    initializeColorPalettes();
    initializeParticles();
    
    globalTime = 0.0f;
    wavePhase = 0.0f;
    energyLevel = 1.0f;
    showTrails = true;
    mouseDragging = false;
    worldCenter = ofVec3f(0, 0, 0);
    mouseForce = ofVec3f(0, 0, 0);
    
    cam.setDistance(800);
    cam.setNearClip(1.0f);
    cam.setFarClip(2000.0f);
    
    ambientLight.setAmbientColor(ofFloatColor(0.1f, 0.15f, 0.25f));
    pointLight.setPointLight();
    pointLight.setPosition(200, -300, 400);
    pointLight.setDiffuseColor(ofFloatColor(0.9f, 0.95f, 1.0f));
    pointLight.setSpecularColor(ofFloatColor(1.0f, 1.0f, 1.0f));
    
    secondaryLight.setPointLight();
    secondaryLight.setPosition(-300, 200, -200);
    secondaryLight.setDiffuseColor(ofFloatColor(0.6f, 0.4f, 0.9f));
    secondaryLight.setSpecularColor(ofFloatColor(0.8f, 0.6f, 1.0f));
    
    connectionMesh.setMode(OF_PRIMITIVE_LINES);
    
    bloomIntensity = 1.5f;
    atmosphericDensity = 0.3f;
    enableBloom = false;
    enableStarField = false;
    enableGalaxyMode = false;
    musicReactivity = 1.0f;
    
    starField.resize(500);
    starBrightness.resize(500);
    for (int i = 0; i < 500; i++) {
        starField[i] = ofVec3f(
            ofRandom(-2000, 2000),
            ofRandom(-2000, 2000),
            ofRandom(-2000, 2000)
        );
        starBrightness[i] = ofRandom(0.1f, 1.0f);
    }
    
    galaxyColors = {
        ofColor::fromHex(0x362d78),
        ofColor::fromHex(0x523fa3),
        ofColor::fromHex(0x916ccc),
        ofColor::fromHex(0x84bae7),
        ofColor::fromHex(0x516ad4)
    };
}

void ofApp::initializeColorPalettes() {
    primaryColors = {{
        ofColor::fromHex(0x362d78),  // Deep purple
        ofColor::fromHex(0x523fa3),  // Medium purple
        ofColor::fromHex(0x916ccc),  // Light purple
        ofColor::fromHex(0xbda1e5),  // Pale purple
        ofColor::fromHex(0xc8c0e9),  // Very pale purple
        ofColor::fromHex(0x84bae7),  // Sky blue
        ofColor::fromHex(0x516ad4),  // Medium blue
        ofColor::fromHex(0x333f87),  // Dark blue
        ofColor::fromHex(0x293039),  // Dark gray-purple
        ofColor::fromHex(0x283631),  // Dark gray-green
        ofColor::fromHex(0x362d78),  // Repeat for extra variety
        ofColor::fromHex(0x916ccc)   // Repeat for extra variety
    }};
    
    accentColors = {{
        ofColor::fromHex(0x916ccc),  // Light purple (brightened)
        ofColor::fromHex(0xbda1e5),  // Pale purple (brightened)  
        ofColor::fromHex(0xc8c0e9),  // Very pale purple (brightened)
        ofColor::fromHex(0x84bae7),  // Sky blue (brightened)
        ofColor::fromHex(0x516ad4),  // Medium blue (brightened)
        ofColor::fromHex(0x523fa3),  // Medium purple (brightened)
        ofColor::fromHex(0x362d78),  // Deep purple (brightened)
        ofColor::fromHex(0x333f87)   // Dark blue (brightened)
    }};
}

void ofApp::initializeParticles() {
    particles.resize(MAX_PARTICLES);
    particleTrails.resize(MAX_PARTICLES);
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle& p = particles[i];
        
        float radius = ofRandom(50, WORLD_SIZE * 0.8f);
        float theta = ofRandom(0, TWO_PI);
        float phi = ofRandom(0, PI);
        
        p.position = ofVec3f(
            radius * sin(phi) * cos(theta),
            radius * sin(phi) * sin(theta),
            radius * cos(phi)
        );
        
        p.velocity = ofVec3f(
            ofRandom(-1.5f, 1.5f),
            ofRandom(-1.5f, 1.5f),
            ofRandom(-1.5f, 1.5f)
        );
        
        p.rotation = ofVec3f(
            ofRandom(0, 360),
            ofRandom(0, 360),
            ofRandom(0, 360)
        );
        
        p.rotationSpeed = ofVec3f(
            ofRandom(-3, 3),
            ofRandom(-3, 3),
            ofRandom(-3, 3)
        );
        
        p.scale = ofRandom(0.5f, 2.0f);
        p.life = ofRandom(0.7f, 1.0f);
        p.age = 0.0f;
        p.maxAge = ofRandom(10.0f, 30.0f);
        p.shapeType = i % 5;
        p.baseColor = primaryColors[i % primaryColors.size()];
        p.birthPosition = p.position;
        p.energy = ofRandom(0.5f, 1.5f);
        p.isDead = false;
        p.sparklePhase = ofRandom(0, TWO_PI);
        p.breathingPhase = ofRandom(0, TWO_PI);
        
        particleTrails[i].clear();
        particleTrails[i].reserve(25);
    }
}

void ofApp::update() {
    float deltaTime = ofGetLastFrameTime();
    globalTime += deltaTime;
    wavePhase += deltaTime * 0.8f;
    
    energyLevel = 0.8f + 0.4f * sin(globalTime * 0.3f);
    
    updateParticles();
    updateParticleLife();
    spawnNewParticles();
    
    pointLight.setPosition(
        sin(globalTime * 0.2f) * 300,
        cos(globalTime * 0.15f) * 200,
        sin(globalTime * 0.25f) * 150 + 200
    );
    
    secondaryLight.setPosition(
        cos(globalTime * 0.3f) * -250,
        sin(globalTime * 0.25f) * 180,
        cos(globalTime * 0.4f) * -150 + 100
    );
    
    atmosphericDensity = 0.2f + 0.15f * sin(globalTime * 0.4f);
    bloomIntensity = 1.2f + 0.8f * sin(globalTime * 0.6f) * energyLevel;
}

void ofApp::updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle& p = particles[i];
        
        ofVec3f attractionForce = worldCenter - p.position;
        float distance = attractionForce.length();
        attractionForce.normalize();
        attractionForce *= 0.02f * (1.0f / (distance * 0.01f + 1.0f));
        
        ofVec3f flowField = ofVec3f(
            sin(p.position.y * 0.01f + wavePhase) * 0.5f,
            cos(p.position.x * 0.01f + wavePhase * 1.2f) * 0.5f,
            sin(p.position.z * 0.008f + wavePhase * 0.8f) * 0.3f
        );
        
        ofVec3f noiseForce = ofVec3f(
            ofNoise(p.position.x * 0.005f, globalTime * 0.2f) - 0.5f,
            ofNoise(p.position.y * 0.005f, globalTime * 0.15f) - 0.5f,
            ofNoise(p.position.z * 0.005f, globalTime * 0.25f) - 0.5f
        ) * 0.8f;
        
        p.velocity += (attractionForce + flowField + noiseForce + mouseForce) * energyLevel;
        p.velocity *= 0.98f;
        
        float speedLimit = 3.0f * energyLevel;
        if (p.velocity.length() > speedLimit) {
            p.velocity.normalize();
            p.velocity *= speedLimit;
        }
        
        if (showTrails && particleTrails[i].size() > 0) {
            particleTrails[i].push_back(p.position);
            if (particleTrails[i].size() > 15) {
                particleTrails[i].erase(particleTrails[i].begin());
            }
        }
        
        p.position += p.velocity;
        p.rotation += p.rotationSpeed * energyLevel;
        
        if (p.position.length() > WORLD_SIZE) {
            p.position.normalize();
            p.position *= WORLD_SIZE;
            p.velocity *= -0.3f;
        }
        
        p.life = ofClamp(p.life + ofRandom(-0.002f, 0.001f), 0.3f, 1.0f);
        p.scale = 0.8f + 0.6f * sin(globalTime * 2.0f + i * 0.1f) * p.life;
        p.age += ofGetLastFrameTime();
        p.sparklePhase += ofGetLastFrameTime() * 3.0f;
        p.breathingPhase += ofGetLastFrameTime() * 1.5f;
    }
    
    mouseForce *= 0.95f;
}

void ofApp::draw() {
    ofBackground(20, 25, 35);
    
    ofEnableLighting();
    ambientLight.enable();
    pointLight.enable();
    
    cam.begin();
    
    if (enableStarField) {
        drawStarField();
    }
    
    if (enableGalaxyMode) {
        drawGalaxyEffects();
    }
    
    if (showTrails) {
        drawParticleTrails();
    }
    
    drawConnections();
    drawParticles();
    drawBackgroundShapes();
    
    cam.end();
    
    pointLight.disable();
    ambientLight.disable();
    ofDisableLighting();
}

void ofApp::drawConnections() {
    connectionMesh.clear();
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        for (int j = i + 1; j < MAX_PARTICLES; j++) {
            float distance = particles[i].position.distance(particles[j].position);
            if (distance < CONNECTION_DISTANCE) {
                float alpha = ofMap(distance, 0, CONNECTION_DISTANCE, 0.8f, 0.0f);
                alpha *= particles[i].life * particles[j].life * energyLevel;
                
                ofColor lineColor = primaryColors[(i + j) % primaryColors.size()];
                lineColor.a = alpha * 120;
                
                connectionMesh.addVertex(particles[i].position);
                connectionMesh.addColor(lineColor);
                connectionMesh.addVertex(particles[j].position);
                connectionMesh.addColor(lineColor);
            }
        }
    }
    
    ofSetLineWidth(1.5f);
    connectionMesh.draw();
}

void ofApp::drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        const Particle& p = particles[i];
        
        if (p.isDead) continue;
        
        ofPushMatrix();
        ofTranslate(p.position);
        ofRotateXDeg(p.rotation.x);
        ofRotateYDeg(p.rotation.y);
        ofRotateZDeg(p.rotation.z);
        
        ofColor particleColor = p.baseColor;
        float brightness = p.life * energyLevel;
        float pulse = sin(globalTime * 3.0f + i * 0.2f) * 0.3f + 0.7f;
        brightness *= pulse;
        
        particleColor.setBrightness(255 * brightness);
        ofSetColor(particleColor);
        
        float size = 8.0f + 4.0f * sin(globalTime * 1.5f + i * 0.15f);
        size *= p.scale;
        
        switch (p.shapeType % 4) {
            case 0:
                ofDrawBox(size);
                break;
            case 1:
                ofDrawSphere(size * 0.8f);
                break;
            case 2:
                ofDrawCone(size * 0.7f, size * 1.8f);
                break;
            case 3:
                ofDrawIcoSphere(size * 0.6f);
                break;
        }
        
        ofPopMatrix();
    }
}

void ofApp::drawBackgroundShapes() {
    for (int i = 0; i < NUM_BACKGROUND_SHAPES; i++) {
        ofPushMatrix();
        
        float orbitRadius = 300 + i * 15;
        float orbitSpeed = 0.1f + i * 0.02f;
        
        float x = sin(globalTime * orbitSpeed + i * 0.5f) * orbitRadius;
        float y = cos(globalTime * orbitSpeed * 0.7f + i * 0.8f) * orbitRadius * 0.6f;
        float z = sin(globalTime * orbitSpeed * 0.4f + i * 1.2f) * orbitRadius * 0.3f;
        
        ofTranslate(x, y, z);
        ofRotateXDeg(globalTime * 15 + i * 25);
        ofRotateYDeg(globalTime * 12 + i * 45);
        ofRotateZDeg(globalTime * 8 + i * 35);
        
        ofColor bgColor = primaryColors[(i * 2) % primaryColors.size()];
        bgColor.a = 40 * energyLevel;
        ofSetColor(bgColor);
        
        float size = 8 + sin(globalTime + i * 0.3f) * 3;
        
        if (i % 3 == 0) {
            ofDrawBox(size);
        } else if (i % 3 == 1) {
            ofDrawIcoSphere(size * 0.8f);
        } else {
            ofDrawCylinder(size * 0.6f, size * 2.0f);
        }
        
        ofPopMatrix();
    }
}

void ofApp::drawParticleTrails() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particleTrails[i].size() > 1) {
            ofMesh trailMesh;
            trailMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            
            for (int j = 0; j < particleTrails[i].size(); j++) {
                float alpha = ofMap(j, 0, particleTrails[i].size() - 1, 0.0f, 0.6f);
                alpha *= particles[i].life;
                
                ofColor trailColor = particles[i].baseColor;
                trailColor.a = alpha * 100;
                
                trailMesh.addVertex(particleTrails[i][j]);
                trailMesh.addColor(trailColor);
            }
            
            ofSetLineWidth(2.0f);
            trailMesh.draw();
        }
    }
}

void ofApp::exit() {
    
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case 't':
        case 'T':
            showTrails = !showTrails;
            if (!showTrails) {
                for (auto& trail : particleTrails) {
                    trail.clear();
                }
            }
            break;
            
        case 'r':
        case 'R':
            initializeParticles();
            break;
            
        case ' ':
            energyLevel = (energyLevel > 0.5f) ? 0.2f : 1.5f;
            break;
            
        case 'c':
        case 'C':
            initializeColorPalettes();
            for (int i = 0; i < MAX_PARTICLES; i++) {
                particles[i].baseColor = primaryColors[i % primaryColors.size()];
            }
            break;
            
        case 's':
        case 'S':
            cameraStabilized = !cameraStabilized;
            if (cameraStabilized) {
                cam.disableMouseInput();
                targetCameraAngles = ofVec2f(0, 0);
                targetCameraDistance = 800.0f;
            } else {
                cam.enableMouseInput();
            }
            break;
            
        case 'o':
        case 'O':
            autoOrbit = !autoOrbit;
            if (autoOrbit) {
                cameraStabilized = true;
                cam.disableMouseInput();
            }
            break;
            
        case 'b':
        case 'B':
            enableBloom = !enableBloom;
            break;
            
        case 'f':
        case 'F':
            enableStarField = !enableStarField;
            break;
            
        case 'g':
        case 'G':
            enableGalaxyMode = !enableGalaxyMode;
            break;
            
        case '1':
            cameraSmoothing = 0.02f;
            break;
        case '2':
            cameraSmoothing = 0.05f;
            break;
        case '3':
            cameraSmoothing = 0.1f;
            break;
            
        case '+':
        case '=':
            bloomIntensity = ofClamp(bloomIntensity + 0.2f, 0.0f, 3.0f);
            break;
            
        case '-':
        case '_':
            bloomIntensity = ofClamp(bloomIntensity - 0.2f, 0.0f, 3.0f);
            break;
            
        case OF_KEY_UP:
            if (cameraStabilized) {
                targetCameraAngles.y = ofClamp(targetCameraAngles.y + 5, -80, 80);
            }
            break;
        case OF_KEY_DOWN:
            if (cameraStabilized) {
                targetCameraAngles.y = ofClamp(targetCameraAngles.y - 5, -80, 80);
            }
            break;
        case OF_KEY_LEFT:
            if (cameraStabilized) {
                targetCameraAngles.x -= 5;
            }
            break;
        case OF_KEY_RIGHT:
            if (cameraStabilized) {
                targetCameraAngles.x += 5;
            }
            break;
    }
}

void ofApp::keyReleased(int key) {
    
}

void ofApp::mouseMoved(int x, int y) {
    
}

void ofApp::mouseDragged(int x, int y, int button) {
    if (!mouseDragging) return;
    
    ofVec3f screenPos(x, y, 0);
    ofVec3f worldPos = cam.screenToWorld(screenPos);
    
    ofVec3f force = worldPos - worldCenter;
    force.normalize();
    mouseForce = force * 2.0f;
}

void ofApp::mousePressed(int x, int y, int button) {
    mouseDragging = true;
    
    ofVec3f screenPos(x, y, 0);
    ofVec3f worldPos = cam.screenToWorld(screenPos);
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        ofVec3f force = worldPos - particles[i].position;
        float distance = force.length();
        if (distance < 100) {
            force.normalize();
            particles[i].velocity += force * 5.0f;
        }
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    mouseDragging = false;
    mouseForce = ofVec3f(0, 0, 0);
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    energyLevel = ofClamp(energyLevel + scrollY * 0.1f, 0.1f, 3.0f);
}

void ofApp::mouseEntered(int x, int y) {
    
}

void ofApp::mouseExited(int x, int y) {
    
}

void ofApp::windowResized(int w, int h) {
    
}

void ofApp::gotMessage(ofMessage msg) {
    
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
    
}

void ofApp::setupPostProcessing() {
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    if (w > 0 && h > 0) {
        mainFbo.allocate(w, h, GL_RGBA);
        bloomFbo.allocate(w, h, GL_RGBA);
        mainFbo.begin();
        ofClear(0, 0, 0, 0);
        mainFbo.end();
        bloomFbo.begin();
        ofClear(0, 0, 0, 0);
        bloomFbo.end();
    }
}

void ofApp::updateParticleLife() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle& p = particles[i];
        
        if (p.age > p.maxAge) {
            p.isDead = true;
        }
        
        float lifeRatio = p.age / p.maxAge;
        p.energy = 1.0f - lifeRatio * 0.7f;
        
        if (p.energy < 0.1f) {
            p.isDead = true;
        }
    }
}

void ofApp::spawnNewParticles() {
    int deadCount = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].isDead) {
            deadCount++;
            if (deadCount <= 5) {
                Particle& p = particles[i];
                
                float radius = ofRandom(100, WORLD_SIZE * 0.6f);
                float theta = ofRandom(0, TWO_PI);
                float phi = ofRandom(0, PI);
                
                p.position = ofVec3f(
                    radius * sin(phi) * cos(theta),
                    radius * sin(phi) * sin(theta),
                    radius * cos(phi)
                );
                
                p.velocity = ofVec3f(
                    ofRandom(-1.0f, 1.0f),
                    ofRandom(-1.0f, 1.0f),
                    ofRandom(-1.0f, 1.0f)
                );
                
                p.age = 0.0f;
                p.maxAge = ofRandom(15.0f, 40.0f);
                p.life = 1.0f;
                p.energy = 1.0f;
                p.isDead = false;
                p.baseColor = primaryColors[i % primaryColors.size()];
                p.sparklePhase = ofRandom(0, TWO_PI);
                p.breathingPhase = ofRandom(0, TWO_PI);
            }
        }
    }
}

void ofApp::drawStarField() {
    ofSetColor(255, 255, 255, 80);
    for (int i = 0; i < starField.size(); i++) {
        float twinkle = sin(globalTime * 2.0f + i * 0.1f) * 0.3f + 0.7f;
        float alpha = starBrightness[i] * twinkle * 150;
        ofSetColor(255, 255, 255, alpha);
        
        ofPushMatrix();
        ofTranslate(starField[i]);
        ofDrawSphere(1.0f + twinkle * 0.5f);
        ofPopMatrix();
    }
}

void ofApp::drawGalaxyEffects() {
    for (int i = 0; i < 100; i++) {
        float angle = globalTime * 0.1f + i * 0.1f;
        float radius = 200 + i * 8;
        float height = sin(angle * 3.0f + i * 0.05f) * 50;
        
        ofVec3f pos(
            cos(angle) * radius,
            height,
            sin(angle) * radius
        );
        
        ofColor galaxyColor = galaxyColors[i % galaxyColors.size()];
        galaxyColor.a = 60 * atmosphericDensity;
        ofSetColor(galaxyColor);
        
        ofPushMatrix();
        ofTranslate(pos);
        ofDrawSphere(2.0f + sin(globalTime * 2.0f + i * 0.2f) * 1.0f);
        ofPopMatrix();
    }
}

void ofApp::drawAtmosphericEffects() {
    for (int i = 0; i < 20; i++) {
        float scale = 50.0f + i * 25.0f;
        float alpha = (atmosphericDensity * 30.0f) / (i + 1);
        
        ofColor atmosphereColor = ofColor::fromHsb(
            200 + sin(globalTime * 0.2f + i * 0.1f) * 60,
            100,
            150
        );
        atmosphereColor.a = alpha;
        ofSetColor(atmosphereColor);
        
        ofPushMatrix();
        ofTranslate(0, 0, 0);
        ofRotateYDeg(globalTime * 5.0f + i * 10);
        ofRotateXDeg(sin(globalTime * 0.3f + i * 0.2f) * 20);
        ofDrawSphere(scale);
        ofPopMatrix();
    }
}

void ofApp::applyBloomEffect() {
    if (!mainFbo.isAllocated()) return;
    
    ofSetColor(255, 255, 255, 255 * bloomIntensity);
    mainFbo.draw(0, 0);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 255, 255, 100 * bloomIntensity);
    mainFbo.draw(0, 0);
    ofDisableBlendMode();
}
