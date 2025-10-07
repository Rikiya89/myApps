#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    time = 0;
    maxParticles = 400;
    pulseAmount = 0;
    breatheAmount = 0;

    cam.setDistance(500);

    // Initialize ring systems
    for (int i = 0; i < 5; i++) {
        RingSystem ring;
        ring.radius = 50 + i * 40;
        ring.rotation = ofRandom(360);
        ring.speed = ofRandom(0.5, 2.0) * (i % 2 == 0 ? 1 : -1);
        ring.numPoints = 30 + i * 10;
        ring.waveOffset = ofRandom(100);
        rings.push_back(ring);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.01;

    // Animate pulse and breathe effects
    pulseAmount = sin(time * 2.0) * 0.5 + 0.5;
    breatheAmount = sin(time * 1.5) * 50;

    // Add new particles
    if (particles.size() < maxParticles) {
        for (int i = 0; i < 3; i++) {
            addParticle();
        }
    }

    // Update ring systems
    for (auto& ring : rings) {
        ring.rotation += ring.speed;
        ring.waveOffset += 0.05;
    }

    // Update particles
    for (int i = particles.size() - 1; i >= 0; i--) {
        particles[i].position += particles[i].velocity;
        particles[i].rotation += particles[i].rotationSpeed;
        particles[i].life -= 0.004;

        // Add some noise-based movement
        float noiseX = ofNoise(particles[i].position.x * 0.005, time) - 0.5;
        float noiseY = ofNoise(particles[i].position.y * 0.005, time + 100) - 0.5;
        float noiseZ = ofNoise(particles[i].position.z * 0.005, time + 200) - 0.5;

        particles[i].velocity.x += noiseX * 0.3;
        particles[i].velocity.y += noiseY * 0.3;
        particles[i].velocity.z += noiseZ * 0.3;

        // Add orbital motion based on particle type
        if (particles[i].type == 1) {
            float angle = particles[i].phase + time * 3;
            float orbitRadius = 100;
            ofVec3f orbitCenter(0, 0, 0);
            ofVec3f targetPos = orbitCenter + ofVec3f(
                cos(angle) * orbitRadius,
                sin(angle * 0.7) * orbitRadius * 0.5,
                sin(angle) * orbitRadius
            );
            ofVec3f force = (targetPos - particles[i].position) * 0.01;
            particles[i].velocity += force;
        }

        particles[i].velocity *= 0.97; // Damping

        // Remove dead particles
        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();

    // Draw pulsing spheres
    drawPulsingSpheres();

    // Draw geometric shapes
    drawGeometricShapes();

    // Draw flowing lines
    drawFlowingLines();

    // Draw animated ring systems
    for (auto& ring : rings) {
        ofPushMatrix();
        ofRotateXDeg(45 + sin(time) * 10);
        ofRotateZDeg(ring.rotation);

        ofSetColor(255, 80);
        ofNoFill();
        ofSetLineWidth(1);

        ofBeginShape();
        for (int i = 0; i <= ring.numPoints; i++) {
            float angle = (i / (float)ring.numPoints) * TWO_PI;
            float wave = sin(angle * 3 + ring.waveOffset) * 10;
            float x = cos(angle) * (ring.radius + wave);
            float y = sin(angle) * (ring.radius + wave);
            float z = sin(angle * 2 + time * 2) * 20;
            ofVertex(x, y, z);
        }
        ofEndShape(true);

        ofPopMatrix();
    }

    // Draw particles with different types
    for (auto& p : particles) {
        ofPushMatrix();
        ofTranslate(p.position);

        float alpha = p.life * 255;

        if (p.type == 0) {
            // Box particles
            ofRotateDeg(p.rotation, 1, 1, 0);
            ofSetColor(255, alpha);
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawBox(0, 0, 0, p.size);
        } else if (p.type == 1) {
            // Sphere particles with glow
            ofSetColor(255, alpha * 0.8);
            ofDrawSphere(0, 0, 0, p.size * 0.5);
            ofNoFill();
            ofSetColor(255, alpha * 0.5);
            ofDrawSphere(0, 0, 0, p.size);
        } else {
            // Pyramid particles
            ofRotateDeg(p.rotation, 0, 1, 0);
            ofSetColor(255, alpha);
            ofNoFill();
            ofSetLineWidth(1);
            ofDrawCone(0, 0, 0, p.size, p.size * 1.5);
        }

        // Draw connecting lines with fade
        if (ofRandom(1) > 0.6) {
            ofSetColor(255, alpha * 0.2);
            ofDrawLine(0, 0, 0, -p.position.x * 0.3, -p.position.y * 0.3, -p.position.z * 0.3);
        }

        ofPopMatrix();
    }

    // Draw spiral structures with animation
    ofSetColor(255, 120);
    ofNoFill();
    ofSetLineWidth(1);

    // Expanding spirals
    for (int s = 0; s < 3; s++) {
        ofBeginShape();
        for (int i = 0; i < 120; i++) {
            float angle = i * 0.25 + time * 15 + s * 120;
            float radius = i * 1.3 + pulseAmount * 20;
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;
            float z = i * 1.5 - 90 + sin(time * 2 + s) * 40;
            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    cam.end();
}

//--------------------------------------------------------------
void ofApp::addParticle(){
    Particle p;

    // Start from center with random direction
    p.position = ofVec3f(0, 0, 0);

    float angle1 = ofRandom(TWO_PI);
    float angle2 = ofRandom(TWO_PI);
    float speed = ofRandom(1, 3);

    p.velocity = ofVec3f(
        cos(angle1) * cos(angle2) * speed,
        sin(angle1) * cos(angle2) * speed,
        sin(angle2) * speed
    );

    p.size = ofRandom(5, 20);
    p.rotation = ofRandom(360);
    p.rotationSpeed = ofRandom(-2, 2);
    p.life = 1.0;
    p.phase = ofRandom(TWO_PI);
    p.type = (int)ofRandom(3); // 0: box, 1: sphere, 2: cone

    particles.push_back(p);
}

//--------------------------------------------------------------
void ofApp::drawPulsingSpheres(){
    // Draw multiple pulsing wireframe spheres
    ofNoFill();
    ofSetLineWidth(1);

    for (int i = 0; i < 4; i++) {
        ofPushMatrix();

        float rotSpeed = 8 + i * 2;
        ofRotateYDeg(time * rotSpeed);
        ofRotateXDeg(time * (rotSpeed * 0.5));
        ofRotateZDeg(time * (rotSpeed * 0.3));

        float radius = 120 + i * 30 + breatheAmount * 0.5;
        float alpha = 15 + pulseAmount * 20;

        ofSetColor(255, alpha);
        ofDrawSphere(0, 0, 0, radius);

        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::drawGeometricShapes(){
    ofNoFill();
    ofSetLineWidth(1);

    // Rotating cubes
    for (int i = 0; i < 3; i++) {
        ofPushMatrix();

        float size = 60 + i * 40;
        ofRotateYDeg(time * (10 + i * 3));
        ofRotateXDeg(time * (8 - i * 2));

        ofSetColor(255, 40 + pulseAmount * 30);
        ofDrawBox(0, 0, 0, size + sin(time * 2 + i) * 10);

        ofPopMatrix();
    }

    // Animated torus
    ofPushMatrix();
    ofRotateXDeg(90 + sin(time) * 20);
    ofRotateZDeg(time * 15);
    ofSetColor(255, 60);

    float torusRadius1 = 80 + pulseAmount * 20;
    float torusRadius2 = 20 + sin(time * 3) * 5;

    // Draw torus manually with connected circles
    int segments = 30;
    for (int i = 0; i < segments; i++) {
        float angle = (i / (float)segments) * TWO_PI;
        ofPushMatrix();
        ofTranslate(cos(angle) * torusRadius1, sin(angle) * torusRadius1, 0);
        ofRotateZDeg(angle * RAD_TO_DEG);
        ofDrawCircle(0, 0, torusRadius2);
        ofPopMatrix();
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawFlowingLines(){
    ofSetLineWidth(1);

    // Flowing wave patterns
    for (int layer = 0; layer < 5; layer++) {
        ofSetColor(255, 60);

        ofBeginShape();
        for (int i = 0; i < 200; i++) {
            float t = i / 200.0;
            float angle = t * TWO_PI * 3 + time * 5 + layer * 0.5;
            float radius = 100 + layer * 20;

            float x = cos(angle) * radius;
            float y = sin(angle * 0.7) * radius + sin(time * 2 + t * 10) * 30;
            float z = sin(angle) * radius * 0.5 + cos(time * 1.5 + layer) * 40;

            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    // DNA helix structure
    ofSetColor(255, 80);
    int helixPoints = 150;

    for (int strand = 0; strand < 2; strand++) {
        ofBeginShape();
        for (int i = 0; i < helixPoints; i++) {
            float t = i / (float)helixPoints;
            float angle = t * TWO_PI * 4 + time * 10 + strand * PI;
            float helixRadius = 60;

            float x = cos(angle) * helixRadius;
            float y = t * 300 - 150;
            float z = sin(angle) * helixRadius;

            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    // Connection lines between DNA strands
    ofSetColor(255, 40);
    for (int i = 0; i < helixPoints; i += 5) {
        float t = i / (float)helixPoints;
        float angle1 = t * TWO_PI * 4 + time * 10;
        float angle2 = t * TWO_PI * 4 + time * 10 + PI;
        float helixRadius = 60;

        float x1 = cos(angle1) * helixRadius;
        float y = t * 300 - 150;
        float z1 = sin(angle1) * helixRadius;

        float x2 = cos(angle2) * helixRadius;
        float z2 = sin(angle2) * helixRadius;

        ofDrawLine(x1, y, z1, x2, y, z2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        particles.clear();
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
