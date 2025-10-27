// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Canvas setup
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(64);
    
    // Initialize variables
    time = 0;
    rotationSpeed = 0.5;
    pulseSpeed = 2.0;
    
    // Visual parameters
    numLayers = 6;
    pointsPerLayer = 6;
    baseRadius = 100;
    maxRadius = 300;
    
    // Colors - deep blue/purple theme
    bgColor = ofColor(10, 10, 25);
    primaryColor = ofColor(100, 150, 255);
    secondaryColor = ofColor(255, 100, 200);
    
    mouseInfluence = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    
    // Smooth mouse influence
    float targetInfluence = ofMap(mousePos.distance(ofVec2f(ofGetWidth()/2, ofGetHeight()/2)),
                                  0, ofGetWidth()/2, 1, 0, true);
    mouseInfluence = ofLerp(mouseInfluence, targetInfluence, 0.1);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Background with subtle gradient
    ofBackgroundGradient(bgColor, ofColor(20, 20, 40), OF_GRADIENT_CIRCULAR);
    
    // Center of canvas
    float centerX = ofGetWidth() / 2;
    float centerY = ofGetHeight() / 2;
    
    // Draw multiple layers of sacred geometry
    for(int layer = 0; layer < numLayers; layer++){
        float layerPct = (float)layer / (numLayers - 1);
        float radius = ofLerp(baseRadius, maxRadius, layerPct);
        
        // Pulsing effect
        float pulse = sin(time * pulseSpeed + layer * 0.5) * 0.1 + 1.0;
        radius *= pulse;
        
        // Mouse influence on radius
        radius *= (1.0 + mouseInfluence * 0.3);
        
        // Rotation based on layer and time
        float rotation = time * rotationSpeed * (layer % 2 == 0 ? 1 : -1);
        rotation += layer * PI / pointsPerLayer;
        
        // Color interpolation
        ofColor layerColor = lerpColor(primaryColor, secondaryColor, layerPct);
        layerColor.a = ofMap(layerPct, 0, 1, 200, 50);
        
        // Draw sacred geometry pattern
        ofPushMatrix();
        ofTranslate(centerX, centerY);
        
        // Main geometry
        drawSacredGeometry(0, 0, radius, pointsPerLayer + layer, rotation);
        
        // Flower of life overlay
        if(layer % 2 == 0){
            ofSetColor(layerColor, 100);
            drawFlower(0, 0, radius * 0.5, 6);
        }
        
        ofPopMatrix();
    }
    
    // Central mandala
    ofPushMatrix();
    ofTranslate(centerX, centerY);
    ofRotate(time * 20);
    
    // Inner circle
    ofSetColor(255, 255, 255, 80);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawCircle(0, 0, baseRadius * 0.3);
    
    // Star pattern
    ofFill();
    for(int i = 0; i < 12; i++){
        float angle = (TWO_PI / 12) * i;
        float x = cos(angle) * baseRadius * 0.2;
        float y = sin(angle) * baseRadius * 0.2;
        
        ofSetColor(255, 255, 255, 150);
        ofDrawCircle(x, y, 3);
    }
    
    ofPopMatrix();
    
    // Particle effects around mouse
    if(mouseInfluence > 0.1){
        ofSeedRandom(0);
        for(int i = 0; i < 20; i++){
            float angle = ofRandom(TWO_PI);
            float dist = ofRandom(20, 100) * mouseInfluence;
            float x = mousePos.x + cos(angle) * dist;
            float y = mousePos.y + sin(angle) * dist;
            float size = ofRandom(1, 3) * mouseInfluence;
            
            ofSetColor(primaryColor, 100 * mouseInfluence);
            ofDrawCircle(x, y, size);
        }
        ofSeedRandom(0); // Reset random seed
    }
}

//--------------------------------------------------------------
void ofApp::drawSacredGeometry(float x, float y, float radius, int points, float rotation){
    vector<ofVec2f> vertices;
    
    // Calculate vertices
    for(int i = 0; i < points; i++){
        float angle = (TWO_PI / points) * i + rotation;
        float vx = x + cos(angle) * radius;
        float vy = y + sin(angle) * radius;
        vertices.push_back(ofVec2f(vx, vy));
    }
    
    // Draw connections between all vertices
    ofSetLineWidth(1);
    for(int i = 0; i < vertices.size(); i++){
        for(int j = i + 1; j < vertices.size(); j++){
            float distance = vertices[i].distance(vertices[j]);
            float alpha = ofMap(distance, 0, radius * 2, 255, 0, true);
            
            ofColor lineColor = lerpColor(primaryColor, secondaryColor,
                                        (float)i / vertices.size());
            ofSetColor(lineColor, alpha * 0.5);
            ofDrawLine(vertices[i], vertices[j]);
        }
    }
    
    // Draw vertices
    ofFill();
    for(int i = 0; i < vertices.size(); i++){
        float sizePulse = sin(time * 3 + i) * 2 + 5;
        ofSetColor(255, 255, 255, 200);
        ofDrawCircle(vertices[i], sizePulse);
    }
}

//--------------------------------------------------------------
void ofApp::drawFlower(float x, float y, float radius, int petals){
    ofNoFill();
    ofSetLineWidth(1.5);
    
    // Draw overlapping circles
    for(int i = 0; i < petals; i++){
        float angle = (TWO_PI / petals) * i;
        float cx = x + cos(angle) * radius * 0.5;
        float cy = y + sin(angle) * radius * 0.5;
        ofDrawCircle(cx, cy, radius * 0.5);
    }
    
    // Center circle
    ofDrawCircle(x, y, radius * 0.5);
}

//--------------------------------------------------------------
ofColor ofApp::lerpColor(ofColor c1, ofColor c2, float amt){
    return ofColor(
        ofLerp(c1.r, c2.r, amt),
        ofLerp(c1.g, c2.g, amt),
        ofLerp(c1.b, c2.b, amt),
        ofLerp(c1.a, c2.a, amt)
    );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's' || key == 'S'){
        // Save screenshot
        ofImage screenshot;
        screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screenshot.save("generative_art_" + ofGetTimestampString() + ".png");
        cout << "Screenshot saved!" << endl;
    }
    else if(key == 'r' || key == 'R'){
        // Reset time for new variation
        time = ofRandom(1000);
    }
    else if(key == ' '){
        // Toggle rotation direction
        rotationSpeed *= -1;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    mousePos.set(x, y);
}
