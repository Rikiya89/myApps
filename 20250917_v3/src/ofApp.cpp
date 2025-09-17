#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(80, 70, 120);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    palette.push_back(ofColor::fromHex(0x362d78) * 1.8);
    palette.push_back(ofColor::fromHex(0x523fa3) * 1.8);
    palette.push_back(ofColor::fromHex(0x916ccc) * 1.6);
    palette.push_back(ofColor::fromHex(0xbda1e5) * 1.4);
    palette.push_back(ofColor::fromHex(0xc8c0e9) * 1.3);
    palette.push_back(ofColor::fromHex(0x84bae7) * 1.5);
    palette.push_back(ofColor::fromHex(0x516ad4) * 1.7);
    palette.push_back(ofColor::fromHex(0x333f87) * 1.9);
    palette.push_back(ofColor(255, 255, 255));
    palette.push_back(ofColor(255, 240, 255));
    palette.push_back(ofColor(240, 200, 255));
    palette.push_back(ofColor(200, 180, 255));
    
    numParticles = 1200;
    particles.resize(numParticles);
    velocities.resize(numParticles);
    particleSizes.resize(numParticles);
    particleColors.resize(numParticles);
    particleLife.resize(numParticles);
    particleMaxLife.resize(numParticles);
    
    for(int i = 0; i < numParticles; i++){
        particles[i].set(ofRandom(-400, 400), ofRandom(-400, 400), ofRandom(-400, 400));
        velocities[i].set(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        particleSizes[i] = ofRandom(1, 12);
        particleColors[i] = palette[int(ofRandom(palette.size()))];
        particleLife[i] = ofRandom(0, 1);
        particleMaxLife[i] = ofRandom(3, 8);
    }
    
    // Initialize flower elements
    int numFlowers = 8;
    flowerPositions.resize(numFlowers);
    flowerRotations.resize(numFlowers);
    flowerColors.resize(numFlowers);
    
    for(int i = 0; i < numFlowers; i++){
        float angle = (TWO_PI / numFlowers) * i;
        flowerPositions[i].set(cos(angle) * 300, sin(angle) * 200, ofRandom(-100, 100));
        flowerRotations[i] = ofRandom(0, TWO_PI);
        flowerColors[i] = palette[int(ofRandom(palette.size()))];
    }
    
    // Initialize flowing paths
    numPaths = 6;
    flowingPaths.resize(numPaths);
    pathColors.resize(numPaths);
    
    for(int p = 0; p < numPaths; p++){
        flowingPaths[p].resize(50);
        pathColors[p] = palette[int(ofRandom(palette.size()))];
        
        for(int i = 0; i < 50; i++){
            float t = float(i) / 49.0;
            float radius = 250 + sin(t * TWO_PI * 3) * 100;
            float angle = t * TWO_PI * 2 + p * (TWO_PI / numPaths);
            flowingPaths[p][i].set(
                cos(angle) * radius,
                sin(t * TWO_PI * 4) * 150,
                sin(angle) * radius
            );
        }
    }
    
    // Initialize lights
    int numLights = 5;
    lightPositions.resize(numLights);
    lightColors.resize(numLights);
    lightIntensities.resize(numLights);
    
    for(int i = 0; i < numLights; i++){
        lightPositions[i].set(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        lightColors[i] = palette[int(ofRandom(palette.size()))];
        lightIntensities[i] = ofRandom(0.5, 1.5);
    }
    
    // Setup FBO for post-processing
    fbo.allocate(720, 1080, GL_RGBA);
    
    time = 0;
    noiseScale = 0.01;
    waveAmplitude = 100;
    
    camera.setDistance(800);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.015;
    
    // Update particles with advanced behaviors
    for(int i = 0; i < numParticles; i++){
        // Multiple noise layers for complex motion
        float noise1 = ofNoise(particles[i].x * noiseScale, particles[i].y * noiseScale, particles[i].z * noiseScale, time);
        float noise2 = ofNoise(particles[i].x * noiseScale * 2, particles[i].y * noiseScale * 2, particles[i].z * noiseScale * 2, time * 1.5);
        float noise3 = ofNoise(particles[i].x * noiseScale * 0.5, particles[i].y * noiseScale * 0.5, particles[i].z * noiseScale * 0.5, time * 0.7);
        
        float combinedNoise = (noise1 + noise2 * 0.5 + noise3 * 0.3) / 1.8;
        float angle = combinedNoise * TWO_PI * 6;
        
        // Spiral force
        ofVec3f spiralForce;
        spiralForce.x = cos(angle + time) * 0.08;
        spiralForce.y = sin(angle * 1.3 + time * 0.8) * 0.08;
        spiralForce.z = sin(combinedNoise * TWO_PI + time) * 0.08;
        
        // Attraction to center with breathing effect
        ofVec3f centerAttraction = particles[i] * -0.001 * (1 + sin(time * 2) * 0.3);
        
        // Particle interaction forces
        ofVec3f interactionForce(0, 0, 0);
        for(int j = 0; j < numParticles; j += 10){
            if(i != j){
                float dist = particles[i].distance(particles[j]);
                if(dist < 60 && dist > 0){
                    ofVec3f dir = (particles[i] - particles[j]).getNormalized();
                    interactionForce += dir * (60 - dist) * 0.002;
                }
            }
        }
        
        velocities[i] += spiralForce + centerAttraction + interactionForce;
        velocities[i] *= 0.985;
        
        particles[i] += velocities[i];
        
        // Complex wave motion
        float wave1 = sin(particles[i].x * 0.008 + time * 1.2) * waveAmplitude * 0.8;
        float wave2 = cos(particles[i].z * 0.012 + time * 0.9) * waveAmplitude * 0.6;
        particles[i].y += (wave1 + wave2) * 0.008;
        
        // Update particle life
        particleLife[i] += 0.016;
        if(particleLife[i] > particleMaxLife[i]){
            particleLife[i] = 0;
            particles[i] = ofVec3f(ofRandom(-80, 80), ofRandom(-80, 80), ofRandom(-80, 80));
            velocities[i] = ofVec3f(ofRandom(-0.3, 0.3), ofRandom(-0.3, 0.3), ofRandom(-0.3, 0.3));
            particleColors[i] = palette[int(ofRandom(palette.size()))];
            particleMaxLife[i] = ofRandom(4, 10);
        }
        
        // Color breathing
        float breathe = sin(time * 3 + i * 0.1) * 0.5 + 0.5;
        ofColor baseColor = palette[i % palette.size()];
        ofColor breatheColor = palette[(i + 1) % palette.size()];
        particleColors[i] = baseColor.lerp(breatheColor, breathe * 0.3);
    }
    
    // Update flower rotations
    for(int i = 0; i < flowerPositions.size(); i++){
        flowerRotations[i] += 0.02 + sin(time + i) * 0.01;
        
        // Orbital motion
        float orbitRadius = 320 + sin(time * 0.8 + i) * 80;
        float angle = (TWO_PI / flowerPositions.size()) * i + time * 0.3;
        flowerPositions[i].x = cos(angle) * orbitRadius;
        flowerPositions[i].z = sin(angle) * orbitRadius;
        flowerPositions[i].y = sin(time * 1.5 + i * 0.7) * 150;
    }
    
    // Update flowing paths
    for(int p = 0; p < numPaths; p++){
        for(int i = 0; i < flowingPaths[p].size(); i++){
            float t = float(i) / (flowingPaths[p].size() - 1);
            float radius = 280 + sin(t * TWO_PI * 4 + time + p) * 120;
            float height = sin(t * TWO_PI * 3 + time * 1.2 + p * 0.5) * 200;
            float angle = t * TWO_PI * 3 + time * 0.8 + p * (TWO_PI / numPaths);
            
            flowingPaths[p][i].set(
                cos(angle) * radius,
                height,
                sin(angle) * radius
            );
        }
    }
    
    // Update light positions
    for(int i = 0; i < lightPositions.size(); i++){
        float lightTime = time * (0.5 + i * 0.2);
        lightPositions[i].x = cos(lightTime) * (200 + sin(lightTime * 0.7) * 100);
        lightPositions[i].y = sin(lightTime * 1.3) * 250;
        lightPositions[i].z = sin(lightTime) * (200 + cos(lightTime * 0.8) * 100);
        lightIntensities[i] = 0.8 + sin(time * 2 + i) * 0.4;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Begin FBO for post-processing
    fbo.begin();
    ofClear(0, 0, 0, 0);
    
    camera.begin();
    
    // Main transformation matrix
    ofPushMatrix();
    ofRotateYDeg(time * 8);
    ofRotateXDeg(time * 3);
    ofRotateZDeg(sin(time * 0.7) * 15);
    
    // Draw flowing paths first (background layer)
    ofSetLineWidth(3);
    for(int p = 0; p < numPaths; p++){
        ofNoFill();
        
        // Create gradient along path
        for(int i = 0; i < flowingPaths[p].size() - 1; i++){
            float t = float(i) / (flowingPaths[p].size() - 1);
            float alpha = sin(t * PI + time * 2 + p) * 0.4 + 0.8;
            alpha *= ofClamp(220 + sin(time * 1.5 + p) * 140, 0, 255);
            
            ofColor pathColor = pathColors[p];
            pathColor.a = ofClamp(alpha, 0, 255);
            ofSetColor(pathColor);
            
            ofDrawLine(flowingPaths[p][i], flowingPaths[p][i+1]);
        }
    }
    
    // Draw dynamic light effects
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for(int i = 0; i < lightPositions.size(); i++){
        ofPushMatrix();
        ofTranslate(lightPositions[i]);
        
        // Light halo
        ofColor lightColor = lightColors[i];
        lightColor.a = ofClamp(lightIntensities[i] * 220, 0, 255);
        ofSetColor(lightColor);
        
        for(int ring = 0; ring < 7; ring++){
            float radius = (ring + 1) * 25 * lightIntensities[i];
            float alpha = ofClamp((7 - ring) * 60 * lightIntensities[i], 0, 255);
            lightColor.a = alpha;
            ofSetColor(lightColor);
            ofNoFill();
            ofSetLineWidth(4);
            ofDrawCircle(0, 0, 0, radius);
        }
        
        // Light sphere
        lightColor.a = ofClamp(lightIntensities[i] * 255, 0, 255);
        ofSetColor(lightColor);
        ofFill();
        ofDrawSphere(0, 0, 0, 15 * lightIntensities[i]);
        ofPopMatrix();
    }
    
    // Draw flower structures
    for(int i = 0; i < flowerPositions.size(); i++){
        ofPushMatrix();
        ofTranslate(flowerPositions[i]);
        ofRotateYDeg(flowerRotations[i] * 57.2958);
        ofRotateXDeg(sin(time + i) * 20);
        
        ofColor flowerColor = flowerColors[i];
        flowerColor.a = ofClamp(200 + sin(time * 2 + i) * 120, 0, 255);
        ofSetColor(flowerColor);
        
        // Draw flower petals
        int numPetals = 8;
        for(int p = 0; p < numPetals; p++){
            ofPushMatrix();
            ofRotateZDeg((360.0 / numPetals) * p);
            
            // Petal shape using bezier curves
            ofNoFill();
            ofSetLineWidth(4);
            ofBeginShape();
            for(int t = 0; t <= 20; t++){
                float angle = ofMap(t, 0, 20, 0, PI);
                float radius = sin(angle) * (40 + sin(time * 3 + i + p) * 10);
                float x = cos(angle * 0.7) * radius;
                float y = sin(angle) * radius * 0.7;
                ofVertex(x, y, 0);
            }
            ofEndShape();
            ofPopMatrix();
        }
        
        // Flower center with bright white core
        ofFill();
        ofSetColor(255, 255, 255, 180);
        ofDrawSphere(0, 0, 0, 8);
        ofSetColor(flowerColor);
        ofDrawSphere(0, 0, 0, 5);
        ofPopMatrix();
    }
    
    // Enhanced particle system
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    for(int i = 0; i < numParticles; i++){
        mesh.addVertex(particles[i]);
        
        // Complex alpha calculation based on multiple factors
        float distAlpha = ofMap(particles[i].length(), 0, 500, 255, 200);
        float lifeAlpha = ofMap(particleLife[i], 0, particleMaxLife[i], 255, 220);
        float breatheAlpha = sin(time * 4 + i * 0.05) * 0.3 + 1.0;
        float finalAlpha = ofClamp(distAlpha * breatheAlpha * (lifeAlpha / 255.0), 0, 255);
        
        ofColor c = particleColors[i];
        c.a = finalAlpha;
        mesh.addColor(c);
    }
    
    glPointSize(12);
    mesh.draw();
    
    // Add bright white sparkles
    ofSetColor(255, 255, 255, 200);
    glPointSize(4);
    for(int i = 0; i < numParticles; i += 5){
        if(sin(time * 8 + i * 0.2) > 0.7){
            ofDrawSphere(particles[i], 2);
        }
    }
    
    // Draw particles as spheres with enhanced lighting
    for(int i = 0; i < numParticles; i += 2){
        ofPushMatrix();
        ofTranslate(particles[i]);
        
        float size = particleSizes[i] * (1 + sin(time * 3 + i * 0.08) * 0.4);
        float pulse = sin(time * 5 + i * 0.1) * 0.5 + 0.5;
        
        ofColor sphereColor = particleColors[i];
        sphereColor.a = ofClamp(220 + pulse * 150, 0, 255);
        ofSetColor(sphereColor);
        ofDrawSphere(0, 0, 0, size);
        
        // Add particle trails with white highlights
        if(i > 0){
            ofSetColor(sphereColor, 200);
            ofSetLineWidth(5);
            ofDrawLine(particles[i], particles[i-1]);
            
            // White highlight trail
            if(i % 3 == 0){
                ofSetColor(255, 255, 255, 100);
                ofSetLineWidth(2);
                ofDrawLine(particles[i], particles[i-1]);
            }
        }
        
        ofPopMatrix();
    }
    
    // Enhanced connection system
    ofSetColor(255, 100);
    for(int i = 0; i < numParticles - 1; i += 3){
        for(int j = i + 1; j < min(i + 20, numParticles); j++){
            float dist = particles[i].distance(particles[j]);
            if(dist < 100){
                float alpha = ofMap(dist, 0, 100, 180, 40);
                ofColor connectionColor = palette[(i + j) % palette.size()];
                connectionColor.a = alpha;
                ofSetColor(connectionColor);
                ofSetLineWidth(3 + (100 - dist) * 0.04);
                ofDrawLine(particles[i], particles[j]);
            }
        }
    }
    
    // Geometric mandala structures
    for(int layer = 0; layer < 3; layer++){
        ofPushMatrix();
        ofRotateYDeg(time * (15 + layer * 5) + layer * 60);
        ofRotateXDeg(sin(time + layer) * 25);
        ofRotateZDeg(cos(time * 1.3 + layer) * 20);
        
        int numSides = 6 + layer * 2;
        float radius = 180 + layer * 60 + sin(time + layer) * 30;
        
        ofNoFill();
        ofColor mandalaColor = palette[(layer * 3) % palette.size()];
        mandalaColor.a = ofClamp(180 + sin(time * 2 + layer) * 120, 0, 255);
        ofSetColor(mandalaColor);
        ofSetLineWidth(4 + layer * 2);
        
        // Draw mandala
        ofBeginShape();
        for(int i = 0; i <= numSides; i++){
            float angle = (TWO_PI / numSides) * i;
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            float z = sin(angle * 3 + time) * 20;
            ofVertex(x, y, z);
        }
        ofEndShape();
        
        // Inner details
        for(int ring = 1; ring <= 3; ring++){
            float innerRadius = radius * (ring / 4.0);
            ofBeginShape();
            for(int i = 0; i <= numSides * 2; i++){
                float angle = (TWO_PI / (numSides * 2)) * i + time * ring;
                float x = cos(angle) * innerRadius;
                float y = sin(angle) * innerRadius;
                float z = cos(angle * 2 + time) * 10;
                ofVertex(x, y, z);
            }
            ofEndShape();
        }
        
        ofPopMatrix();
    }
    
    // Add beautiful aurora waves
    for(int wave = 0; wave < 4; wave++){
        ofPushMatrix();
        ofRotateYDeg(time * 15 + wave * 90);
        ofRotateXDeg(sin(time * 0.8 + wave) * 30);
        
        ofNoFill();
        ofColor waveColor = palette[(wave + 8) % palette.size()];
        waveColor.a = 150 + sin(time * 3 + wave) * 100;
        ofSetColor(waveColor);
        ofSetLineWidth(6);
        
        ofBeginShape();
        for(int i = 0; i <= 80; i++){
            float angle = ofMap(i, 0, 80, 0, TWO_PI * 3);
            float radius = 400 + sin(angle * 0.5 + time + wave) * 150;
            float height = sin(angle + time * 2 + wave) * 200;
            float x = cos(angle) * radius;
            float y = height;
            float z = sin(angle) * radius;
            ofVertex(x, y, z);
        }
        ofEndShape();
        ofPopMatrix();
    }
    
    // Add geometric starbursts
    for(int star = 0; star < 6; star++){
        ofPushMatrix();
        float starTime = time * (1 + star * 0.2);
        ofTranslate(
            cos(starTime) * 350,
            sin(starTime * 1.3) * 200,
            sin(starTime * 0.7) * 350
        );
        ofRotateYDeg(starTime * 45);
        ofRotateXDeg(starTime * 30);
        
        ofColor starColor = palette[(star + 9) % palette.size()];
        starColor.a = 200;
        ofSetColor(starColor);
        ofSetLineWidth(3);
        
        // Draw starburst rays
        for(int ray = 0; ray < 12; ray++){
            ofPushMatrix();
            ofRotateZDeg(ray * 30);
            float rayLength = 80 + sin(time * 4 + star + ray) * 40;
            
            ofNoFill();
            ofBeginShape();
            ofVertex(0, 0, 0);
            ofVertex(rayLength, 0, 0);
            ofVertex(rayLength * 0.8, rayLength * 0.2, 0);
            ofVertex(rayLength, 0, 0);
            ofVertex(rayLength * 0.8, -rayLength * 0.2, 0);
            ofEndShape();
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    
    ofPopMatrix();
    camera.end();
    fbo.end();
    
    // Draw FBO to screen with post-processing
    ofSetColor(255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // Quadruple bloom effect for maximum brightness
    ofPushMatrix();
    ofScale(1.008, 1.008);
    ofSetColor(255, 100);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    // Secondary bloom
    ofPushMatrix();
    ofScale(1.004, 1.004);
    ofSetColor(255, 80);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    // Tertiary bloom
    ofPushMatrix();
    ofScale(1.002, 1.002);
    ofSetColor(255, 60);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    // Quaternary bloom
    ofPushMatrix();
    ofScale(1.001, 1.001);
    ofSetColor(255, 40);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    // White highlight overlay
    ofSetColor(255, 255, 255, 20);
    fbo.draw(0, 0);
    
    // Main render
    ofSetColor(255);
    fbo.draw(0, 0);
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
