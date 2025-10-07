#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(720, 1080);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofEnableSmoothing();

    // Initialize Pandora-inspired color palette
    palette.push_back(ofColor::fromHex(0x362d78));
    palette.push_back(ofColor::fromHex(0x523fa3));
    palette.push_back(ofColor::fromHex(0x916ccc));
    palette.push_back(ofColor::fromHex(0xbda1e5));
    palette.push_back(ofColor::fromHex(0xc8c0e9));
    palette.push_back(ofColor::fromHex(0x84bae7));
    palette.push_back(ofColor::fromHex(0x516ad4));
    palette.push_back(ofColor::fromHex(0x333f87));
    palette.push_back(ofColor::fromHex(0x293039));
    palette.push_back(ofColor::fromHex(0x283631));

    // Camera setup
    cam.setDistance(400);
    cameraRotation = 0;

    // Camera animation setup
    currentCameraMode = ORBIT;
    cameraModeTime = 0;
    cameraModeDuration = 12.0; // Each mode lasts 12 seconds
    cameraTarget = ofVec3f(0, 0, 0);
    focusedElementIndex = 0;

    // Setup lighting
    ofEnableLighting();
    mainLight.setPointLight();
    mainLight.setDiffuseColor(ofColor::fromHex(0x84bae7));
    mainLight.setAmbientColor(ofColor::fromHex(0x362d78));

    rimLight.setPointLight();
    rimLight.setDiffuseColor(ofColor::fromHex(0x916ccc));
    rimLight.setSpecularColor(ofColor::fromHex(0xbda1e5));

    // Initialize particles
    maxParticles = 800;
    particleTrails.resize(maxParticles);

    // Create floating islands
    for(int i = 0; i < 12; i++) {
        float angle = (TWO_PI / 12.0) * i;
        float radius = ofRandom(150, 320);
        ofVec3f pos(
            cos(angle) * radius,
            ofRandom(-200, 200),
            sin(angle) * radius
        );
        createIsland(pos);
    }

    // Create tendrils
    for(int i = 0; i < 25; i++) {
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(100, 280);
        ofVec3f start(
            cos(angle) * radius,
            ofRandom(-280, 280),
            sin(angle) * radius
        );
        ofVec3f end = start + ofVec3f(
            ofRandom(-120, 120),
            ofRandom(-180, 180),
            ofRandom(-120, 120)
        );
        createTendril(start, end);
    }

    // Create energy orbs
    for(int i = 0; i < 6; i++) {
        float angle = (TWO_PI / 6.0) * i;
        float radius = 200;
        ofVec3f center(
            cos(angle) * radius,
            ofRandom(-100, 100),
            sin(angle) * radius
        );
        createEnergyOrb(center);
    }

    // Create jelly creatures
    for(int i = 0; i < 5; i++) {
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(150, 250);
        ofVec3f pos(
            cos(angle) * radius,
            ofRandom(-150, 150),
            sin(angle) * radius
        );
        createJellyCreature(pos);
    }

    time = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    cameraRotation += 0.1;

    // Update camera animation
    updateCamera();

    // Get camera position for lighting
    ofVec3f camPos = cam.getPosition();

    // Update lighting positions
    mainLight.setPosition(camPos.x * 0.5, 200, camPos.z * 0.5);
    rimLight.setPosition(-camPos.x * 0.3, -100, -camPos.z * 0.3);

    // Create new particles
    if(particles.size() < maxParticles) {
        for(int i = 0; i < 8; i++) {
            float angle = ofRandom(TWO_PI);
            float radius = ofRandom(50, 320);
            ofVec3f pos(
                cos(angle) * radius,
                ofRandom(-330, -200),
                sin(angle) * radius
            );
            createParticle(pos);
        }
    }

    // Update particles
    for(int i = particles.size() - 1; i >= 0; i--) {
        particles[i].pos += particles[i].vel;
        particles[i].life -= ofGetLastFrameTime();

        // Update trail
        if(particles[i].trailIndex < particleTrails.size()) {
            particleTrails[particles[i].trailIndex].push_back(particles[i].pos);
            if(particleTrails[particles[i].trailIndex].size() > 15) {
                particleTrails[particles[i].trailIndex].erase(
                    particleTrails[particles[i].trailIndex].begin()
                );
            }
        }

        // Add swirling motion
        float angle = time + particles[i].pos.y * 0.01;
        particles[i].vel.x += cos(angle) * 0.15;
        particles[i].vel.z += sin(angle) * 0.15;

        // Gravity and buoyancy
        particles[i].vel.y += ofRandom(-0.05, 0.18);
        particles[i].vel *= 0.97;

        // Remove dead particles
        if(particles[i].life <= 0 || particles[i].pos.y > 450) {
            if(particles[i].trailIndex < particleTrails.size()) {
                particleTrails[particles[i].trailIndex].clear();
            }
            particles.erase(particles.begin() + i);
        }
    }

    // Update islands
    for(auto& island : islands) {
        island.rotation += island.rotSpeed;
        island.pos.y += sin(time + island.bobOffset) * 0.15;
    }

    // Update tendrils
    for(auto& tendril : tendrils) {
        tendril.phase += tendril.speed;
    }

    // Update energy orbs
    for(auto& orb : energyOrbs) {
        orb.orbitAngle += orb.orbitSpeed;
        orb.pos = orb.orbitCenter + ofVec3f(
            cos(orb.orbitAngle) * orb.orbitRadius,
            sin(orb.orbitAngle * 2) * 30,
            sin(orb.orbitAngle) * orb.orbitRadius
        );
    }

    // Update jelly creatures
    for(auto& jelly : jellyCreatures) {
        jelly.pos += jelly.vel;

        // Smooth floating motion
        float wobble = sin(time * 2 + jelly.pulsePhase);
        jelly.vel.y += wobble * 0.02;
        jelly.vel *= 0.99;

        // Update tentacles
        for(int i = 0; i < jelly.tentacles.size(); i++) {
            float t = float(i) / jelly.tentacles.size();
            float wave = sin(time * 3 + jelly.pulsePhase + t * PI * 4);
            jelly.tentacles[i] = jelly.pos + ofVec3f(
                cos(t * TWO_PI) * (15 + wave * 3),
                -20 + wave * 8,
                sin(t * TWO_PI) * (15 + wave * 3)
            );
        }

        // Boundary check
        if(jelly.pos.length() > 350) {
            jelly.vel -= jelly.pos.getNormalized() * 0.1;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(
        ofColor::fromHex(0x293039),
        ofColor::fromHex(0x0f0f1e),
        OF_GRADIENT_CIRCULAR
    );

    cam.begin();

    mainLight.enable();
    rimLight.enable();

    // Draw environmental effects
    drawEnvironmentalEffects();

    // Draw atmosphere
    drawAtmosphere();

    // Draw islands
    drawIslands();

    // Draw tendrils
    drawTendrils();

    // Draw energy orbs
    drawEnergyOrbs();

    // Draw jelly creatures
    drawJellyCreatures();

    // Draw particles
    drawParticles();

    mainLight.disable();
    rimLight.disable();

    cam.end();

    // Draw info
    ofSetColor(255, 200);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 0), 10, 20);
    ofDrawBitmapString("Particles: " + ofToString(particles.size()), 10, 35);
}

