#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofBackground(8, 88, 79); // #08584f - Deep teal background
    
    // Initialize camera
    cam.setDistance(800);
    cam.setNearClip(1);
    cam.setFarClip(2000);
    
    // Setup color palette
    setupColors();
    
    // Initialize meshes
    setupMeshes();
    
    // Lighting
    light.setPosition(500, 500, 500);
    light.enable();
    
    time = 0;
}

//--------------------------------------------------------------
void ofApp::setupColors(){
    colors.push_back(ofColor::fromHex(0x5b8bdf)); // Sky blue
    colors.push_back(ofColor::fromHex(0x3e79c7)); // Ocean blue
    colors.push_back(ofColor::fromHex(0x08584f)); // Deep teal
    colors.push_back(ofColor::fromHex(0xead3ff)); // Pale lavender
    colors.push_back(ofColor::fromHex(0x102a67)); // Navy blue
    colors.push_back(ofColor::fromHex(0x3c7dd1)); // Bright blue
    colors.push_back(ofColor::fromHex(0x6556bd)); // Purple blue
    colors.push_back(ofColor::fromHex(0x645ebe)); // Periwinkle
    colors.push_back(ofColor::fromHex(0x152d91)); // Royal blue
    colors.push_back(ofColor::fromHex(0x8aa5f6)); // Light blue
}

//--------------------------------------------------------------
void ofApp::setupMeshes(){
    // Create parametric surfaces
    createParametricSurface(mesh1, 0);
    createParametricSurface(mesh2, 1);
    createParametricSurface(mesh3, 2);
}

//--------------------------------------------------------------
void ofApp::createParametricSurface(ofMesh& mesh, int type){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    mesh.clear();
    
    int resolution = 120;
    float scale = 200.0f;
    
    // Generate vertices using different mathematical formulas
    for(int i = 0; i < resolution; i++){
        for(int j = 0; j < resolution; j++){
            float u = ofMap(i, 0, resolution-1, -PI, PI);
            float v = ofMap(j, 0, resolution-1, -PI, PI);
            
            ofVec3f pos;
            ofColor vertexColor;
            
            switch(type){
                case 0: // Klein bottle inspired surface
                    pos = calculateKleinBottle(u, v, scale);
                    vertexColor = colors[i % colors.size()];
                    break;
                case 1: // Torus knot surface
                    pos = calculateTorusKnot(u, v, scale);
                    vertexColor = colors[(i + j) % colors.size()];
                    break;
                case 2: // Möbius strip variation
                    pos = calculateMobiusStrip(u, v, scale);
                    vertexColor = colors[(i * j) % colors.size()];
                    break;
            }
            
            mesh.addVertex(pos);
            mesh.addColor(vertexColor);
            
            // Add triangles
            if(i < resolution-1 && j < resolution-1){
                int index = i * resolution + j;
                
                // Triangle 1
                mesh.addIndex(index);
                mesh.addIndex(index + 1);
                mesh.addIndex(index + resolution);
                
                // Triangle 2
                mesh.addIndex(index + 1);
                mesh.addIndex(index + resolution + 1);
                mesh.addIndex(index + resolution);
            }
        }
    }
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateKleinBottle(float u, float v, float scale){
    // Klein bottle parametric equations with time animation
    float t = time * 0.01f;
    float r = 2 + cos(u * 0.5f + t);
    
    float x = (6 * cos(u) * (1 + sin(u)) + 4 * r * cos(u) * cos(v + t)) * scale * 0.01f;
    float y = (16 * sin(u) + 4 * r * sin(u) * cos(v + t)) * scale * 0.01f;
    float z = 4 * r * sin(v + t) * scale * 0.01f;
    
    return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateTorusKnot(float u, float v, float scale){
    // Torus knot with dynamic parameters
    float t = time * 0.005f;
    float p = 3 + sin(t);
    float q = 2 + cos(t * 0.7f);
    float R = 3 + cos(u * p + t);
    float r = 1 + 0.5f * sin(u * q + t);
    
    float x = (R + r * cos(v)) * cos(u) * scale * 0.1f;
    float y = (R + r * cos(v)) * sin(u) * scale * 0.1f;
    float z = r * sin(v) * scale * 0.1f;
    
    return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateMobiusStrip(float u, float v, float scale){
    // Möbius strip with wave distortion
    float t = time * 0.008f;
    float waveHeight = 0.5f + 0.3f * sin(u * 3 + t);
    
    float x = (1 + v * cos(u * 0.5f) * waveHeight) * cos(u + t) * scale * 0.2f;
    float y = (1 + v * cos(u * 0.5f) * waveHeight) * sin(u + t) * scale * 0.2f;
    float z = v * sin(u * 0.5f) * waveHeight * scale * 0.2f;
    
    return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimeMillis();
    
    // Update meshes with new mathematical calculations
    setupMeshes();
    
    // Rotate camera around the scene
    cam.orbit(0.5f, 0, 800);
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    // Enable smooth shading
    ofEnableSmoothing();
    ofSetLineWidth(1);
    
    // Draw first surface (Klein bottle)
    ofPushMatrix();
    ofTranslate(-300, 0, 0);
    ofRotateYDeg(time * 0.02f);
    mesh1.draw();
    ofPopMatrix();
    
    // Draw second surface (Torus knot)
    ofPushMatrix();
    ofTranslate(0, 0, 0);
    ofRotateXDeg(time * 0.015f);
    ofRotateZDeg(time * 0.01f);
    mesh2.draw();
    ofPopMatrix();
    
    // Draw third surface (Möbius strip)
    ofPushMatrix();
    ofTranslate(300, 0, 0);
    ofRotateYDeg(-time * 0.025f);
    mesh3.draw();
    ofPopMatrix();
    
    // Draw connecting particles
    drawParticles();
    
    cam.end();
    
    // Draw info
    drawInfo();
}

//--------------------------------------------------------------
void ofApp::drawParticles(){
    // Create floating particles that follow mathematical paths
    int numParticles = 50;
    float t = time * 0.001f;
    
    for(int i = 0; i < numParticles; i++){
        float phase = i * TWO_PI / numParticles;
        
        // Lorenz attractor inspired movement
        float x = 100 * sin(t + phase) * cos(t * 1.1f + phase);
        float y = 80 * cos(t * 0.8f + phase) * sin(t * 1.3f + phase);
        float z = 60 * sin(t * 1.2f + phase) * cos(t * 0.9f + phase);
        
        ofSetColor(colors[i % colors.size()], 150);
        ofDrawSphere(x, y, z, 3 + 2 * sin(t + phase));
    }
}

//--------------------------------------------------------------
void ofApp::drawInfo(){
    ofDisableDepthTest();
    ofSetColor(234, 211, 255, 200); // Pale lavender with transparency
    
    string info = "3D Generative Art - Mathematical Surfaces\n";
    info += "Klein Bottle | Torus Knot | Möbius Strip\n";
    info += "Press 'f' for fullscreen | Space to pause";
    
    ofDrawBitmapString(info, 20, 30);
    
    ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    if(key == ' '){
        static bool paused = false;
        paused = !paused;
        if(paused) {
            ofSetFrameRate(0);
        } else {
            ofSetFrameRate(60);
        }
    }
}

//--------------------------------------------------------------
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
