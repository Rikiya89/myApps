#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(ofColor::fromHex(0x293039));
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    // Initialize color palette inspired by Avatar's Pandora ocean
    palette[0] = ofColor::fromHex(0x362d78);
    palette[1] = ofColor::fromHex(0x523fa3);
    palette[2] = ofColor::fromHex(0x916ccc);
    palette[3] = ofColor::fromHex(0xbda1e5);
    palette[4] = ofColor::fromHex(0xc8c0e9);
    palette[5] = ofColor::fromHex(0x84bae7);
    palette[6] = ofColor::fromHex(0x516ad4);
    palette[7] = ofColor::fromHex(0x333f87);
    palette[8] = ofColor::fromHex(0x293039);
    palette[9] = ofColor::fromHex(0x283631);
    
    time = 0.0f;
    
    // Initialize camera animation with gentle movements
    cameraRadius = 300;
    cameraHeight = 0;
    cameraAngle = 0;
    cameraSpeed = 0.15;  // Slower, more gentle
    cameraTarget = ofVec3f(0, 0, 0);
    gentleBreathe = 0;
    smoothEasing = 0;
    
    // Initialize particles for bioluminescent effects
    particles.resize(1500);
    for (int i = 0; i < particles.size(); i++) {
        particles[i].position = ofVec3f(
            ofRandom(-500, 500),
            ofRandom(-400, 400),
            ofRandom(-500, 500)
        );
        particles[i].velocity = ofVec3f(
            ofRandom(-0.8, 0.8),
            ofRandom(-0.3, 0.3),
            ofRandom(-0.8, 0.8)
        );
        particles[i].acceleration = ofVec3f(0, 0, 0);
        particles[i].type = (ParticleType)int(ofRandom(0, 4));
        
        // Different properties based on type
        switch(particles[i].type) {
            case PLANKTON:
                particles[i].color = palette[int(ofRandom(2, 6))];
                particles[i].size = ofRandom(3, 12);
                particles[i].trailLength = ofRandom(20, 50);
                break;
            case BUBBLE:
                particles[i].color = ofColor(palette[5], 120);
                particles[i].size = ofRandom(1, 4);
                particles[i].velocity.y -= ofRandom(0.5, 1.5);
                break;
            case SPARK:
                particles[i].color = palette[int(ofRandom(0, 4))];
                particles[i].size = ofRandom(1, 3);
                particles[i].trailLength = ofRandom(10, 30);
                break;
            case DUST:
                particles[i].color = ofColor(palette[8], 80);
                particles[i].size = ofRandom(0.5, 2);
                break;
        }
        
        particles[i].color.a = ofRandom(80, 255);
        particles[i].maxLife = ofRandom(400, 800);
        particles[i].life = particles[i].maxLife;
        particles[i].rotation = 0;
        particles[i].rotationSpeed = ofRandom(-1, 1);  // Gentler rotation
        particles[i].breathePhase = ofRandom(0, TWO_PI);
        particles[i].gentleSpeed = ofRandom(0.3, 0.8);
        particles[i].homePosition = particles[i].position;
        particles[i].trail.clear();
    }
    
    createOceanFloor();
    generateCorals();
    generateJellyfish();
    generateKelp();
    createGodRays();
    generateMagicOrbs();
    generateCrystals();
    
    cam.setNearClip(0.1);
    cam.setFarClip(3000);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    
    updateCamera();
    updateParticles();
    updateJellyfish();
    updateKelp();
    updateMagicOrbs();
    updateLightBeams();
    updateCrystals();
    
    // Update coral pulsing with gentle breathing
    for (auto& coral : corals) {
        coral.pulsePhase += 0.008; // Slower, more gentle pulsing
        coral.breatheIntensity = sin(time * 0.1 + coral.position.length() * 0.01) * 0.5 + 0.5;
        coral.gentleGlow = sin(time * 0.05 + coral.pulsePhase) * 0.3 + 0.7;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Dynamic gradient background with gentle color transitions
    float colorShift = sin(time * 0.02) * 0.5 + 0.5;
    ofColor bgTop = palette[8].getLerped(palette[7], colorShift * 0.3);
    ofColor bgBottom = palette[7].getLerped(palette[6], colorShift * 0.2);
    ofBackgroundGradient(bgTop, bgBottom, OF_GRADIENT_LINEAR);
    
    cam.begin();
    
    // Enhanced volumetric atmosphere with gentle breathing
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < 4; i++) {
        float breatheEffect = sin(time * 0.03 + i * 0.8) * 0.5 + 0.5;
        float gentleFloat = sin(time * (0.05 + i * 0.02)) * 25;
        float verticalFloat = cos(time * (0.04 + i * 0.015)) * 15;
        
        ofColor atmosphereColor = palette[4 + i].getLerped(palette[5 + i], breatheEffect);
        ofSetColor(atmosphereColor, int(5 + i * 2 + breatheEffect * 3));
        
        ofPushMatrix();
        ofTranslate(gentleFloat, verticalFloat, sin(time * 0.06 + i) * 20);
        ofScale(0.9 + breatheEffect * 0.2, 0.9 + breatheEffect * 0.1, 0.9 + breatheEffect * 0.15);
        ofDrawBox(0, 0, 0, 1100, 700, 1100);
        ofPopMatrix();
    }
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    drawGodRays();
    drawOceanFloor();
    drawCaustics();
    drawAurora();
    drawKelp();
    drawCrystals();
    drawCorals();
    drawMysticalRings();
    drawJellyfish();
    drawMagicOrbs();
    drawLightBeams();
    drawParticles();
    
    cam.end();
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
void ofApp::updateCamera(){
    // Ultra-smooth cinematic camera movement with gentle breathing
    gentleBreathe += 0.008;
    smoothEasing += 0.003;
    cameraAngle += cameraSpeed * ofGetLastFrameTime();
    
    // Gentle figure-8 motion with soft height variation using easing curves
    float breatheEffect = (sin(gentleBreathe) + 1) * 0.5; // 0 to 1
    float easedBreathe = ofMap(breatheEffect * breatheEffect * (3.0 - 2.0 * breatheEffect), 0, 1, 0, 1); // Smooth step easing
    
    float targetRadius = cameraRadius + sin(time * 0.03) * 60 * easedBreathe;
    float targetHeight = cameraHeight + sin(time * 0.02) * 80 + cos(time * 0.025) * 40;
    
    // Add gentle floating motion
    float floatX = sin(cameraAngle * 0.3) * 30 + cos(smoothEasing) * 20;
    float floatZ = cos(cameraAngle * 0.4) * 35 + sin(smoothEasing * 0.7) * 25;
    float floatY = sin(gentleBreathe * 0.5) * 15;
    
    cameraPosition.x = cos(cameraAngle) * targetRadius + floatX;
    cameraPosition.z = sin(cameraAngle) * targetRadius + floatZ;
    cameraPosition.y = targetHeight + floatY;
    
    // Gentle target movement with smooth transitions
    float targetBreathe = sin(time * 0.015) * 0.5 + 0.5;
    cameraTarget.x = sin(time * 0.02) * 40 * targetBreathe;
    cameraTarget.y = cos(time * 0.018) * 25 * targetBreathe;
    cameraTarget.z = sin(time * 0.012) * 30 * targetBreathe;
    
    cam.setPosition(cameraPosition);
    cam.lookAt(cameraTarget);
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
    for (int i = 0; i < particles.size(); i++) {
        auto& particle = particles[i];
        
        // Gentle breathing animation
        particle.breathePhase += 0.01 * particle.gentleSpeed;
        float breatheEffect = (sin(particle.breathePhase) + 1) * 0.5;
        
        // Ultra-gentle ocean current simulation
        ofVec3f currentForce;
        currentForce.x = sin(time * 0.3 + particle.position.z * 0.005) * 0.008;
        currentForce.y = sin(time * 0.2 + particle.position.x * 0.006) * 0.006;
        currentForce.z = cos(time * 0.25 + particle.position.y * 0.004) * 0.01;
        
        // Gentle home-seeking behavior
        ofVec3f homeForce = (particle.homePosition - particle.position) * 0.0001;
        currentForce += homeForce;
        
        // Soft particle interactions (flocking)
        ofVec3f separation, alignment, cohesion;
        int neighborCount = 0;
        
        for (int j = 0; j < particles.size(); j++) {
            if (i == j) continue;
            float distance = particle.position.distance(particles[j].position);
            
            if (distance < 80 && distance > 0.1) {
                neighborCount++;
                // Separation
                ofVec3f diff = particle.position - particles[j].position;
                diff.normalize();
                separation += diff / distance;
                
                // Alignment
                alignment += particles[j].velocity;
                
                // Cohesion
                cohesion += particles[j].position;
            }
        }
        
        if (neighborCount > 0) {
            separation *= 0.002;
            alignment /= neighborCount;
            alignment *= 0.001;
            cohesion /= neighborCount;
            cohesion = (cohesion - particle.position) * 0.0005;
            
            currentForce += separation + alignment + cohesion;
        }
        
        // Type-specific gentle behaviors
        switch(particle.type) {
            case BUBBLE:
                currentForce.y -= 0.2 * breatheEffect; // Gentle buoyancy
                currentForce += ofVec3f(
                    sin(time * 0.8 + i) * 0.003,
                    0,
                    cos(time * 0.6 + i) * 0.003
                );
                break;
            case PLANKTON:
                // Gentle schooling with breathing rhythm
                currentForce += ofVec3f(
                    sin(time * 0.5 + particle.position.length() * 0.05) * 0.01 * breatheEffect,
                    cos(time * 0.4 + particle.position.length() * 0.04) * 0.008 * breatheEffect,
                    sin(time * 0.6 + particle.position.length() * 0.06) * 0.012 * breatheEffect
                );
                break;
            case SPARK:
                // Gentle twinkling movement
                currentForce += ofVec3f(
                    sin(time * 2 + i * 0.5) * 0.015,
                    cos(time * 1.8 + i * 0.3) * 0.01,
                    sin(time * 2.2 + i * 0.7) * 0.018
                ) * breatheEffect;
                break;
            case DUST:
                currentForce *= 0.1; // Very slow, gentle drift
                break;
        }
        
        particle.acceleration = currentForce;
        particle.velocity += particle.acceleration;
        particle.velocity *= 0.995; // Gentler damping
        particle.position += particle.velocity * particle.gentleSpeed;
        
        // Update trail
        particle.trail.insert(particle.trail.begin(), particle.position);
        if (particle.trail.size() > 15) {
            particle.trail.pop_back();
        }
        
        particle.rotation += particle.rotationSpeed * breatheEffect;
        particle.life -= 0.3; // Slower life decay
        
        // Gentle boundary wrapping with breathing room
        float boundary = 500;
        if (particle.position.x > boundary) particle.position.x = -boundary;
        if (particle.position.x < -boundary) particle.position.x = boundary;
        if (particle.position.z > boundary) particle.position.z = -boundary;
        if (particle.position.z < -boundary) particle.position.z = boundary;
        if (particle.position.y > 350) particle.position.y = -350;
        if (particle.position.y < -350) particle.position.y = 350;
        
        // Gentle alpha animation with breathing
        float lifeRatio = particle.life / particle.maxLife;
        float gentlePulse = sin(time * 0.8 + particle.position.length() * 0.005) * 0.2 + 0.8;
        particle.color.a = int(ofMap(lifeRatio, 0, 1, 0, 200) * gentlePulse * breatheEffect);
        
        // Respawn particle gently
        if (particle.life <= 0) {
            particle.position = ofVec3f(ofRandom(-400, 400), ofRandom(-300, 300), ofRandom(-400, 400));
            particle.homePosition = particle.position;
            particle.velocity = ofVec3f(ofRandom(-0.3, 0.3), ofRandom(-0.1, 0.1), ofRandom(-0.3, 0.3));
            particle.life = particle.maxLife;
            particle.trail.clear();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (const auto& particle : particles) {
        // Draw gentle trails first
        if (particle.trail.size() > 1) {
            for (int i = 1; i < particle.trail.size(); i++) {
                float trailAlpha = ofMap(i, 1, particle.trail.size()-1, particle.color.a * 0.8, 0);
                float trailSize = ofMap(i, 1, particle.trail.size()-1, particle.size * 0.8, particle.size * 0.1);
                
                ofSetColor(particle.color, trailAlpha);
                ofDrawSphere(particle.trail[i], trailSize);
            }
        }
        
        ofPushMatrix();
        ofTranslate(particle.position);
        ofRotateZDeg(particle.rotation);
        
        // Gentle breathing effect for size
        float breatheEffect = (sin(particle.breathePhase) + 1) * 0.5;
        float gentleSize = particle.size * (0.8 + breatheEffect * 0.4);
        
        switch(particle.type) {
            case PLANKTON:
                // Main body with gentle pulsing
                ofSetColor(particle.color, particle.color.a * (0.7 + breatheEffect * 0.3));
                ofDrawSphere(0, 0, 0, gentleSize);
                
                // Soft outer glow
                ofSetColor(particle.color, particle.color.a * 0.2);
                ofDrawSphere(0, 0, 0, gentleSize * 2.2);
                
                // Gentle inner sparkle
                ofSetColor(255, 255, 255, particle.color.a * 0.4 * breatheEffect);
                ofDrawSphere(0, 0, 0, gentleSize * 0.2);
                break;
                
            case BUBBLE:
                // Soft bubble with gentle rim
                ofSetColor(particle.color, particle.color.a * 0.6);
                ofNoFill();
                ofSetLineWidth(1);
                ofDrawSphere(0, 0, 0, gentleSize);
                
                // Gentle highlight that breathes
                ofFill();
                ofSetColor(255, 255, 255, particle.color.a * 0.3 * breatheEffect);
                ofDrawSphere(gentleSize * 0.2, -gentleSize * 0.2, 0, gentleSize * 0.15);
                
                // Soft inner glow
                ofSetColor(particle.color, particle.color.a * 0.1);
                ofDrawSphere(0, 0, 0, gentleSize * 1.3);
                break;
                
            case SPARK:
                {
                    // Gentle twinkling spark
                    ofSetColor(particle.color, particle.color.a * (0.6 + breatheEffect * 0.4));
                    ofDrawSphere(0, 0, 0, gentleSize);
                    
                    // Soft radiating glow
                    ofSetLineWidth(1);
                    ofSetColor(particle.color, particle.color.a * 0.3 * breatheEffect);
                    float rayLength = gentleSize * 1.5;
                    ofDrawLine(-rayLength, 0, 0, rayLength, 0, 0);
                    ofDrawLine(0, -rayLength, 0, 0, rayLength, 0);
                    
                    // Additional soft glow
                    ofSetColor(particle.color, particle.color.a * 0.2);
                    ofDrawSphere(0, 0, 0, gentleSize * 2);
                    break;
                }
                
            case DUST:
                // Very gentle dust particle
                ofSetColor(particle.color, particle.color.a * 0.5);
                ofDrawSphere(0, 0, 0, gentleSize);
                
                // Extremely soft glow
                ofSetColor(particle.color, particle.color.a * 0.1);
                ofDrawSphere(0, 0, 0, gentleSize * 2.5);
                break;
        }
        
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawCorals(){
    for (const auto& coral : corals) {
        ofPushMatrix();
        ofTranslate(coral.position);
        
        float pulse = sin(coral.pulsePhase) * 0.4 + 0.6;
        float organicPulse = sin(coral.pulsePhase * 0.7) * 0.2 + 0.8;
        
        ofColor glowColor = coral.color;
        glowColor.a = int(glowColor.a * pulse);
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        // Draw main coral body with organic deformation
        ofPushMatrix();
        ofScale(pulse * organicPulse, pulse, pulse * (1.1 - organicPulse * 0.1));
        ofSetColor(glowColor);
        ofDrawSphere(0, 0, 0, coral.size);
        ofPopMatrix();
        
        // Add inner glow
        ofSetColor(coral.color, coral.color.a * 0.3 * pulse);
        ofDrawSphere(0, 0, 0, coral.size * 1.5);
        
        // Draw organic branches with variation
        for (int i = 0; i < coral.branches.size(); i++) {
            const auto& branch = coral.branches[i];
            float branchPulse = sin(coral.pulsePhase + i * 0.5) * 0.3 + 0.7;
            
            ofPushMatrix();
            ofVec3f branchPos = branch * coral.size * (0.6 + branchPulse * 0.3);
            ofTranslate(branchPos);
            
            // Organic scaling
            ofScale(
                branchPulse,
                branchPulse * (0.8 + sin(time * 2 + i) * 0.2),
                branchPulse
            );
            
            ofSetColor(glowColor, glowColor.a * branchPulse);
            ofDrawSphere(0, 0, 0, coral.size * 0.35);
            ofPopMatrix();
            
            // Draw connecting tendril with thickness variation
            ofSetColor(coral.color, coral.color.a * 0.6);
            
            ofPolyline tendril;
            int steps = 8;
            for (int j = 0; j <= steps; j++) {
                float t = float(j) / steps;
                ofVec3f pos = ofVec3f(0,0,0).interpolate(branchPos, t);
                
                // Add organic curve
                pos.x += sin(t * PI + coral.pulsePhase) * coral.size * 0.1;
                pos.y += cos(t * PI * 0.7 + coral.pulsePhase) * coral.size * 0.08;
                pos.z += sin(t * PI * 1.3 + coral.pulsePhase) * coral.size * 0.06;
                
                tendril.addVertex(pos);
            }
            
            ofSetLineWidth(4 * branchPulse);
            tendril.draw();
        }
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawOceanFloor(){
    ofSetColor(palette[8], 150);
    ofPushMatrix();
    ofTranslate(0, 250, 0);
    oceanFloor.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::createOceanFloor(){
    oceanFloor.setMode(OF_PRIMITIVE_TRIANGLES);
    
    int resolution = 50;
    float size = 800;
    
    for (int x = 0; x < resolution; x++) {
        for (int z = 0; z < resolution; z++) {
            float xPos = ofMap(x, 0, resolution-1, -size/2, size/2);
            float zPos = ofMap(z, 0, resolution-1, -size/2, size/2);
            
            // Create undulating floor with noise
            float height = ofNoise(x * 0.1, z * 0.1, time * 0.1) * 30;
            
            ofVec3f pos1(xPos, height, zPos);
            ofVec3f pos2(xPos + size/resolution, ofNoise((x+1) * 0.1, z * 0.1, time * 0.1) * 30, zPos);
            ofVec3f pos3(xPos, ofNoise(x * 0.1, (z+1) * 0.1, time * 0.1) * 30, zPos + size/resolution);
            ofVec3f pos4(xPos + size/resolution, ofNoise((x+1) * 0.1, (z+1) * 0.1, time * 0.1) * 30, zPos + size/resolution);
            
            // Add triangles for the mesh
            oceanFloor.addVertex(pos1);
            oceanFloor.addVertex(pos2);
            oceanFloor.addVertex(pos3);
            
            oceanFloor.addVertex(pos2);
            oceanFloor.addVertex(pos4);
            oceanFloor.addVertex(pos3);
            
            // Add colors with some variation
            ofColor floorColor = palette[8];
            floorColor.r += ofRandom(-20, 20);
            floorColor.g += ofRandom(-20, 20);
            floorColor.b += ofRandom(-20, 20);
            
            for (int i = 0; i < 6; i++) {
                oceanFloor.addColor(floorColor);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::generateCorals(){
    corals.clear();
    
    for (int i = 0; i < 30; i++) {
        CoralNode coral;
        coral.position = ofVec3f(
            ofRandom(-300, 300),
            ofRandom(150, 250),
            ofRandom(-300, 300)
        );
        coral.color = palette[int(ofRandom(0, 7))];
        coral.size = ofRandom(15, 40);
        coral.pulsePhase = ofRandom(0, TWO_PI);
        coral.breatheIntensity = ofRandom(0.3, 0.8);
        coral.gentleGlow = ofRandom(0.5, 1.0);
        
        // Generate organic branches
        int numBranches = int(ofRandom(3, 8));
        for (int j = 0; j < numBranches; j++) {
            float angle = ofMap(j, 0, numBranches, 0, TWO_PI);
            float height = ofRandom(0.5, 1.5);
            coral.branches.push_back(ofVec3f(
                cos(angle) * ofRandom(0.8, 1.2),
                height,
                sin(angle) * ofRandom(0.8, 1.2)
            ));
        }
        
        corals.push_back(coral);
    }
}

//--------------------------------------------------------------
void ofApp::updateJellyfish(){
    for (auto& jelly : jellyfish) {
        // Ultra-gentle swimming motion with breathing rhythm
        float breatheEffect = sin(time * 0.1 * jelly.breatheSpeed) * 0.5 + 0.5;
        
        // Gentle target seeking
        if (jelly.position.distance(jelly.targetPosition) < 50) {
            jelly.targetPosition = ofVec3f(
                ofRandom(-250, 250),
                ofRandom(-150, 150),
                ofRandom(-250, 250)
            );
        }
        
        ofVec3f targetForce = (jelly.targetPosition - jelly.position) * 0.001;
        
        // Gentle floating currents
        jelly.velocity.x = (sin(time * 0.3 + jelly.position.z * 0.005) * 0.15 + targetForce.x) * jelly.gentleDrift;
        jelly.velocity.y = (sin(time * 0.2 + jelly.position.x * 0.003) * 0.1 + targetForce.y) * jelly.gentleDrift;
        jelly.velocity.z = (cos(time * 0.25 + jelly.position.y * 0.004) * 0.12 + targetForce.z) * jelly.gentleDrift;
        
        // Apply breathing effect to movement
        jelly.velocity *= (0.7 + breatheEffect * 0.3);
        jelly.position += jelly.velocity;
        jelly.pulsePhase += 0.02 * jelly.breatheSpeed; // Slower pulsing
        
        // Update tentacles with ultra-fluid motion
        for (int i = 0; i < jelly.tentacles.size(); i++) {
            float angle = ofMap(i, 0, jelly.tentacles.size()-1, 0, TWO_PI);
            float tentaclePhase = jelly.pulsePhase + angle + sin(time * 0.1 + i) * 0.5;
            float gentleWave = sin(tentaclePhase) * 0.6 + 0.4;
            
            jelly.tentacles[i] = ofVec3f(
                cos(angle) * gentleWave * 0.6,
                -jelly.tentacleLength * (0.8 + sin(tentaclePhase * 0.7) * 0.2),
                sin(angle) * gentleWave * 0.6
            );
        }
        
        // Gentle boundary wrapping with larger space
        if (jelly.position.x > 350) jelly.position.x = -350;
        if (jelly.position.x < -350) jelly.position.x = 350;
        if (jelly.position.z > 350) jelly.position.z = -350;
        if (jelly.position.z < -350) jelly.position.z = 350;
        if (jelly.position.y > 180) jelly.position.y = -180;
        if (jelly.position.y < -180) jelly.position.y = 180;
    }
}

//--------------------------------------------------------------
void ofApp::updateKelp(){
    for (auto& kelpStrand : kelpFields) {
        for (int i = 0; i < kelpStrand.size(); i++) {
            auto& node = kelpStrand[i];
            node.swayPhase += 0.008 * node.gentleSway; // Much slower, gentler sway
            
            // Ultra-gentle sway with breathing effect
            float breatheEffect = sin(time * 0.05 + i * 0.3) * 0.5 + 0.5;
            float swayAmount = (node.height / 200.0) * 15 * node.gentleSway * breatheEffect;
            
            // Gentle horizontal sway
            node.position.x = node.basePosition.x + sin(node.swayPhase) * swayAmount;
            node.position.z = node.basePosition.z + cos(node.swayPhase * 0.6) * swayAmount * 0.4;
            
            // Very gentle vertical breathing
            float verticalBreathe = sin(time * 0.03 + i * 0.2) * 2 * node.breatheAmount;
            node.position.y = node.basePosition.y + verticalBreathe;
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawJellyfish(){
    for (const auto& jelly : jellyfish) {
        ofPushMatrix();
        ofTranslate(jelly.position);
        
        float pulse = sin(jelly.pulsePhase) * 0.2 + 0.8;
        
        // Draw bell with transparency
        ofSetColor(jelly.color, jelly.color.a * pulse);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        // Main bell
        ofPushMatrix();
        ofScale(pulse, pulse * 0.8, pulse);
        ofDrawSphere(0, 0, 0, jelly.bellSize);
        ofPopMatrix();
        
        // Inner glow
        ofSetColor(jelly.color, jelly.color.a * 0.3);
        ofDrawSphere(0, 0, 0, jelly.bellSize * 1.5);
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        // Draw tentacles
        ofSetColor(jelly.color, jelly.color.a * 0.8);
        ofSetLineWidth(2);
        for (const auto& tentacle : jelly.tentacles) {
            ofDrawLine(ofVec3f(0, 0, 0), tentacle);
            // Add glow to tentacles
            ofSetColor(jelly.color, jelly.color.a * 0.4);
            ofDrawSphere(tentacle.x, tentacle.y, tentacle.z, 2);
        }
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawKelp(){
    for (const auto& kelpStrand : kelpFields) {
        if (kelpStrand.size() < 2) continue;
        
        ofSetColor(palette[9], 180);
        ofSetLineWidth(3);
        
        // Draw kelp strand
        ofPolyline kelpLine;
        for (const auto& node : kelpStrand) {
            kelpLine.addVertex(node.position);
        }
        kelpLine.draw();
        
        // Add some organic bulges
        for (int i = 0; i < kelpStrand.size(); i += 3) {
            ofPushMatrix();
            ofTranslate(kelpStrand[i].position);
            ofSetColor(palette[8], 120);
            ofDrawSphere(0, 0, 0, ofRandom(3, 8));
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawGodRays(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Create gentle god rays with breathing effect
    for (int i = 0; i < 6; i++) {
        ofPushMatrix();
        
        float angle = ofMap(i, 0, 6, 0, TWO_PI);
        float breatheEffect = sin(time * 0.02 + i * 0.7) * 0.5 + 0.5;
        float rayRadius = 180 + sin(time * 0.08 + i) * 30 * breatheEffect;
        
        // Gentle swaying motion
        float gentleFloat = sin(time * 0.05 + i) * 10;
        float verticalBreathe = sin(time * 0.03 + i * 0.8) * 15;
        
        ofTranslate(
            cos(angle) * rayRadius + gentleFloat,
            -280 + verticalBreathe,
            sin(angle) * rayRadius + cos(time * 0.04 + i) * 8
        );
        
        // Gentle rotation
        ofRotateXDeg(sin(time * 0.02 + i) * 5);
        ofRotateZDeg(cos(time * 0.025 + i) * 4);
        
        // Gentle color transitions
        ofColor rayColor = palette[5].getLerped(palette[4], breatheEffect * 0.6);
        rayColor.r += sin(time * 0.08 + i) * 15;
        rayColor.g += cos(time * 0.06 + i) * 10;
        rayColor.b += sin(time * 0.05 + i) * 20;
        
        ofSetColor(rayColor, int(8 + breatheEffect * 6));
        
        // Draw gentle volumetric ray
        float coneSize = 25 + breatheEffect * 8;
        ofDrawCone(0, 350 + breatheEffect * 50, 0, coneSize, 700);
        
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawCaustics(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Gentle caustic patterns with breathing
    float breatheEffect = sin(time * 0.04) * 0.5 + 0.5;
    
    for (int x = -250; x < 250; x += 25) {
        for (int z = -250; z < 250; z += 25) {
            float causticIntensity = ofNoise(
                x * 0.015 + time * 0.1,
                z * 0.015 + time * 0.08,
                time * 0.2
            );
            
            // Gentler threshold and smoother transitions
            if (causticIntensity > 0.4) {
                ofPushMatrix();
                ofTranslate(x, 235, z);
                
                // Gentle color transitions
                ofColor causticColor = palette[4].getLerped(palette[3], breatheEffect * 0.4);
                float gentleAlpha = ofMap(causticIntensity, 0.4, 1.0, 0, 60) * (0.7 + breatheEffect * 0.3);
                causticColor.a = int(gentleAlpha);
                ofSetColor(causticColor);
                
                // Gentle ripple size with breathing
                float rippleSize = causticIntensity * 8 * (0.8 + breatheEffect * 0.4);
                ofDrawCircle(0, 0, rippleSize);
                
                // Add gentle secondary ripple
                causticColor.a = int(gentleAlpha * 0.3);
                ofSetColor(causticColor);
                ofDrawCircle(0, 0, rippleSize * 1.8);
                
                ofPopMatrix();
            }
        }
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::generateJellyfish(){
    jellyfish.clear();
    
    for (int i = 0; i < 12; i++) {
        Jellyfish jelly;
        jelly.position = ofVec3f(
            ofRandom(-300, 300),
            ofRandom(-100, 100),
            ofRandom(-300, 300)
        );
        jelly.velocity = ofVec3f(0, 0, 0);
        jelly.bellSize = ofRandom(15, 35);
        jelly.tentacleLength = ofRandom(40, 80);
        jelly.color = palette[int(ofRandom(3, 7))];
        jelly.color.a = ofRandom(150, 220);
        jelly.pulsePhase = ofRandom(0, TWO_PI);
        jelly.breatheSpeed = ofRandom(0.5, 1.2);
        jelly.gentleDrift = ofRandom(0.3, 0.8);
        jelly.targetPosition = jelly.position;
        
        // Generate tentacles
        int numTentacles = int(ofRandom(6, 12));
        jelly.tentacles.resize(numTentacles);
        
        jellyfish.push_back(jelly);
    }
}

//--------------------------------------------------------------
void ofApp::generateKelp(){
    kelpFields.clear();
    
    for (int i = 0; i < 25; i++) {
        vector<KelpNode> kelpStrand;
        
        ofVec3f basePos = ofVec3f(
            ofRandom(-400, 400),
            220, // Near ocean floor
            ofRandom(-400, 400)
        );
        
        int strandLength = int(ofRandom(15, 30));
        for (int j = 0; j < strandLength; j++) {
            KelpNode node;
            node.basePosition = basePos + ofVec3f(
                ofRandom(-10, 10),
                -j * ofRandom(8, 15),
                ofRandom(-10, 10)
            );
            node.position = node.basePosition;
            node.height = j * 10;
            node.swayPhase = ofRandom(0, TWO_PI);
            node.gentleSway = ofRandom(0.5, 1.5);
            node.breatheAmount = ofRandom(0.2, 0.8);
            
            kelpStrand.push_back(node);
        }
        
        kelpFields.push_back(kelpStrand);
    }
}

//--------------------------------------------------------------
void ofApp::createGodRays(){
    godRayMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for (int i = 0; i < 12; i++) {
        float angle = ofMap(i, 0, 12, 0, TWO_PI);
        
        // Create ray geometry
        ofVec3f top(cos(angle) * 20, -400, sin(angle) * 20);
        ofVec3f bottom1(cos(angle) * 100, 400, sin(angle) * 100);
        ofVec3f bottom2(cos(angle + 0.2) * 100, 400, sin(angle + 0.2) * 100);
        
        godRayMesh.addVertex(top);
        godRayMesh.addVertex(bottom1);
        godRayMesh.addVertex(bottom2);
        
        ofColor rayColor = palette[5];
        rayColor.a = 20;
        
        for (int j = 0; j < 3; j++) {
            godRayMesh.addColor(rayColor);
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateMagicOrbs(){
    for (auto& orb : magicOrbs) {
        orb.pulsePhase += 0.03 * orb.energy;
        
        // Gentle orbital motion around center point
        float orbitX = cos(time * orb.orbitSpeed + orb.pulsePhase) * orb.orbitRadius;
        float orbitY = sin(time * orb.orbitSpeed * 0.7 + orb.pulsePhase) * orb.orbitRadius * 0.6;
        float orbitZ = sin(time * orb.orbitSpeed * 0.5 + orb.pulsePhase) * orb.orbitRadius * 0.8;
        
        orb.position = orb.orbitCenter + ofVec3f(orbitX, orbitY, orbitZ);
        
        // Update energy trail
        orb.energyTrail.insert(orb.energyTrail.begin(), orb.position);
        if (orb.energyTrail.size() > 20) {
            orb.energyTrail.pop_back();
        }
        
        // Gentle color shifting
        float colorShift = sin(time * 0.1 + orb.pulsePhase) * 0.5 + 0.5;
        orb.color = palette[2].getLerped(palette[4], colorShift);
        orb.color.a = 180 + sin(orb.pulsePhase) * 50;
    }
}

//--------------------------------------------------------------
void ofApp::updateLightBeams(){
    // Clear and regenerate beams periodically for variety
    if (int(time * 10) % 100 == 0 && lightBeams.size() < 8) {
        LightBeam beam;
        beam.startPos = ofVec3f(ofRandom(-300, 300), ofRandom(-200, 200), ofRandom(-300, 300));
        beam.endPos = ofVec3f(ofRandom(-300, 300), ofRandom(-200, 200), ofRandom(-300, 300));
        beam.color = palette[int(ofRandom(0, 6))];
        beam.intensity = ofRandom(0.3, 0.8);
        beam.width = ofRandom(2, 6);
        beam.dancePhase = ofRandom(0, TWO_PI);
        beam.lifetime = ofRandom(200, 400);
        lightBeams.push_back(beam);
    }
    
    // Update existing beams
    for (auto it = lightBeams.begin(); it != lightBeams.end();) {
        it->dancePhase += 0.02;
        it->lifetime -= 1;
        
        // Gentle dancing motion
        it->endPos.x += sin(time * 0.1 + it->dancePhase) * 2;
        it->endPos.y += cos(time * 0.08 + it->dancePhase) * 1.5;
        it->endPos.z += sin(time * 0.12 + it->dancePhase) * 2.5;
        
        if (it->lifetime <= 0) {
            it = lightBeams.erase(it);
        } else {
            ++it;
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateCrystals(){
    for (auto& crystal : crystals) {
        crystal.rotation += crystal.rotationSpeed;
        crystal.breathePhase += 0.015;
        
        // Gentle floating motion
        crystal.position.y += sin(time * 0.1 + crystal.breathePhase) * 0.5;
        
        // Gentle size breathing
        float breathe = sin(crystal.breathePhase * 2) * 0.2 + 0.8;
        crystal.glowIntensity = breathe;
        
        // Color shifting
        float colorShift = sin(time * 0.05 + crystal.breathePhase) * 0.5 + 0.5;
        crystal.color = palette[1].getLerped(palette[3], colorShift);
        crystal.color.a = 150 + breathe * 80;
    }
}

//--------------------------------------------------------------
void ofApp::drawMagicOrbs(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (const auto& orb : magicOrbs) {
        // Draw energy trail first
        if (orb.energyTrail.size() > 1) {
            for (int i = 1; i < orb.energyTrail.size(); i++) {
                float trailAlpha = ofMap(i, 1, orb.energyTrail.size()-1, orb.color.a * 0.6, 0);
                float trailSize = ofMap(i, 1, orb.energyTrail.size()-1, orb.size * 0.8, orb.size * 0.1);
                
                ofSetColor(orb.color, trailAlpha);
                ofDrawSphere(orb.energyTrail[i], trailSize);
            }
        }
        
        // Draw main orb as wire sphere
        ofPushMatrix();
        ofTranslate(orb.position);
        
        // Main wire sphere
        ofSetColor(orb.color);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawSphere(0, 0, 0, orb.size);
        
        // Second wire sphere layer
        ofSetLineWidth(1);
        ofSetColor(orb.color, orb.color.a * 0.6);
        ofDrawSphere(0, 0, 0, orb.size * 1.3);
        
        // Bright center core (solid)
        ofFill();
        ofSetColor(255, 255, 255, orb.color.a * 0.8);
        ofDrawSphere(0, 0, 0, orb.size * 0.2);
        
        // Soft outer glow
        ofSetColor(orb.color, orb.color.a * 0.3);
        ofDrawSphere(0, 0, 0, orb.size * 2);
        
        ofPopMatrix();
        
        // Draw connections to nearby orbs
        for (const auto& other : magicOrbs) {
            if (&orb != &other) {
                float distance = orb.position.distance(other.position);
                if (distance < orb.connectionDistance) {
                    float connectionAlpha = ofMap(distance, 0, orb.connectionDistance, 80, 0);
                    ofSetColor(orb.color, connectionAlpha);
                    ofSetLineWidth(2);
                    ofDrawLine(orb.position, other.position);
                }
            }
        }
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawLightBeams(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (const auto& beam : lightBeams) {
        ofSetColor(beam.color, beam.intensity * 255);
        ofSetLineWidth(beam.width);
        
        // Draw main beam
        ofDrawLine(beam.startPos, beam.endPos);
        
        // Add glow effect
        ofSetColor(beam.color, beam.intensity * 100);
        ofSetLineWidth(beam.width * 2);
        ofDrawLine(beam.startPos, beam.endPos);
        
        // Add sparkles along the beam
        int sparkleCount = 5;
        for (int i = 0; i < sparkleCount; i++) {
            float t = float(i) / sparkleCount;
            ofVec3f sparklePos = beam.startPos + (beam.endPos - beam.startPos) * t;
            ofSetColor(255, 255, 255, beam.intensity * 150);
            ofDrawSphere(sparklePos, 2);
        }
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawCrystals(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    for (const auto& crystal : crystals) {
        ofPushMatrix();
        ofTranslate(crystal.position);
        ofRotateXDeg(crystal.rotation.x);
        ofRotateYDeg(crystal.rotation.y);
        ofRotateZDeg(crystal.rotation.z);
        
        // Draw crystal with multiple layers
        ofSetColor(crystal.color, crystal.color.a * crystal.glowIntensity);
        
        // Create geometric crystal shape
        if (crystal.faces == 6) {
            // Hexagonal crystal
            ofDrawBox(0, 0, 0, crystal.size, crystal.size * 1.5, crystal.size);
        } else {
            // Octahedral crystal
            ofDrawSphere(0, 0, 0, crystal.size);
        }
        
        // Add inner glow
        ofSetColor(crystal.color, crystal.color.a * 0.3);
        ofDrawSphere(0, 0, 0, crystal.size * 1.5);
        
        // Add bright core
        ofSetColor(255, 255, 255, crystal.color.a * 0.8 * crystal.glowIntensity);
        ofDrawSphere(0, 0, 0, crystal.size * 0.2);
        
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawAurora(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Create flowing aurora effect
    for (int i = 0; i < 3; i++) {
        ofPushMatrix();
        
        float auroraPhase = time * 0.02 + i * TWO_PI / 3;
        float flow = sin(auroraPhase) * 200;
        float height = cos(auroraPhase * 0.7) * 100;
        
        ofTranslate(flow, -150 + height, sin(auroraPhase * 0.5) * 150);
        ofRotateYDeg(sin(auroraPhase) * 30);
        
        ofColor auroraColor = palette[i + 3].getLerped(palette[i + 4], sin(auroraPhase) * 0.5 + 0.5);
        ofSetColor(auroraColor, 20 + sin(auroraPhase * 2) * 10);
        
        // Draw flowing curtain
        ofMesh auroraMesh;
        auroraMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        for (int j = 0; j < 20; j++) {
            float x = ofMap(j, 0, 19, -300, 300);
            float wave1 = sin(x * 0.01 + auroraPhase) * 80;
            float wave2 = sin(x * 0.015 + auroraPhase * 1.3) * 60;
            
            auroraMesh.addVertex(ofVec3f(x, wave1, 0));
            auroraMesh.addVertex(ofVec3f(x, wave2, 200));
            auroraMesh.addColor(auroraColor);
            auroraMesh.addColor(ofColor(auroraColor, 0));
        }
        
        auroraMesh.draw();
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawMysticalRings(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Draw mystical rings around corals and jellyfish
    for (const auto& coral : corals) {
        float ringPhase = time * 0.1 + coral.pulsePhase;
        float ringSize = coral.size * (2 + sin(ringPhase) * 0.5);
        
        ofPushMatrix();
        ofTranslate(coral.position);
        ofRotateXDeg(90);
        
        ofSetColor(coral.color, 30 + sin(ringPhase * 2) * 20);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawCircle(0, 0, ringSize);
        
        ofSetLineWidth(1);
        ofDrawCircle(0, 0, ringSize * 1.5);
        
        ofPopMatrix();
    }
    
    // Rings around jellyfish
    for (const auto& jelly : jellyfish) {
        float ringPhase = time * 0.15 + jelly.pulsePhase;
        float ringSize = jelly.bellSize * (2.5 + sin(ringPhase) * 0.5);  // Made gentler like coral rings
        
        ofPushMatrix();
        ofTranslate(jelly.position);
        ofRotateXDeg(sin(ringPhase) * 15);  // Reduced rotation for gentler movement
        ofRotateZDeg(cos(ringPhase * 0.7) * 10);  // Reduced rotation
        
        ofSetColor(jelly.color, 25 + sin(ringPhase * 3) * 15);
        ofNoFill();
        ofSetLineWidth(1.5);
        ofDrawCircle(0, 0, ringSize);
        
        // Add a second ring like the corals
        ofSetLineWidth(1);
        ofSetColor(jelly.color, 15 + sin(ringPhase * 2) * 10);
        ofDrawCircle(0, 0, ringSize * 1.4);
        
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::generateMagicOrbs(){
    magicOrbs.clear();
    
    for (int i = 0; i < 8; i++) {
        MagicOrb orb;
        orb.orbitCenter = ofVec3f(
            ofRandom(-200, 200),
            ofRandom(-100, 100),
            ofRandom(-200, 200)
        );
        orb.position = orb.orbitCenter;
        orb.color = palette[int(ofRandom(2, 6))];
        orb.color.a = 180;
        orb.size = ofRandom(8, 18);
        orb.energy = ofRandom(0.5, 1.5);
        orb.pulsePhase = ofRandom(0, TWO_PI);
        orb.orbitRadius = ofRandom(50, 120);
        orb.orbitSpeed = ofRandom(0.1, 0.3);
        orb.connectionDistance = ofRandom(100, 200);
        
        magicOrbs.push_back(orb);
    }
}

//--------------------------------------------------------------
void ofApp::generateCrystals(){
    crystals.clear();
    
    for (int i = 0; i < 12; i++) {
        Crystal crystal;
        crystal.position = ofVec3f(
            ofRandom(-350, 350),
            ofRandom(-50, 200),
            ofRandom(-350, 350)
        );
        crystal.rotation = ofVec3f(ofRandom(0, 360), ofRandom(0, 360), ofRandom(0, 360));
        crystal.rotationSpeed = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.3, 0.3), ofRandom(-0.4, 0.4));
        crystal.size = ofRandom(15, 35);
        crystal.color = palette[int(ofRandom(1, 5))];
        crystal.color.a = 150;
        crystal.glowIntensity = 1.0;
        crystal.breathePhase = ofRandom(0, TWO_PI);
        crystal.faces = (ofRandom(0, 1) > 0.5) ? 6 : 8;
        
        crystals.push_back(crystal);
    }
}
