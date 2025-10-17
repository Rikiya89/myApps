#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetSphereResolution(24);
    ofSetCircleResolution(64);

    time = 0;
    rotationAngle = 0;
    numParticles = 400;
    flowFieldResolution = 10;

    // Initialize color palette
    initializePalette();

    // Set background
    ofBackground(41, 48, 57); // #293039

    // Setup lighting
    pointLight.setPointLight();
    pointLight.setDiffuseColor(ofColor(132, 186, 231)); // #84bae7
    pointLight.setSpecularColor(ofColor(200, 161, 229)); // #bda1e5

    ambientLight.setAmbientColor(ofColor(80, 80, 100));

    // Setup material
    material.setShininess(120);
    material.setSpecularColor(ofColor(255, 255, 255, 255));

    // Create elements
    createFlowField();
    createParticles();
    createGeometricShapes();

    // Set camera distance
    cam.setDistance(800);
}

//--------------------------------------------------------------
void ofApp::initializePalette(){
    palette.clear();

    // Add the theme colors
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
}

//--------------------------------------------------------------
void ofApp::createFlowField(){
    flowField.clear();
    int res = flowFieldResolution;

    for(int x = 0; x < res; x++){
        flowField.push_back(vector<vector<ofVec3f>>());
        for(int y = 0; y < res; y++){
            flowField[x].push_back(vector<ofVec3f>());
            for(int z = 0; z < res; z++){
                ofVec3f vec(
                    ofNoise(x * 0.1, y * 0.1, z * 0.1) * 2 - 1,
                    ofNoise(x * 0.1 + 100, y * 0.1 + 100, z * 0.1) * 2 - 1,
                    ofNoise(x * 0.1, y * 0.1 + 100, z * 0.1 + 100) * 2 - 1
                );
                flowField[x][y].push_back(vec);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::createParticles(){
    particles.clear();

    for(int i = 0; i < numParticles; i++){
        Particle p;

        // Create particles in various formations
        if(i < numParticles * 0.6){
            // Spherical distribution
            float radius = ofRandom(150, 450);
            float theta = ofRandom(TWO_PI);
            float phi = ofRandom(-PI, PI);

            p.position.x = radius * cos(theta) * cos(phi);
            p.position.y = radius * sin(phi);
            p.position.z = radius * sin(theta) * cos(phi);
        } else {
            // Spiral formation
            float angle = ofRandom(TWO_PI * 4);
            float radius = ofRandom(200, 400);
            p.position.x = cos(angle) * radius;
            p.position.y = ofRandom(-300, 300);
            p.position.z = sin(angle) * radius;
        }

        // Small random velocity
        p.velocity.x = ofRandom(-0.3, 0.3);
        p.velocity.y = ofRandom(-0.3, 0.3);
        p.velocity.z = ofRandom(-0.3, 0.3);

        // Random color from palette
        p.color = palette[int(ofRandom(palette.size()))];
        p.color.a = ofRandom(180, 255);

        // Random size
        p.size = ofRandom(2, 12);

        // Noise offsets for organic movement
        p.noiseOffsetX = ofRandom(1000);
        p.noiseOffsetY = ofRandom(1000);
        p.noiseOffsetZ = ofRandom(1000);

        p.life = ofRandom(1.0);
        p.rotationSpeed = ofRandom(-1, 1);

        particles.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::createGeometricShapes(){
    shapes.clear();

    for(int i = 0; i < 15; i++){
        GeometricShape shape;

        // Arrange in orbital patterns
        float angle = (float)i / 15.0 * TWO_PI;
        float radius = 300 + (i % 3) * 100;

        shape.position.x = cos(angle) * radius;
        shape.position.y = sin(angle * 2) * 150;
        shape.position.z = sin(angle) * radius;

        shape.rotation = ofVec3f(ofRandom(360), ofRandom(360), ofRandom(360));
        shape.rotationSpeed = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));

        shape.size = ofRandom(15, 45);
        shape.color = palette[i % palette.size()];
        shape.color.a = 120;
        shape.type = i % 4;

        shapes.push_back(shape);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;
    rotationAngle += 0.2;

    // Update point light position
    pointLight.setPosition(
        cos(time) * 400,
        sin(time * 0.7) * 300,
        sin(time) * 400
    );

    // Update particles
    for(int i = 0; i < particles.size(); i++){
        // Use Perlin noise for organic movement
        float noiseScale = 0.3;
        float noiseX = ofNoise(particles[i].noiseOffsetX + time * noiseScale) * 2 - 1;
        float noiseY = ofNoise(particles[i].noiseOffsetY + time * noiseScale) * 2 - 1;
        float noiseZ = ofNoise(particles[i].noiseOffsetZ + time * noiseScale) * 2 - 1;

        // Apply noise to velocity
        particles[i].velocity.x += noiseX * 0.15;
        particles[i].velocity.y += noiseY * 0.15;
        particles[i].velocity.z += noiseZ * 0.15;

        // Flow field influence
        ofVec3f flowForce(0, 0, 0);
        float gridX = ofMap(particles[i].position.x, -500, 500, 0, flowFieldResolution - 1, true);
        float gridY = ofMap(particles[i].position.y, -500, 500, 0, flowFieldResolution - 1, true);
        float gridZ = ofMap(particles[i].position.z, -500, 500, 0, flowFieldResolution - 1, true);

        if(gridX >= 0 && gridX < flowFieldResolution - 1 &&
           gridY >= 0 && gridY < flowFieldResolution - 1 &&
           gridZ >= 0 && gridZ < flowFieldResolution - 1){
            flowForce = flowField[int(gridX)][int(gridY)][int(gridZ)];
            particles[i].velocity += flowForce * 0.05;
        }

        // Orbital motion
        float orbitForce = 0.02;
        ofVec3f toCenter = -particles[i].position.normalized();
        ofVec3f tangent = toCenter.crossed(ofVec3f(0, 1, 0)).normalized();
        particles[i].velocity += tangent * orbitForce;

        // Damping
        particles[i].velocity *= 0.97;

        // Update position
        particles[i].position += particles[i].velocity;

        // Keep particles in bounds with elastic boundary
        float maxDistance = 550;
        float distance = particles[i].position.length();
        if(distance > maxDistance){
            particles[i].position = particles[i].position.normalized() * maxDistance;
            particles[i].velocity *= -0.5;
        }

        // Update trail
        particles[i].trail.push_front(particles[i].position);
        if(particles[i].trail.size() > 20){
            particles[i].trail.pop_back();
        }

        // Update life cycle
        particles[i].life += 0.002;
        if(particles[i].life > 1.0) particles[i].life = 0;

        // Smooth color cycling with interpolation
        float colorPos = particles[i].life * (palette.size() - 1);
        int colorIndex1 = int(colorPos) % palette.size();
        int colorIndex2 = (colorIndex1 + 1) % palette.size();
        float blend = colorPos - int(colorPos);

        particles[i].color = palette[colorIndex1];
        particles[i].color.lerp(palette[colorIndex2], blend);
        particles[i].color.a = 180 + sin(particles[i].life * TWO_PI) * 75;
    }

    // Update geometric shapes
    for(int i = 0; i < shapes.size(); i++){
        shapes[i].rotation += shapes[i].rotationSpeed;

        // Orbital motion for shapes
        float angle = time * 0.5 + i * 0.5;
        float radius = 300 + (i % 3) * 100;
        shapes[i].position.x = cos(angle) * radius;
        shapes[i].position.y = sin(angle * 2 + i) * 150;
        shapes[i].position.z = sin(angle) * radius;

        // Color cycling
        float cycleSpeed = time * 0.5 + i * 0.3;
        int colorIdx = int(cycleSpeed) % palette.size();
        shapes[i].color = palette[colorIdx];
        shapes[i].color.a = 120 + sin(cycleSpeed * 2) * 40;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    // Enable lighting
    ofEnableLighting();
    pointLight.enable();
    ambientLight.enable();

    cam.begin();

    // Slow global rotation
    ofRotateYDeg(rotationAngle * 0.15);
    ofRotateXDeg(sin(time * 0.3) * 15);
    ofRotateZDeg(cos(time * 0.4) * 10);

    // Draw crystal structure
    drawCrystalStructure();

    // Draw energy rings
    drawEnergyRings();

    // Draw flow ribbons
    drawFlowRibbons();

    // Draw geometric shapes with material
    material.begin();
    for(int i = 0; i < shapes.size(); i++){
        ofPushMatrix();
        ofTranslate(shapes[i].position);
        ofRotateXDeg(shapes[i].rotation.x);
        ofRotateYDeg(shapes[i].rotation.y);
        ofRotateZDeg(shapes[i].rotation.z);

        material.setDiffuseColor(shapes[i].color);
        ofSetColor(shapes[i].color);

        switch(shapes[i].type){
            case 0: ofDrawBox(shapes[i].size); break;
            case 1: ofDrawSphere(shapes[i].size); break;
            case 2: ofDrawCone(shapes[i].size, shapes[i].size * 1.5); break;
            case 3: ofDrawIcoSphere(shapes[i].size); break;
        }

        ofPopMatrix();
    }
    material.end();

    // Disable lighting for particles
    ofDisableLighting();

    // Draw particle trails
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for(int i = 0; i < particles.size(); i++){
        if(particles[i].trail.size() > 1){
            ofSetColor(particles[i].color, particles[i].color.a * 0.3);
            ofSetLineWidth(1);

            for(int j = 0; j < particles[i].trail.size() - 1; j++){
                float alpha = ofMap(j, 0, particles[i].trail.size(), particles[i].color.a * 0.3, 0);
                ofSetColor(particles[i].color, alpha);
                ofDrawLine(particles[i].trail[j], particles[i].trail[j + 1]);
            }
        }
    }

    // Draw connecting web between nearby particles
    ofSetLineWidth(1);
    for(int i = 0; i < particles.size(); i++){
        int connections = 0;
        for(int j = i + 1; j < particles.size() && connections < 3; j++){
            float distance = particles[i].position.distance(particles[j].position);

            if(distance < 120){
                float alpha = ofMap(distance, 0, 120, 120, 0);

                // Color blend between the two particles
                ofColor blendColor = particles[i].color;
                blendColor.lerp(particles[j].color, 0.5);
                blendColor.a = alpha * 0.4;

                ofSetColor(blendColor);
                ofDrawLine(particles[i].position, particles[j].position);
                connections++;
            }
        }
    }

    // Draw particles as glowing orbs
    for(int i = 0; i < particles.size(); i++){
        ofPushMatrix();
        ofTranslate(particles[i].position);

        // Outer glow
        ofSetColor(particles[i].color, particles[i].color.a * 0.2);
        ofDrawSphere(0, 0, 0, particles[i].size * 2.5);

        // Mid glow
        ofSetColor(particles[i].color, particles[i].color.a * 0.5);
        ofDrawSphere(0, 0, 0, particles[i].size * 1.5);

        // Core
        ofSetColor(particles[i].color);
        ofDrawSphere(0, 0, 0, particles[i].size);

        ofPopMatrix();
    }

    cam.end();

    // Draw UI
    ofDisableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(200, 200, 220, 200);
//    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 0), 20, 20);
//    ofDrawBitmapString("Particles: " + ofToString(particles.size()), 20, 40);
//    ofDrawBitmapString("Press SPACE to regenerate", 20, 60);
//    ofDrawBitmapString("Drag mouse to rotate view", 20, 80);
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::drawCrystalStructure(){
    ofNoFill();
    ofSetLineWidth(2);

    for(int layer = 0; layer < 8; layer++){
        ofSetColor(palette[layer % palette.size()], 60);

        int numPoints = 6;
        float radius = 100 + layer * 40;
        float heightOffset = sin(time + layer * 0.5) * 50;

        // Draw hexagonal crystal structure
        for(int side = 0; side < 6; side++){
            ofBeginShape();
            for(int i = 0; i <= 20; i++){
                float t = (float)i / 20.0;
                float angle1 = (float)side / 6.0 * TWO_PI;
                float angle2 = (float)(side + 1) / 6.0 * TWO_PI;

                float x = ofLerp(cos(angle1), cos(angle2), t) * radius;
                float y = sin(time * 0.5 + t * PI) * 30 + heightOffset;
                float z = ofLerp(sin(angle1), sin(angle2), t) * radius;

                ofVertex(x, y, z);
            }
            ofEndShape();
        }
    }

    ofFill();
}

//--------------------------------------------------------------
void ofApp::drawEnergyRings(){
    ofNoFill();
    ofSetLineWidth(3);

    for(int i = 0; i < 5; i++){
        float phase = time + i * PI / 2.5;
        float radius = 200 + i * 60;

        ofPushMatrix();

        ofRotateXDeg(90);
        ofRotateZDeg(sin(phase) * 30);

        ofColor ringColor = palette[(i * 2) % palette.size()];
        ringColor.a = 100 + sin(phase * 2) * 50;
        ofSetColor(ringColor);

        ofDrawCircle(0, 0, radius);

        // Inner ring glow
        ringColor.a *= 0.4;
        ofSetColor(ringColor);
        ofDrawCircle(0, 0, radius + 5);

        ofPopMatrix();
    }

    ofFill();
}

//--------------------------------------------------------------
void ofApp::drawFlowRibbons(){
    ofNoFill();
    ofSetLineWidth(2);

    for(int ribbon = 0; ribbon < 10; ribbon++){
        ofSetColor(palette[ribbon % palette.size()], 80);

        ofBeginShape();
        for(int i = 0; i < 100; i++){
            float t = (float)i / 100.0;
            float angle = t * TWO_PI * 4 + time + ribbon * 0.5;
            float radius = 250 + sin(angle * 2) * 100;

            float x = cos(angle) * radius;
            float y = sin(t * TWO_PI * 2 + time) * 200;
            float z = sin(angle) * radius;

            // Add wave motion
            x += sin(time * 2 + t * TWO_PI) * 30;
            z += cos(time * 2 + t * TWO_PI) * 30;

            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    ofFill();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        createParticles();
        createGeometricShapes();
        createFlowField();
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