//--------------------------------------------------------------
void ofApp::createParticle(ofVec3f position) {
    Particle p;
    p.pos = position;
    p.vel = ofVec3f(ofRandom(-1, 1), ofRandom(1, 3), ofRandom(-1, 1));
    p.color = palette[int(ofRandom(2, 7))]; // Use brighter colors
    p.size = ofRandom(2, 10);
    p.life = ofRandom(5, 12);
    p.maxLife = p.life;
    p.isBioluminescent = ofRandom(1) > 0.25;
    p.pulseOffset = ofRandom(TWO_PI);
    p.trailIndex = particles.size();
    particles.push_back(p);
}

//--------------------------------------------------------------
void ofApp::createIsland(ofVec3f position) {
    FloatingIsland island;
    island.pos = position;
    island.rotation = ofRandom(TWO_PI);
    island.rotSpeed = ofRandom(-0.005, 0.005);
    island.scale = ofRandom(35, 70);
    island.color = palette[int(ofRandom(0, 3))];
    island.bobOffset = ofRandom(TWO_PI);

    // Create irregular island shape
    int numVertices = 25;
    for(int i = 0; i < numVertices; i++) {
        float angle = (TWO_PI / numVertices) * i;
        float radius = ofRandom(0.7, 1.3) * island.scale;
        float height = ofRandom(-15, 15);
        island.vertices.push_back(ofVec3f(
            cos(angle) * radius,
            height,
            sin(angle) * radius
        ));
    }

    // Add glowing crystals
    int numCrystals = ofRandom(8, 15);
    for(int i = 0; i < numCrystals; i++) {
        int idx = int(ofRandom(island.vertices.size()));
        island.crystalPositions.push_back(island.vertices[idx] * ofRandom(0.6, 0.9));
        island.crystalSizes.push_back(ofRandom(3, 8));
    }

    islands.push_back(island);
}

