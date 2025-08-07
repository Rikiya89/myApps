#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    time = 0;
    numParticles = 800;
    cameraRotationSpeed = 0.2;
    autoRotateCamera = true;
    
    initializePalette();
    createParticles();
    
    cam.setDistance(500);
    cam.setFov(70);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    updateParticles();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::fromHex(0x0F0F23), ofColor::fromHex(0x1A1A2E));
    
    if(autoRotateCamera) {
        float camX = cos(time * cameraRotationSpeed) * cam.getDistance();
        float camZ = sin(time * cameraRotationSpeed) * cam.getDistance();
        cam.setPosition(camX, cam.getPosition().y, camZ);
        cam.lookAt(ofVec3f(0, 0, 0));
    }
    
    cam.begin();
    
    drawParticles();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        autoRotateCamera = !autoRotateCamera;
    }
    if(key == 'r' || key == 'R') {
        createParticles();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(!autoRotateCamera) {
        float mouseInfluence = 0.001;
        cameraRotationSpeed = (x - ofGetWidth()/2) * mouseInfluence;
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    autoRotateCamera = false;
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
void ofApp::initializePalette(){
    palette.clear();
    palette.push_back(ofColor::fromHex(0x2C2370)); // deep indigo
    palette.push_back(ofColor::fromHex(0x3E2B9B)); // rich violet
    palette.push_back(ofColor::fromHex(0x885DC8)); // medium purple
    palette.push_back(ofColor::fromHex(0xB799E4)); // light lavender
    palette.push_back(ofColor::fromHex(0xCABBE8)); // pale lavender
    palette.push_back(ofColor::fromHex(0x78B9E6)); // soft sky-blue
    palette.push_back(ofColor::fromHex(0x4059D4)); // vivid royal-blue
    palette.push_back(ofColor::fromHex(0x212F7D)); // midnight blue
    palette.push_back(ofColor::fromHex(0x141B21)); // near-black blue
    palette.push_back(ofColor::fromHex(0x1E2E28)); // deep teal-black
}

//--------------------------------------------------------------
void ofApp::createParticles(){
    particles.clear();
    velocities.clear();
    sizes.clear();
    particleColors.clear();
    shapeTypes.clear();
    rotationAngles.clear();
    rotationAxis.clear();
    
    for(int i = 0; i < numParticles; i++){
        ofVec3f pos;
        pos.x = ofRandom(-200, 200);
        pos.y = ofRandom(-200, 200);
        pos.z = ofRandom(-200, 200);
        particles.push_back(pos);
        
        ofVec3f vel;
        vel.x = ofRandom(-0.5, 0.5);
        vel.y = ofRandom(-0.5, 0.5);
        vel.z = ofRandom(-0.5, 0.5);
        velocities.push_back(vel);
        
        sizes.push_back(ofRandom(3, 12));
        shapeTypes.push_back(ofRandom(3));
        rotationAngles.push_back(ofRandom(360));
        rotationAxis.push_back(ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)).getNormalized());
        
        float colorIndex = (float)i / numParticles;
        particleColors.push_back(getColorFromPalette(colorIndex));
    }
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
    for(int i = 0; i < particles.size(); i++){
        float noiseScale = 0.005;
        float noiseStrength = 2.0;
        
        ofVec3f waveForce;
        float waveTime = time * 0.8;
        waveForce.x = sin(waveTime + particles[i].y * 0.01) * 0.3;
        waveForce.y = cos(waveTime + particles[i].x * 0.01) * 0.3;
        waveForce.z = sin(waveTime * 0.7 + particles[i].x * 0.005 + particles[i].y * 0.005) * 0.2;
        
        ofVec3f noise;
        noise.x = ofNoise(particles[i].x * noiseScale, particles[i].y * noiseScale, particles[i].z * noiseScale, time * 0.15) - 0.5;
        noise.y = ofNoise(particles[i].x * noiseScale + 1000, particles[i].y * noiseScale, particles[i].z * noiseScale, time * 0.15) - 0.5;
        noise.z = ofNoise(particles[i].x * noiseScale, particles[i].y * noiseScale + 1000, particles[i].z * noiseScale, time * 0.15) - 0.5;
        
        ofVec3f spiralForce;
        float spiralRadius = sqrt(particles[i].x * particles[i].x + particles[i].z * particles[i].z);
        if(spiralRadius > 0) {
            float spiralAngle = atan2(particles[i].z, particles[i].x) + time * 0.5;
            spiralForce.x = cos(spiralAngle) * 0.1;
            spiralForce.z = sin(spiralAngle) * 0.1;
            spiralForce.y = sin(time * 0.3 + i * 0.1) * 0.05;
        }
        
        ofVec3f attractionForce;
        for(int j = 0; j < min(5, (int)particles.size()); j++) {
            if(i != j) {
                ofVec3f diff = particles[j] - particles[i];
                float dist = diff.length();
                if(dist > 0 && dist < 100) {
                    diff.normalize();
                    float strength = ofMap(dist, 0, 100, 0.2, 0.01);
                    attractionForce += diff * strength;
                }
            }
        }
        
        ofVec3f centerAttraction = -particles[i] * 0.001;
        
        velocities[i] += (noise * noiseStrength + waveForce + spiralForce + attractionForce + centerAttraction) * ofGetLastFrameTime();
        velocities[i] *= 0.97;
        
        particles[i] += velocities[i];
        
        if(particles[i].x > 300 || particles[i].x < -300) velocities[i].x *= -0.8;
        if(particles[i].y > 300 || particles[i].y < -300) velocities[i].y *= -0.8;
        if(particles[i].z > 300 || particles[i].z < -300) velocities[i].z *= -0.8;
        
        float rotSpeed = 1.0 + sin(time * 0.3 + i * 0.05) * 0.5;
        rotationAngles[i] += rotSpeed;
        
        float colorWave1 = sin(time * 0.8 + i * 0.1) * 0.5 + 0.5;
        float colorWave2 = cos(time * 0.6 + particles[i].y * 0.01) * 0.3 + 0.7;
        float t = (colorWave1 + colorWave2) * 0.5;
        particleColors[i] = getColorFromPalette(t);
    }
}

