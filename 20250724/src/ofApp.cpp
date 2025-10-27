#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    
    time = 0;
    numParticles = 108;
    numOrbs = 21;
    numDustParticles = 300;
    noiseScale = 0.01f;
    rotationSpeed = 0.3f;
    showWireframe = false;
    showTrails = true;
    showWaves = false;
    showChakras = true;
    showSacredGeometry = true;
    showMandala = true;
    showEnergyStreams = true;
    showAurora = true;
    showOrbs = true;
    meditationMode = false;
    currentPalette = 0;
    cameraAutoRotate = 0;
    attractorStrength = 50.0f;
    centerAttractor = ofVec3f(0, 0, 0);
    breathingCycle = 0;
    morphCycle = 0;
    auroraCycle = 0;
    goldenRatio = 1.618033988749f;
    
    particles.resize(numParticles);
    particleColors.resize(numParticles);
    particleRotations.resize(numParticles);
    particleScales.resize(numParticles);
    particleTrails.resize(numParticles);
    attractorForces.resize(numParticles);
    particleMorphStates.resize(numParticles);
    particleLifespan.resize(numParticles);
    
    lightOrbs.resize(numOrbs);
    orbColors.resize(numOrbs);
    orbSizes.resize(numOrbs);
    cosmicDust.resize(numDustParticles);
    
    chakraPositions = {
        ofVec3f(0, -200, 0),   // Root - Red
        ofVec3f(0, -133, 0),   // Sacral - Orange
        ofVec3f(0, -67, 0),    // Solar Plexus - Yellow
        ofVec3f(0, 0, 0),      // Heart - Green
        ofVec3f(0, 67, 0),     // Throat - Blue
        ofVec3f(0, 133, 0),    // Third Eye - Indigo
        ofVec3f(0, 200, 0)     // Crown - Violet
    };
    
    chakraColors = {
        ofColor(255, 0, 0),    // Root
        ofColor(255, 127, 0),  // Sacral
        ofColor(255, 255, 0),  // Solar Plexus
        ofColor(0, 255, 0),    // Heart
        ofColor(0, 127, 255),  // Throat
        ofColor(75, 0, 130),   // Third Eye
        ofColor(148, 0, 211)   // Crown
    };
    
    for(int i = 0; i < 19; i++) {
        float angle = TWO_PI * i / 19.0f;
        float radius = 150;
        flowerOfLifePoints.push_back(ofVec3f(
            cos(angle) * radius,
            sin(angle) * radius,
            0
        ));
        
        for(int j = 1; j <= 6; j++) {
            float innerAngle = angle + TWO_PI * j / 6.0f;
            float innerRadius = radius / goldenRatio;
            flowerOfLifePoints.push_back(ofVec3f(
                cos(angle) * radius + cos(innerAngle) * innerRadius,
                sin(angle) * radius + sin(innerAngle) * innerRadius,
                sin(i * 0.3f) * 50
            ));
        }
    }
    
    float merkSize = 100;
    merkabaPyramid = {
        ofVec3f(0, merkSize, 0),
        ofVec3f(-merkSize, -merkSize/2, merkSize),
        ofVec3f(merkSize, -merkSize/2, merkSize),
        ofVec3f(0, -merkSize/2, -merkSize),
        ofVec3f(0, -merkSize, 0),
        ofVec3f(-merkSize, merkSize/2, -merkSize),
        ofVec3f(merkSize, merkSize/2, -merkSize),
        ofVec3f(0, merkSize/2, merkSize)
    };
    
    for(int layer = 0; layer < 7; layer++) {
        int pointsInLayer = 8 + layer * 4;
        float radius = 80 + layer * 40;
        float heightOffset = (layer - 3) * 60;
        
        for(int i = 0; i < pointsInLayer; i++) {
            float angle = TWO_PI * i / pointsInLayer + layer * 0.2f;
            mandalaPoints.push_back(ofVec3f(
                cos(angle) * radius,
                sin(angle) * radius,
                heightOffset
            ));
        }
    }
    
    for(int i = 0; i < 200; i++) {
        float auroraAngle = i * 0.05f;
        float auroraRadius = 400 + sin(i * 0.1f) * 100;
        auroraPoints.push_back(ofVec3f(
            cos(auroraAngle) * auroraRadius,
            sin(auroraAngle * 2) * 200,
            sin(auroraAngle) * auroraRadius
        ));
    }
    
    for(int i = 0; i < 100; i++) {
        energyStreams.push_back(ofVec3f(
            ofRandom(-500, 500),
            ofRandom(-400, 400),
            ofRandom(-500, 500)
        ));
    }
    
    for(int i = 0; i < numOrbs; i++) {
        lightOrbs[i] = ofVec3f(
            ofRandom(-400, 400),
            ofRandom(-300, 300),
            ofRandom(-400, 400)
        );
        orbColors[i] = ofColor::fromHsb(ofRandom(0, 255), 150, 255);
        orbSizes[i] = ofRandom(20, 60);
    }
    
    for(int i = 0; i < numDustParticles; i++) {
        cosmicDust[i] = ofVec3f(
            ofRandom(-800, 800),
            ofRandom(-600, 600),
            ofRandom(-800, 800)
        );
    }
    
    for(int i = 0; i < numParticles; i++) {
        float spiralAngle = i * goldenRatio * 0.1f;
        float spiralRadius = sqrt(i) * 15;
        
        particles[i] = ofVec3f(
            cos(spiralAngle) * spiralRadius,
            sin(spiralAngle) * spiralRadius,
            (i - numParticles/2) * 8
        );
        
        int chakraIndex = i % 7;
        particleColors[i] = chakraColors[chakraIndex];
        
        particleRotations[i] = spiralAngle;
        particleScales[i] = 0.5f + (i % 21) * 0.05f;
        attractorForces[i] = 0.3f + (i % 13) * 0.1f;
        particleMorphStates[i] = ofRandom(0, TWO_PI);
        particleLifespan[i] = ofRandom(0.5f, 1.0f);
        
        particleTrails[i].resize(21);
        for(int j = 0; j < 21; j++) {
            particleTrails[i][j] = particles[i];
        }
    }
    
    cam.setDistance(600);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.008f;
    breathingCycle += 0.02f;
    morphCycle += 0.03f;
    auroraCycle += 0.015f;
    cameraAutoRotate += 0.1f;
    
    float breathingPulse = (sin(breathingCycle) + 1.0f) * 0.5f;
    float heartbeat = sin(time * 8) * 0.1f + 1.0f;
    float morphIntensity = (sin(morphCycle) + 1.0f) * 0.5f;
    
    if(meditationMode) {
        cameraAutoRotate += 0.05f;
        cam.setPosition(
            sin(cameraAutoRotate * 0.01f) * 400,
            cos(cameraAutoRotate * 0.007f) * 200,
            400 + sin(cameraAutoRotate * 0.005f) * 200
        );
        cam.lookAt(ofVec3f(0, 0, 0));
    }
    
    centerAttractor.x = sin(time * 0.3f) * breathingPulse * 100;
    centerAttractor.y = cos(time * 0.2f) * breathingPulse * 100;
    centerAttractor.z = sin(time * 0.4f) * breathingPulse * 80;
    
    for(int i = 0; i < chakraPositions.size(); i++) {
        float chakraEnergy = sin(time * (1 + i * 0.3f)) * 30;
        chakraPositions[i].x = sin(time + i) * chakraEnergy;
        chakraPositions[i].z = cos(time + i) * chakraEnergy;
    }
    
    for(int i = 0; i < flowerOfLifePoints.size(); i++) {
        float originalRadius = 150;
        float angle = atan2(flowerOfLifePoints[i].y, flowerOfLifePoints[i].x);
        float radius = originalRadius + sin(time * 2 + i * 0.1f) * 20;
        
        flowerOfLifePoints[i].x = cos(angle + time * 0.1f) * radius;
        flowerOfLifePoints[i].y = sin(angle + time * 0.1f) * radius;
        flowerOfLifePoints[i].z = sin(time + i * 0.2f) * 30;
    }
    
    for(int i = 0; i < merkabaPyramid.size(); i++) {
        ofVec3f original = merkabaPyramid[i];
        merkabaPyramid[i].rotateRad(time * 0.01f, ofVec3f(0, 1, 0));
        merkabaPyramid[i].rotateRad(time * 0.007f, ofVec3f(1, 0, 0));
        merkabaPyramid[i] *= (1.0f + breathingPulse * 0.3f);
    }
    
    for(int i = 0; i < numParticles; i++) {
        float spiralAngle = particleRotations[i] + time * goldenRatio * 0.1f;
        float spiralRadius = sqrt(i) * (12 + breathingPulse * 8);
        
        ofVec3f targetPos = ofVec3f(
            cos(spiralAngle) * spiralRadius,
            sin(spiralAngle) * spiralRadius,
            (i - numParticles/2) * (6 + sin(time + i * 0.1f) * 4)
        );
        
        int nearestChakra = i % 7;
        ofVec3f chakraAttraction = chakraPositions[nearestChakra] - particles[i];
        chakraAttraction *= 0.02f * breathingPulse;
        
        float noiseX = ofNoise(particles[i].x * noiseScale, time + i);
        float noiseY = ofNoise(particles[i].y * noiseScale, time + i + 100);
        float noiseZ = ofNoise(particles[i].z * noiseScale, time + i + 200);
        
        ofVec3f noiseForce = ofVec3f(
            (noiseX - 0.5f) * 0.5f,
            (noiseY - 0.5f) * 0.5f,
            (noiseZ - 0.5f) * 0.5f
        );
        
        particles[i] = particles[i] * 0.98f + targetPos * 0.02f + chakraAttraction + noiseForce;
        
        for(int j = particleTrails[i].size() - 1; j > 0; j--) {
            particleTrails[i][j] = particleTrails[i][j - 1];
        }
        particleTrails[i][0] = particles[i];
        
        particleRotations[i] = spiralAngle;
        
        int chakraIndex = i % 7;
        ofColor baseChakraColor = chakraColors[chakraIndex];
        
        float energyIntensity = breathingPulse * heartbeat;
        float hueShift = sin(time * 0.5f + i * 0.1f) * 20;
        
        particleColors[i] = ofColor::fromHsb(
            fmod(baseChakraColor.getHue() + hueShift, 255),
            baseChakraColor.getSaturation() * (0.7f + energyIntensity * 0.3f),
            baseChakraColor.getBrightness() * (0.8f + energyIntensity * 0.2f)
        );
        
        particleScales[i] = 0.4f + sin(time * 2.0f + i * 0.2f) * 0.3f * breathingPulse + 0.3f;
        
        particleMorphStates[i] += 0.02f;
        particleLifespan[i] = 0.7f + sin(time + i * 0.15f) * 0.3f;
    }
    
    for(int i = 0; i < auroraPoints.size(); i++) {
        float auroraAngle = i * 0.05f + auroraCycle;
        float auroraRadius = 400 + sin(i * 0.1f + time * 2) * 150;
        auroraPoints[i] = ofVec3f(
            cos(auroraAngle) * auroraRadius,
            sin(auroraAngle * 2 + auroraCycle) * 250,
            sin(auroraAngle + auroraCycle * 0.7f) * auroraRadius
        );
    }
    
    for(int i = 0; i < energyStreams.size(); i++) {
        float streamFlow = time * 3.0f + i * 0.1f;
        energyStreams[i].x += sin(streamFlow) * 2.0f;
        energyStreams[i].y += cos(streamFlow * 1.3f) * 1.5f;
        energyStreams[i].z += sin(streamFlow * 0.8f) * 2.5f;
        
        if(energyStreams[i].length() > 600) {
            energyStreams[i] *= 0.8f;
        }
    }
    
    for(int i = 0; i < numOrbs; i++) {
        float orbDance = time * 1.5f + i * 0.3f;
        lightOrbs[i] += ofVec3f(
            sin(orbDance) * 0.8f,
            cos(orbDance * 1.2f) * 0.6f,
            sin(orbDance * 0.7f) * 0.9f
        );
        
        if(lightOrbs[i].length() > 500) {
            lightOrbs[i] *= 0.9f;
        }
        
        orbSizes[i] = 30 + sin(time * 4 + i) * 20 * breathingPulse;
        
        float hueShift = sin(time * 0.5f + i * 0.2f) * 30;
        orbColors[i] = ofColor::fromHsb(
            fmod(orbColors[i].getHue() + hueShift, 255),
            150 + breathingPulse * 50,
            200 + morphIntensity * 55
        );
    }
    
    for(int i = 0; i < numDustParticles; i++) {
        float drift = time * 0.5f + i * 0.01f;
        cosmicDust[i] += ofVec3f(
            sin(drift) * 0.3f,
            cos(drift * 1.1f) * 0.2f,
            sin(drift * 0.9f) * 0.4f
        );
        
        if(cosmicDust[i].length() > 1000) {
            cosmicDust[i] = ofVec3f(
                ofRandom(-800, 800),
                ofRandom(-600, 600),
                ofRandom(-800, 800)
            );
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float breathingPulse = (sin(breathingCycle) + 1.0f) * 0.5f;
    float morphIntensity = (sin(morphCycle) + 1.0f) * 0.5f;
    float auroraBrightness = (sin(auroraCycle) + 1.0f) * 0.5f;
    
    ofColor bgTop = ofColor(
        5 + auroraBrightness * 20,
        2 + morphIntensity * 15,
        15 + breathingPulse * 25,
        255
    );
    ofColor bgBottom = ofColor(
        auroraBrightness * 10,
        morphIntensity * 8,
        5 + breathingPulse * 10,
        255
    );
    ofBackgroundGradient(bgTop, bgBottom);
    
    cam.begin();
    
    ofEnableLighting();
    ofLight etherealLight1, etherealLight2, etherealLight3;
    etherealLight1.setPosition(sin(time * 0.3f) * 400, 300, cos(time * 0.3f) * 400);
    etherealLight2.setPosition(-300, cos(time * 0.2f) * 300, 200);
    etherealLight3.setPosition(200, -200, sin(time * 0.4f) * 300);
    
    etherealLight1.setDiffuseColor(ofColor(255, 200, 255, 150));
    etherealLight2.setDiffuseColor(ofColor(200, 255, 255, 150));
    etherealLight3.setDiffuseColor(ofColor(255, 255, 200, 150));
    
    etherealLight1.enable();
    etherealLight2.enable();
    etherealLight3.enable();
    
    if(showAurora) {
        ofSetLineWidth(3);
        for(int i = 0; i < auroraPoints.size() - 1; i++) {
            float auroraAlpha = 30 + auroraBrightness * 60;
            float hue = fmod(time * 30 + i * 5, 255);
            ofColor auroraColor = ofColor::fromHsb(hue, 180, 255, auroraAlpha);
            ofSetColor(auroraColor);
            
            ofDrawLine(auroraPoints[i], auroraPoints[i + 1]);
            
            if(i % 20 == 0) {
                ofPushMatrix();
                ofTranslate(auroraPoints[i]);
                ofSetColor(auroraColor.r, auroraColor.g, auroraColor.b, auroraAlpha * 2);
                ofDrawSphere(0, 0, 0, 15 + auroraBrightness * 10);
                ofPopMatrix();
            }
        }
    }
    
    for(int i = 0; i < numDustParticles; i++) {
        float dustAlpha = 20 + sin(time + i * 0.1f) * 15;
        ofSetColor(255, 255, 255, dustAlpha);
        ofPushMatrix();
        ofTranslate(cosmicDust[i]);
        ofDrawSphere(0, 0, 0, 1 + sin(time * 2 + i) * 0.5f);
        ofPopMatrix();
    }
    
    if(showOrbs) {
        for(int i = 0; i < numOrbs; i++) {
            ofPushMatrix();
            ofTranslate(lightOrbs[i]);
            
            ofColor orbGlow = orbColors[i];
            
            for(int layer = 5; layer >= 0; layer--) {
                float layerAlpha = ofMap(layer, 0, 5, 200, 30);
                float layerSize = orbSizes[i] * (1.0f + layer * 0.4f);
                
                orbGlow.a = layerAlpha * breathingPulse;
                ofSetColor(orbGlow);
                ofDrawSphere(0, 0, 0, layerSize);
            }
            
            ofPopMatrix();
        }
    }
    
    if(showEnergyStreams) {
        ofSetLineWidth(2);
        for(int i = 0; i < chakraPositions.size() - 1; i++) {
            ofColor streamColor = chakraColors[i];
            streamColor.a = 100 + breathingPulse * 80;
            ofSetColor(streamColor);
            
            ofVec3f start = chakraPositions[i];
            ofVec3f end = chakraPositions[i + 1];
            
            for(float t = 0; t <= 1.0f; t += 0.1f) {
                ofVec3f point = start.interpolate(end, t);
                point.x += sin(time * 3 + t * 10) * 20;
                point.z += cos(time * 2 + t * 8) * 15;
                
                ofPushMatrix();
                ofTranslate(point);
                ofSetColor(streamColor.r, streamColor.g, streamColor.b, streamColor.a * (1.0f - t));
                ofDrawSphere(0, 0, 0, 5 + sin(time * 4 + t * 15) * 3);
                ofPopMatrix();
            }
        }
        
        for(int i = 0; i < energyStreams.size(); i++) {
            float streamAlpha = 60 + sin(time * 2 + i * 0.3f) * 40;
            ofColor energyColor = ofColor::fromHsb(
                fmod(time * 20 + i * 10, 255),
                180,
                255,
                streamAlpha
            );
            ofSetColor(energyColor);
            
            ofPushMatrix();
            ofTranslate(energyStreams[i]);
            ofDrawSphere(0, 0, 0, 8 + sin(time * 5 + i) * 4);
            ofPopMatrix();
        }
    }
    
    if(showChakras) {
        for(int i = 0; i < chakraPositions.size(); i++) {
            ofPushMatrix();
            ofTranslate(chakraPositions[i]);
            
            ofColor chakraGlow = chakraColors[i];
            chakraGlow.a = 60 + breathingPulse * 60;
            
            for(int ring = 4; ring >= 0; ring--) {
                float ringAlpha = ofMap(ring, 0, 4, 180, 30);
                float ringSize = 40 + ring * 15 + breathingPulse * 20;
                
                chakraGlow.a = ringAlpha;
                ofSetColor(chakraGlow);
                ofDrawSphere(0, 0, 0, ringSize);
            }
            
            ofPopMatrix();
        }
    }
    
    if(showSacredGeometry) {
        ofSetColor(255, 255, 255, 40 + breathingPulse * 40);
        ofSetLineWidth(2);
        
        for(int i = 0; i < flowerOfLifePoints.size() - 1; i++) {
            ofDrawLine(flowerOfLifePoints[i], flowerOfLifePoints[(i + 1) % flowerOfLifePoints.size()]);
            
            if(i % 7 == 0) {
                ofPushMatrix();
                ofTranslate(flowerOfLifePoints[i]);
                ofSetColor(255, 255, 255, 80 + breathingPulse * 80);
                ofDrawSphere(0, 0, 0, 8);
                ofPopMatrix();
            }
        }
        
        ofSetColor(255, 215, 0, 60 + breathingPulse * 60);
        ofSetLineWidth(3);
        for(int i = 0; i < merkabaPyramid.size(); i++) {
            for(int j = i + 1; j < merkabaPyramid.size(); j++) {
                ofDrawLine(merkabaPyramid[i], merkabaPyramid[j]);
            }
        }
    }
    
    if(showMandala) {
        ofSetColor(255, 180, 255, 50 + breathingPulse * 50);
        ofSetLineWidth(1);
        
        for(int i = 0; i < mandalaPoints.size(); i++) {
            ofVec3f point = mandalaPoints[i];
            point.rotate(time * 10, ofVec3f(0, 0, 1));
            
            ofPushMatrix();
            ofTranslate(point);
            ofDrawSphere(0, 0, 0, 4 + breathingPulse * 3);
            ofPopMatrix();
            
            if(i < mandalaPoints.size() - 1) {
                ofVec3f nextPoint = mandalaPoints[i + 1];
                nextPoint.rotate(time * 10, ofVec3f(0, 0, 1));
                ofDrawLine(point, nextPoint);
            }
        }
    }
    
    ofPushMatrix();
    ofTranslate(centerAttractor);
    ofSetColor(255, 255, 255, 20 + breathingPulse * 40);
    ofDrawSphere(0, 0, 0, 30 + breathingPulse * 20);
    ofPopMatrix();
    
    if(!showWireframe) {
        ofFill();
    } else {
        ofNoFill();
    }
    
    for(int i = 0; i < numParticles; i++) {
        if(showTrails) {
            for(int j = 1; j < particleTrails[i].size(); j++) {
                float alpha = ofMap(j, 0, particleTrails[i].size(), 150, 10);
                ofColor trailColor = particleColors[i];
                trailColor.a = alpha * breathingPulse;
                ofSetColor(trailColor);
                
                float trailWidth = ofMap(j, 0, particleTrails[i].size(), 4, 0.5f);
                ofSetLineWidth(trailWidth);
                ofDrawLine(particleTrails[i][j-1], particleTrails[i][j]);
            }
        }
        
        ofPushMatrix();
        ofTranslate(particles[i]);
        ofRotateXDeg(particleRotations[i] * 57.2958f);
        ofRotateYDeg(particleRotations[i] * 73.2958f);
        ofRotateZDeg(particleRotations[i] * 43.2958f);
        
        ofColor glowColor = particleColors[i];
        
        for(int glow = 3; glow >= 0; glow--) {
            float glowAlpha = ofMap(glow, 0, 3, 200, 40);
            float glowScale = particleScales[i] * (1.0f + glow * 0.4f) * breathingPulse;
            
            glowColor.a = glowAlpha;
            ofSetColor(glowColor);
            
            ofPushMatrix();
            ofScale(glowScale, glowScale, glowScale);
            
            float morphState = particleMorphStates[i];
            float lifespan = particleLifespan[i];
            int primaryShape = i % 7;
            int secondaryShape = (i + 1) % 7;
            float morphBlend = (sin(morphState) + 1.0f) * 0.5f;
            
            ofPushMatrix();
            ofScale(lifespan, lifespan, lifespan);
            
            if(morphBlend < 0.5f) {
                float alpha = ofMap(morphBlend, 0, 0.5f, 1.0f, 0.0f);
                glowColor.a *= alpha;
                ofSetColor(glowColor);
                
                switch(primaryShape) {
                    case 0: ofDrawSphere(0, 0, 0, 10); break;
                    case 1: ofDrawBox(0, 0, 0, 14); break;
                    case 2: ofDrawCone(0, 0, 0, 7, 16); break;
                    case 3: ofDrawCylinder(0, 0, 0, 5, 14); break;
                    case 4: ofDrawIcoSphere(0, 0, 0, 8); break;
                    case 5: 
                        ofRotateYDeg(morphState * 10);
                        ofDrawBox(0, 0, 0, 12); 
                        break;
                    case 6: 
                        ofRotateXDeg(morphState * 15);
                        ofDrawCone(0, 0, 0, 6, 18); 
                        break;
                }
            } else {
                float alpha = ofMap(morphBlend, 0.5f, 1.0f, 0.0f, 1.0f);
                glowColor.a *= alpha;
                ofSetColor(glowColor);
                
                switch(secondaryShape) {
                    case 0: ofDrawSphere(0, 0, 0, 10); break;
                    case 1: ofDrawBox(0, 0, 0, 14); break;
                    case 2: ofDrawCone(0, 0, 0, 7, 16); break;
                    case 3: ofDrawCylinder(0, 0, 0, 5, 14); break;
                    case 4: ofDrawIcoSphere(0, 0, 0, 8); break;
                    case 5: 
                        ofRotateYDeg(morphState * 10);
                        ofDrawBox(0, 0, 0, 12); 
                        break;
                    case 6: 
                        ofRotateXDeg(morphState * 15);
                        ofDrawCone(0, 0, 0, 6, 18); 
                        break;
                }
            }
            
            ofPopMatrix();
            
            ofPopMatrix();
        }
        
        ofPopMatrix();
        
        if(i % 7 == 0 && i < numParticles - 7) {
            ofColor energyLine = particleColors[i];
            energyLine.a = 60 * breathingPulse;
            ofSetColor(energyLine);
            ofSetLineWidth(1.5f);
            ofDrawLine(particles[i], particles[i + 7]);
        }
    }
    
    ofSetLineWidth(1);
    etherealLight1.disable();
    etherealLight2.disable();
    etherealLight3.disable();
    ofDisableLighting();
    
    cam.end();
    
    ofSetColor(255, 255, 255, 180);
//    ofDrawBitmapString("✨ Beautiful Spiritual Meditation Experience ✨", 20, 20);
//    ofDrawBitmapString("Particles: " + ofToString(numParticles) + " | Orbs: " + ofToString(numOrbs) + " | Cosmic Dust: " + ofToString(numDustParticles), 20, 40);
//    ofDrawBitmapString("Breathing: " + ofToString(breathingPulse, 2) + " | Morph: " + ofToString(morphIntensity, 2) + " | Aurora: " + ofToString(auroraBrightness, 2), 20, 60);
//    ofDrawBitmapString("Sacred: c(chakras) s(geometry) m(mandala) e(energy streams) space(meditation)", 20, 80);
//    ofDrawBitmapString("Visual: a(aurora) o(orbs) t(trails) w(wireframe) f(fullscreen) r(reset)", 20, 100);
//    ofDrawBitmapString("Enhance: 1(breath reset) 2(energy) 3(rotation speed)", 20, 120);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'w':
        case 'W':
            showWireframe = !showWireframe;
            break;
        case 't':
        case 'T':
            showTrails = !showTrails;
            break;
        case 'c':
        case 'C':
            showChakras = !showChakras;
            break;
        case 's':
        case 'S':
            showSacredGeometry = !showSacredGeometry;
            break;
        case 'm':
        case 'M':
            showMandala = !showMandala;
            break;
        case 'e':
        case 'E':
            showEnergyStreams = !showEnergyStreams;
            break;
        case 'a':
        case 'A':
            showAurora = !showAurora;
            break;
        case 'o':
        case 'O':
            showOrbs = !showOrbs;
            break;
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
        case 'r':
        case 'R':
            time = 0;
            breathingCycle = 0;
            for(int i = 0; i < numParticles; i++) {
                float spiralAngle = i * goldenRatio * 0.1f;
                float spiralRadius = sqrt(i) * 15;
                
                particles[i] = ofVec3f(
                    cos(spiralAngle) * spiralRadius,
                    sin(spiralAngle) * spiralRadius,
                    (i - numParticles/2) * 8
                );
                
                for(int j = 0; j < particleTrails[i].size(); j++) {
                    particleTrails[i][j] = particles[i];
                }
            }
            break;
        case '+':
        case '=':
            if(numParticles < 216) {
                int oldSize = numParticles;
                numParticles += 9;
                particles.resize(numParticles);
                particleColors.resize(numParticles);
                particleRotations.resize(numParticles);
                particleScales.resize(numParticles);
                particleTrails.resize(numParticles);
                attractorForces.resize(numParticles);
                
                for(int i = oldSize; i < numParticles; i++) {
                    float spiralAngle = i * goldenRatio * 0.1f;
                    float spiralRadius = sqrt(i) * 15;
                    
                    particles[i] = ofVec3f(
                        cos(spiralAngle) * spiralRadius,
                        sin(spiralAngle) * spiralRadius,
                        (i - numParticles/2) * 8
                    );
                    
                    int chakraIndex = i % 7;
                    particleColors[i] = chakraColors[chakraIndex];
                    
                    particleRotations[i] = spiralAngle;
                    particleScales[i] = 0.5f + (i % 21) * 0.05f;
                    attractorForces[i] = 0.3f + (i % 13) * 0.1f;
                    
                    particleTrails[i].resize(21);
                    for(int j = 0; j < 21; j++) {
                        particleTrails[i][j] = particles[i];
                    }
                }
            }
            break;
        case '-':
        case '_':
            if(numParticles > 27) {
                numParticles -= 9;
                particles.resize(numParticles);
                particleColors.resize(numParticles);
                particleRotations.resize(numParticles);
                particleScales.resize(numParticles);
                particleTrails.resize(numParticles);
                attractorForces.resize(numParticles);
            }
            break;
        case ' ':
            meditationMode = !meditationMode;
            if(!meditationMode) {
                cam.setDistance(600);
            }
            break;
        case '1':
            breathingCycle = 0;
            break;
        case '2':
            attractorStrength = (attractorStrength > 30) ? 10.0f : 80.0f;
            break;
        case '3':
            rotationSpeed = (rotationSpeed > 0.2f) ? 0.1f : 0.5f;
            break;
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