//--------------------------------------------------------------
void ofApp::createTendril(ofVec3f start, ofVec3f end) {
    Tendril tendril;
    tendril.thickness = ofRandom(1.5, 4);
    tendril.phase = ofRandom(TWO_PI);
    tendril.speed = ofRandom(0.01, 0.03);
    tendril.color = palette[int(ofRandom(2, 6))];
    tendril.color.setBrightness(200);

    // Create curved path
    int numPoints = 40;
    for(int i = 0; i < numPoints; i++) {
        float t = float(i) / (numPoints - 1);
        ofVec3f point = start.getInterpolated(end, t);

        // Add organic wave
        float wave = sin(t * PI * 3 + tendril.phase) * 25;
        point.x += wave;
        point.z += cos(t * PI * 2 + tendril.phase) * 20;

        tendril.points.push_back(point);

        // Add leaf positions
        if(i % 4 == 0 && i > 0 && i < numPoints - 1) {
            ofVec3f leafPos = point + ofVec3f(
                ofRandom(-10, 10),
                ofRandom(-5, 5),
                ofRandom(-10, 10)
            );
            tendril.leafPositions.push_back(leafPos);
        }
    }

    tendrils.push_back(tendril);
}

//--------------------------------------------------------------
void ofApp::createEnergyOrb(ofVec3f center) {
    EnergyOrb orb;
    orb.orbitCenter = center;
    orb.orbitRadius = ofRandom(40, 80);
    orb.orbitSpeed = ofRandom(0.005, 0.015);
    orb.orbitAngle = ofRandom(TWO_PI);
    orb.color = palette[int(ofRandom(3, 7))];
    orb.size = ofRandom(8, 15);
    orb.pulsePhase = ofRandom(TWO_PI);
    orb.pos = center;
    energyOrbs.push_back(orb);
}

//--------------------------------------------------------------
void ofApp::createJellyCreature(ofVec3f position) {
    JellyCreature jelly;
    jelly.pos = position;
    jelly.vel = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.3, 0.3), ofRandom(-0.5, 0.5));
    jelly.color = palette[int(ofRandom(2, 6))];
    jelly.size = ofRandom(15, 30);
    jelly.pulsePhase = ofRandom(TWO_PI);

    // Create tentacles
    int numTentacles = ofRandom(6, 10);
    for(int i = 0; i < numTentacles; i++) {
        jelly.tentacles.push_back(jelly.pos);
    }

    jellyCreatures.push_back(jelly);
}

