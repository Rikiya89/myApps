#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofSetCircleResolution(60);
    
    // Allocate FBOs with alpha channels
    mainFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    mainFbo.begin();
    ofClear(0, 0, 0, 255);
    mainFbo.end();
    
    bloomFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    bloomFbo.begin();
    ofClear(0, 0, 0, 255);
    bloomFbo.end();
    
    // Initialize particles with swirling properties and animated parameters
    for (int i = 0; i < numParticles; i++){
        // Random initial radius and angle for swirling motion
        float r = ofRandom(50, ofGetWidth()/2);
        float a = ofRandom(TWO_PI);
        radii.push_back(r);
        angles.push_back(a);
        
        // Calculate initial position relative to the center
        ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
        ofVec2f pos = center + ofVec2f(r * cos(a), r * sin(a));
        positions.push_back(pos);
        
        noiseSeeds.push_back(ofRandom(1000));
        
        // Soothing pastel hues from soft blue to purple
        float hue = ofRandom(160, 240);
        colors.push_back(ofColor::fromHsb(hue, 150, 255, 200));
        
        // Initialize additional animation parameters
        rotations.push_back(ofRandom(360));      // Starting rotation angle in degrees
        scaleOffsets.push_back(ofRandom(0, TWO_PI)); // Phase offset for scaling oscillation
    }
}

void ofApp::update(){
    time += 0.01;
    gradientOffset += 0.001;
    ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
    
    // Update swirling motion, color, rotation, and scaling for each particle
    for (int i = 0; i < numParticles; i++){
        float noiseVal = ofNoise(noiseSeeds[i], time);
        angles[i] += 0.01 + noiseVal * 0.05;
        radii[i] += ofRandom(-0.3, 0.3);
        radii[i] = ofClamp(radii[i], 30, ofGetWidth()/2);
        positions[i] = center + ofVec2f(radii[i] * cos(angles[i]), radii[i] * sin(angles[i]));
        
        // Gradually shift hue within a calming range
        int hue = colors[i].getHue();
        hue = (hue + 0.1) > 240 ? 160 : hue + 0.1;
        colors[i].setHue(hue);
        
        // Animate additional rotation and scaling
        rotations[i] += 0.5; // Rotate the shape slowly
        // The scale oscillates between 0.9 and 1.1
        scaleOffsets[i] += 0.02;
    }
    
    // Draw into mainFbo with feedback trails and evolving gradient
    mainFbo.begin();
    
    // Draw a semi-transparent, dynamic gradient overlay to softly fade previous frames
    for (int y = 0; y < ofGetHeight(); y += 5) {
        float pct = ofMap(y, 0, ofGetHeight(), 0, 1);
        ofColor top(10, 5, 20);
        ofColor bottom(5, 0, 10);
        ofColor lineColor = top.getLerped(bottom, pct + sin(gradientOffset) * 0.05);
        ofSetColor(lineColor, 10);
        ofDrawRectangle(0, y, ofGetWidth(), 5);
    }
    
    // Draw complex shapes with additive blending for luminous trails
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < numParticles; i++){
        // Compute a scale factor that oscillates over time
        float scaleFactor = 1.0 + 0.1 * sin(scaleOffsets[i]);
        drawComplexShape(positions[i], 15, colors[i], rotations[i], scaleFactor);
    }
    ofDisableBlendMode();
    
    mainFbo.end();
    
    // Create bloom effect by drawing the mainFbo texture with slight offsets
    bloomFbo.begin();
    ofClear(0, 0, 0, 255);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    // Offset drawing creates a soft glow around the complex shapes
    for (int offsetX = -2; offsetX <= 2; offsetX++){
        for (int offsetY = -2; offsetY <= 2; offsetY++){
            ofSetColor(255, 50);
            mainFbo.draw(offsetX, offsetY);
        }
    }
    ofDisableBlendMode();
    bloomFbo.end();
}

void ofApp::draw(){
    // Draw the main FBO and overlay the bloom effect for a radiant glow
    mainFbo.draw(0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    bloomFbo.draw(0, 0);
    ofDisableBlendMode();
}

// Helper function to draw a dynamic star shape with noise deformation, rotation, and scaling
void ofApp::drawComplexShape(const ofVec2f &pos, float radius, const ofColor &col, float rotation, float scale) {
    int numPoints = 10;   // Total number of vertices for the star shape
    float innerRadius = radius * 0.5;
    
    ofPushMatrix();
    ofTranslate(pos);
    // Apply animated rotation and scaling
    ofRotateDeg(rotation);
    ofScale(scale, scale);
    
    ofSetColor(col);
    ofFill();
    ofBeginShape();
    for (int i = 0; i < numPoints; i++){
        // Alternate between outer and inner vertices
        float currentRadius = (i % 2 == 0) ? radius : innerRadius;
        // Add noise-based deformation to the radius
        float noiseOffset = ofNoise(i * 0.1, ofGetElapsedTimef() * 0.5) * 5.0;
        currentRadius += noiseOffset;
        
        float angle = ofMap(i, 0, numPoints, 0, TWO_PI);
        float x = currentRadius * cos(angle);
        float y = currentRadius * sin(angle);
        ofVertex(x, y);
    }
    ofEndShape(true);
    ofPopMatrix();
}