//--------------------------------------------------------------
void ofApp::drawParticles(){

    
    for(int i = 0; i < particles.size(); i++){
        ofColor glowColor = particleColors[i];
        
        ofPushMatrix();
        ofTranslate(particles[i]);
        
        float pulse1 = sin(time * 2.5 + i * 0.3) * 0.3;
        float pulse2 = cos(time * 4.0 + particles[i].x * 0.01) * 0.2;
        float pulse3 = sin(time * 1.8 + particles[i].y * 0.008) * 0.25;
        float pulseSize = sizes[i] * (1.0 + pulse1 + pulse2 + pulse3);
        
        float wobbleX = sin(time * 1.2 + i * 0.2) * 2.0;
        float wobbleY = cos(time * 0.9 + i * 0.15) * 1.5;
        float wobbleZ = sin(time * 1.5 + i * 0.25) * 1.8;
        ofTranslate(wobbleX, wobbleY, wobbleZ);
        
        ofSetColor(glowColor, 60);
        if(shapeTypes[i] == 0) {
            drawCrystal(pulseSize * 2.0, rotationAngles[i], rotationAxis[i]);
        } else if(shapeTypes[i] == 1) {
            drawStar(pulseSize * 2.0, rotationAngles[i]);
        } else {
            drawDiamond(pulseSize * 2.0, rotationAngles[i], rotationAxis[i]);
        }
        
        ofSetColor(glowColor, 120);
        if(shapeTypes[i] == 0) {
            drawCrystal(pulseSize * 1.2, rotationAngles[i] * 1.3, rotationAxis[i]);
        } else if(shapeTypes[i] == 1) {
            drawStar(pulseSize * 1.2, rotationAngles[i] * 1.3);
        } else {
            drawDiamond(pulseSize * 1.2, rotationAngles[i] * 1.3, rotationAxis[i]);
        }
        
        ofSetColor(glowColor, 255);
        if(shapeTypes[i] == 0) {
            drawCrystal(pulseSize * 0.6, rotationAngles[i] * 1.8, rotationAxis[i]);
        } else if(shapeTypes[i] == 1) {
            drawStar(pulseSize * 0.6, rotationAngles[i] * 1.8);
        } else {
            drawDiamond(pulseSize * 0.6, rotationAngles[i] * 1.8, rotationAxis[i]);
        }
        
        ofPopMatrix();
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetColor(palette[5], 60);
    for(int i = 0; i < particles.size(); i += 2){
        for(int j = i + 1; j < min(i + 3, (int)particles.size()); j++){
            float distance = particles[i].distance(particles[j]);
            if(distance < 60){
                float alpha = ofMap(distance, 0, 60, 120, 20);
                ofSetColor(palette[6], alpha);
                ofDrawLine(particles[i], particles[j]);
            }
        }
    }
    
    for(int i = 0; i < trailPoints.size(); i++){
        float trailAlpha = ofMap(i, 0, trailPoints.size() - 1, 10, 80);
        ofSetColor(trailColors[i], trailAlpha);
        ofPushMatrix();
        ofTranslate(trailPoints[i]);
        ofDrawSphere(0, 0, 0, 1.5);
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
ofColor ofApp::getColorFromPalette(float t){
    t = ofClamp(t, 0, 1);
    float scaledT = t * (palette.size() - 1);
    int index1 = (int)scaledT;
    int index2 = min(index1 + 1, (int)palette.size() - 1);
    float blend = scaledT - index1;
    
    return palette[index1].getLerped(palette[index2], blend);
}

//--------------------------------------------------------------
void ofApp::drawCrystal(float size, float rotation, ofVec3f axis){
    ofPushMatrix();
    ofRotateDeg(rotation, axis.x, axis.y, axis.z);
    
    ofMesh crystal;
    crystal.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofVec3f top(0, size, 0);
    ofVec3f bottom(0, -size, 0);
    
    int segments = 8;
    float radius = size * 0.6;
    
    for(int i = 0; i < segments; i++){
        float angle1 = (i * TWO_PI) / segments;
        float angle2 = ((i + 1) * TWO_PI) / segments;
        
        ofVec3f p1(cos(angle1) * radius, 0, sin(angle1) * radius);
        ofVec3f p2(cos(angle2) * radius, 0, sin(angle2) * radius);
        
        crystal.addVertex(top);
        crystal.addVertex(p1);
        crystal.addVertex(p2);
        
        crystal.addVertex(bottom);
        crystal.addVertex(p2);
        crystal.addVertex(p1);
    }
    
    crystal.draw();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawStar(float size, float rotation){
    ofPushMatrix();
    ofRotateDeg(rotation, 0, 0, 1);
    
    int points = 6;
    float outerRadius = size;
    float innerRadius = size * 0.4;
    
    ofPath star;
    star.setStrokeWidth(2);
    star.setFilled(true);
    
    for(int i = 0; i <= points * 2; i++){
        float angle = (i * TWO_PI) / (points * 2);
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        
        if(i == 0) star.moveTo(x, y);
        else star.lineTo(x, y);
    }
    star.close();
    star.draw();
    
    ofRotateDeg(90, 1, 0, 0);
    star.draw();
    
    ofRotateDeg(90, 0, 1, 0);
    star.draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawDiamond(float size, float rotation, ofVec3f axis){
    ofPushMatrix();
    ofRotateDeg(rotation, axis.x, axis.y, axis.z);
    
    ofMesh diamond;
    diamond.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofVec3f vertices[] = {
        ofVec3f(0, size, 0),        // top
        ofVec3f(size * 0.7, 0, 0),  // right
        ofVec3f(0, 0, size * 0.7),  // front
        ofVec3f(-size * 0.7, 0, 0), // left
        ofVec3f(0, 0, -size * 0.7), // back
        ofVec3f(0, -size, 0)        // bottom
    };
    
    int faces[][3] = {
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 1},
        {5, 2, 1}, {5, 3, 2}, {5, 4, 3}, {5, 1, 4}
    };
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 3; j++){
            diamond.addVertex(vertices[faces[i][j]]);
        }
    }
    
    diamond.draw();
    ofPopMatrix();
}