//--------------------------------------------------------------
void ofApp::drawParticles() {
    ofEnablePointSprites();

    // Draw trails first
    for(int i = 0; i < particleTrails.size(); i++) {
        if(particleTrails[i].size() > 1) {
            ofSetLineWidth(1);
            for(int j = 1; j < particleTrails[i].size(); j++) {
                float alpha = (float(j) / particleTrails[i].size()) * 100;
                if(i < particles.size()) {
                    ofSetColor(particles[i].color, alpha);
                } else {
                    ofSetColor(palette[5], alpha);
                }
                ofDrawLine(particleTrails[i][j-1], particleTrails[i][j]);
            }
        }
    }

    // Draw particles
    for(auto& p : particles) {
        float alpha = (p.life / p.maxLife) * 255;
        float pulse = (sin(time * 3 + p.pulseOffset) + 1) * 0.5;

        if(p.isBioluminescent) {
            // Draw outer glow
            ofSetColor(p.color, alpha * 0.2);
            ofDrawSphere(p.pos, p.size * 4 + pulse * 2);

            // Draw mid glow
            ofSetColor(p.color, alpha * 0.4);
            ofDrawSphere(p.pos, p.size * 2.5 + pulse);
        }

        // Draw core
        ofSetColor(p.color, alpha);
        ofDrawSphere(p.pos, p.size + pulse * 0.5);

        // Draw bright center
        ofSetColor(255, 255, 255, alpha * 0.8);
        ofDrawSphere(p.pos, p.size * 0.3);
    }

    ofDisablePointSprites();
}

//--------------------------------------------------------------
void ofApp::drawIslands() {
    for(auto& island : islands) {
        ofPushMatrix();
        ofTranslate(island.pos);
        ofRotateDeg(island.rotation, 0, 1, 0);

        // Draw island mesh with depth
        ofSetColor(island.color);
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addVertex(ofVec3f(0, 0, 0));

        for(auto& v : island.vertices) {
            mesh.addVertex(v);
        }
        mesh.addVertex(island.vertices[0]);

        mesh.draw();

        // Draw glowing edges with pulse
        ofNoFill();
        ofSetLineWidth(2.5);
        float edgePulse = (sin(time * 2 + island.bobOffset) + 1) * 0.5;
        ofSetColor(palette[5], 120 + edgePulse * 135);

        for(int i = 0; i < island.vertices.size(); i++) {
            int next = (i + 1) % island.vertices.size();
            ofDrawLine(island.vertices[i], island.vertices[next]);
        }

        ofFill();

        // Draw crystals
        for(int i = 0; i < island.crystalPositions.size(); i++) {
            float pulse = (sin(time * 2.5 + i * 0.5 + island.bobOffset) + 1) * 0.5;
            ofVec3f crystalPos = island.crystalPositions[i];
            float crystalSize = island.crystalSizes[i];

            // Crystal glow
            ofSetColor(palette[6], 80 + pulse * 120);
            ofDrawSphere(crystalPos, crystalSize * 2);

            // Crystal core
            ofSetColor(palette[4], 200 + pulse * 55);
            ofDrawBox(crystalPos, crystalSize, crystalSize * 1.5, crystalSize);

            // Crystal bright tip
            ofSetColor(255, 255, 255, 180 + pulse * 75);
            ofDrawSphere(crystalPos + ofVec3f(0, crystalSize * 0.75, 0), crystalSize * 0.3);
        }

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawTendrils() {
    for(auto& tendril : tendrils) {
        // Draw glowing line
        for(int i = 0; i < tendril.points.size() - 1; i++) {
            float t = float(i) / tendril.points.size();
            float pulse = (sin(time * 2 + tendril.phase + t * PI * 2) + 1) * 0.5;

            ofColor glowColor = tendril.color;
            glowColor.setBrightness(100 + pulse * 155);

            ofVec3f p1 = tendril.points[i];
            ofVec3f p2 = tendril.points[i + 1];

            // Draw outer glow
            ofSetLineWidth(tendril.thickness * 6);
            ofSetColor(glowColor, 40);
            ofDrawLine(p1, p2);

            // Draw mid glow
            ofSetLineWidth(tendril.thickness * 3);
            ofSetColor(glowColor, 80);
            ofDrawLine(p1, p2);

            // Draw core
            ofSetLineWidth(tendril.thickness);
            ofSetColor(glowColor, 220);
            ofDrawLine(p1, p2);
        }

        // Draw bioluminescent nodes
        for(int i = 0; i < tendril.points.size(); i += 3) {
            float pulse = (sin(time * 3 + tendril.phase + i * 0.5) + 1) * 0.5;

            // Node glow
            ofSetColor(palette[4], 100 + pulse * 105);
            ofDrawSphere(tendril.points[i], 3 + pulse * 4);

            // Node core
            ofSetColor(palette[3], 180 + pulse * 75);
            ofDrawSphere(tendril.points[i], 2 + pulse * 2);
        }

        // Draw leaves
        for(auto& leafPos : tendril.leafPositions) {
            float pulse = (sin(time * 2.5 + leafPos.x * 0.1) + 1) * 0.5;

            ofSetColor(palette[5], 120 + pulse * 80);
            ofDrawCircle(leafPos, 3 + pulse * 2);

            ofSetColor(palette[6], 160 + pulse * 95);
            ofDrawCircle(leafPos, 1.5 + pulse);
        }
    }

    ofSetLineWidth(1);
}

//--------------------------------------------------------------
void ofApp::drawAtmosphere() {
    // Draw atmospheric fog layers
    for(int i = 0; i < 5; i++) {
        ofPushMatrix();
        ofRotateDeg(time * 1.5 + i * 72, 0, 1, 0);

        float pulse = (sin(time + i) + 1) * 0.5;
        ofSetColor(palette[6], 15 + pulse * 25);
        ofNoFill();
        ofDrawCircle(0, -150 + i * 75, 0, 320 + i * 40);
        ofFill();

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawEnergyOrbs() {
    for(auto& orb : energyOrbs) {
        float pulse = (sin(time * 2.5 + orb.pulsePhase) + 1) * 0.5;

        // Draw orbit path
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(orb.color, 30 + pulse * 40);
        ofPushMatrix();
        ofTranslate(orb.orbitCenter);
        ofRotateXDeg(90);
        ofDrawCircle(0, 0, orb.orbitRadius);
        ofPopMatrix();
        ofFill();

        // Draw orb layers
        // Outer glow
        ofSetColor(orb.color, 50 + pulse * 50);
        ofDrawSphere(orb.pos, orb.size * 2.5 + pulse * 5);

        // Mid glow
        ofSetColor(orb.color, 100 + pulse * 80);
        ofDrawSphere(orb.pos, orb.size * 1.5 + pulse * 3);

        // Core
        ofSetColor(orb.color, 200 + pulse * 55);
        ofDrawSphere(orb.pos, orb.size + pulse * 2);

        // Bright center
        ofSetColor(255, 255, 255, 230 + pulse * 25);
        ofDrawSphere(orb.pos, orb.size * 0.4);

        // Energy particles around orb
        for(int i = 0; i < 8; i++) {
            float angle = (TWO_PI / 8) * i + time * 2;
            float radius = orb.size * 2 + pulse * 3;
            ofVec3f particlePos = orb.pos + ofVec3f(
                cos(angle) * radius,
                sin(angle * 2) * (radius * 0.5),
                sin(angle) * radius
            );

            ofSetColor(orb.color, 180 + pulse * 75);
            ofDrawSphere(particlePos, 2 + pulse);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawJellyCreatures() {
    for(auto& jelly : jellyCreatures) {
        float pulse = (sin(time * 2 + jelly.pulsePhase) + 1) * 0.5;

        // Draw bell/dome
        ofSetColor(jelly.color, 120 + pulse * 80);
        ofDrawSphere(jelly.pos, jelly.size + pulse * 3);

        // Draw inner dome
        ofSetColor(jelly.color, 60 + pulse * 60);
        ofDrawSphere(jelly.pos, jelly.size * 0.7 + pulse * 2);

        // Draw bright spots
        for(int i = 0; i < 6; i++) {
            float angle = (TWO_PI / 6) * i;
            ofVec3f spotPos = jelly.pos + ofVec3f(
                cos(angle) * jelly.size * 0.5,
                sin(time * 3 + i) * 3,
                sin(angle) * jelly.size * 0.5
            );

            ofSetColor(palette[5], 150 + pulse * 105);
            ofDrawSphere(spotPos, 3 + pulse * 2);
        }

        // Draw tentacles
        ofSetLineWidth(2);
        for(int i = 0; i < jelly.tentacles.size(); i++) {
            float t = float(i) / jelly.tentacles.size();

            // Tentacle glow
            ofSetLineWidth(4);
            ofSetColor(jelly.color, 60 + pulse * 40);
            ofDrawLine(jelly.pos, jelly.tentacles[i]);

            // Tentacle core
            ofSetLineWidth(2);
            ofSetColor(jelly.color, 140 + pulse * 80);
            ofDrawLine(jelly.pos, jelly.tentacles[i]);

            // Tentacle tip
            ofSetColor(palette[4], 180 + pulse * 75);
            ofDrawSphere(jelly.tentacles[i], 2 + pulse * 1.5);
        }

        ofSetLineWidth(1);
    }
}

//--------------------------------------------------------------
void ofApp::drawEnvironmentalEffects() {
    // Draw floating energy wisps
    for(int i = 0; i < 30; i++) {
        float angle = (TWO_PI / 30) * i;
        float radius = 280 + sin(time * 0.5 + i) * 50;
        float height = sin(time * 0.3 + i * 0.5) * 200;

        ofVec3f wispPos(
            cos(angle + time * 0.2) * radius,
            height,
            sin(angle + time * 0.2) * radius
        );

        float pulse = (sin(time * 3 + i * 0.3) + 1) * 0.5;
        ofColor wispColor = palette[int(ofRandom(4, 7))];

        ofSetColor(wispColor, 40 + pulse * 60);
        ofDrawSphere(wispPos, 2 + pulse * 3);
    }

    // Draw connecting energy beams between close elements
    ofSetLineWidth(1);
    for(int i = 0; i < energyOrbs.size(); i++) {
        for(int j = i + 1; j < energyOrbs.size(); j++) {
            float dist = energyOrbs[i].pos.distance(energyOrbs[j].pos);
            if(dist < 200) {
                float alpha = ofMap(dist, 0, 200, 100, 0);
                float pulse = (sin(time * 2 + i + j) + 1) * 0.5;

                ofSetColor(palette[5], alpha * pulse);
                ofDrawLine(energyOrbs[i].pos, energyOrbs[j].pos);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::updateCamera() {
    cameraModeTime += ofGetLastFrameTime();

    // Switch camera mode when duration is reached
    if(cameraModeTime >= cameraModeDuration) {
        switchCameraMode();
        cameraModeTime = 0;
    }

    // Smooth transition factor
    float t = cameraModeTime / cameraModeDuration;
    float smoothT = sin(t * PI * 0.5); // Ease in-out

    ofVec3f camPos;
    ofVec3f lookAtPos;

    switch(currentCameraMode) {
        case ORBIT:
            // Wide circular orbit around the entire scene
            {
                float angle = time * 0.15;
                float radius = 480 + sin(time * 0.3) * 80;
                float height = sin(time * 0.2) * 150;

                camPos = ofVec3f(
                    cos(angle) * radius,
                    height,
                    sin(angle) * radius
                );
                lookAtPos = ofVec3f(0, 0, 0);
            }
            break;

        case FLY_THROUGH:
            // Fly through the scene in a figure-8 pattern
            {
                float angle = time * 0.25;
                float radius = 350;

                camPos = ofVec3f(
                    sin(angle * 2) * radius,
                    cos(angle * 3) * 200,
                    cos(angle * 2) * radius
                );

                // Look ahead in the flight path
                float lookAheadAngle = angle + 0.3;
                lookAtPos = ofVec3f(
                    sin(lookAheadAngle * 2) * radius * 0.5,
                    cos(lookAheadAngle * 3) * 100,
                    cos(lookAheadAngle * 2) * radius * 0.5
                );
            }
            break;

        case ZOOM_IN:
            // Zoom in from far to close, then back out
            {
                float zoomPhase = sin(t * PI); // 0 -> 1 -> 0
                float distance = 700 - (zoomPhase * 500); // 700 -> 200 -> 700
                float angle = time * 0.1 + t * TWO_PI;

                camPos = ofVec3f(
                    cos(angle) * distance,
                    sin(time * 0.4) * 100 * (1.0 - zoomPhase * 0.5),
                    sin(angle) * distance
                );

                // Focus on center during zoom
                lookAtPos = ofVec3f(
                    sin(time * 0.2) * 50,
                    cos(time * 0.15) * 30,
                    0
                );
            }
            break;

        case SPIRAL:
            // Spiral upward motion
            {
                float angle = time * 0.2 + t * PI * 4;
                float radius = 400 - (t * 150); // Spiral inward
                float height = -300 + (t * 600); // Move from bottom to top

                camPos = ofVec3f(
                    cos(angle) * radius,
                    height,
                    sin(angle) * radius
                );

                // Look at the center, slightly ahead vertically
                lookAtPos = ofVec3f(0, height + 50, 0);
            }
            break;

        case FOCUS_ELEMENT:
            // Focus on specific elements (islands, orbs, jellies)
            {
                ofVec3f targetPos;

                // Cycle through different elements
                int elementType = focusedElementIndex % 3;

                if(elementType == 0 && islands.size() > 0) {
                    // Focus on an island
                    int islandIdx = (focusedElementIndex / 3) % islands.size();
                    targetPos = islands[islandIdx].pos;
                }
                else if(elementType == 1 && energyOrbs.size() > 0) {
                    // Focus on an energy orb
                    int orbIdx = (focusedElementIndex / 3) % energyOrbs.size();
                    targetPos = energyOrbs[orbIdx].pos;
                }
                else if(elementType == 2 && jellyCreatures.size() > 0) {
                    // Focus on a jelly creature
                    int jellyIdx = (focusedElementIndex / 3) % jellyCreatures.size();
                    targetPos = jellyCreatures[jellyIdx].pos;
                }
                else {
                    targetPos = ofVec3f(0, 0, 0);
                }

                // Smooth transition to target
                cameraTarget = cameraTarget.getInterpolated(targetPos, 0.05);

                // Orbit around the focused element
                float orbitAngle = time * 0.3;
                float orbitRadius = 120 + sin(time * 0.5) * 30;

                camPos = cameraTarget + ofVec3f(
                    cos(orbitAngle) * orbitRadius,
                    sin(time * 0.4) * 40,
                    sin(orbitAngle) * orbitRadius
                );

                lookAtPos = cameraTarget;
            }
            break;
    }

    // Apply position and look-at
    cam.setPosition(camPos);
    cam.lookAt(lookAtPos);
}

//--------------------------------------------------------------
void ofApp::switchCameraMode() {
    // Cycle through camera modes
    int nextMode = (currentCameraMode + 1) % 5;
    currentCameraMode = static_cast<CameraMode>(nextMode);

    // If switching to focus mode, pick a new element
    if(currentCameraMode == FOCUS_ELEMENT) {
        focusedElementIndex++;
        // Initialize target to first element position
        if(islands.size() > 0) {
            cameraTarget = islands[0].pos;
        }
    }

    // Vary duration for different modes
    switch(currentCameraMode) {
        case ORBIT:
            cameraModeDuration = 15.0;
            break;
        case FLY_THROUGH:
            cameraModeDuration = 18.0;
            break;
        case ZOOM_IN:
            cameraModeDuration = 14.0;
            break;
        case SPIRAL:
            cameraModeDuration = 16.0;
            break;
        case FOCUS_ELEMENT:
            cameraModeDuration = 10.0;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
