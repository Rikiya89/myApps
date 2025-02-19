#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0); // Black background for contrast
    ofEnableBlendMode(OF_BLENDMODE_ADD); // Additive blending for glowing effects
    
    box2d.init();
    box2d.setGravity(0, 0); // Disable gravity for Perlin noise control
    box2d.createBounds(); // Create boundaries for the canvas
    box2d.setFPS(60.0); // Smooth animation at 60 FPS

    int numParticles = 120;
    for (int i = 0; i < numParticles; i++) {
        auto circle = make_shared<ofxBox2dCircle>();
        circle->setPhysics(1.0, 0.5, 0.1); // density, bounce, friction
        circle->setup(box2d.getWorld(), ofGetWidth() / 2, ofGetHeight() / 2, ofRandom(3, 6));
        particles.push_back(circle);

        noiseOffsets.push_back(ofVec2f(ofRandom(0, 1000), ofRandom(0, 1000)));
    }

    baseColor = ofColor::fromHsb(ofRandom(0, 255), 80, 200); // Base color for soft pastels

    // Set up slightly different rotation speeds for each mandala layer
    layerRotationSpeeds[0] = 0.01;
    layerRotationSpeeds[1] = 0.015;
    layerRotationSpeeds[2] = 0.02;
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    for (int i = 0; i < particles.size(); i++) {
        float noiseValueX = ofNoise(noiseOffsets[i].x) * 2 - 1;
        float noiseValueY = ofNoise(noiseOffsets[i].y) * 2 - 1;

        float angle = i * TWO_PI / numSides + ofGetElapsedTimef() * 0.1;
        float radialX = cos(angle) * noiseStrength * noiseValueX;
        float radialY = sin(angle) * noiseStrength * noiseValueY;

        ofVec2f noiseForce(radialX, radialY);

        particles[i]->addForce(noiseForce, 1.0);

        noiseOffsets[i].x += noiseScale;
        noiseOffsets[i].y += noiseScale;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw a radial gradient background for spiritual depth
    ofColor color1 = ofColor::fromHsb((int(baseColor.getHue() + ofGetElapsedTimef() * 5)) % 255, 80, 50);
    ofColor color2 = ofColor::fromHsb((int(baseColor.getHue() + ofGetElapsedTimef() * 5 + 128)) % 255, 80, 20);
    ofBackgroundGradient(color1, color2, OF_GRADIENT_CIRCULAR);
    
    // Subtle transparency modulation for breathing effect
    ofSetColor(0, 0, 0, 10 + 10 * sin(ofGetElapsedTimef() * 0.5));
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

    for (int i = 0; i < particles.size(); i++) {
        float colorHue = fmod(baseColor.getHue() + sin(ofGetElapsedTimef() * 0.3) * 30, 255);
        ofColor particleColor = ofColor::fromHsb(colorHue, 80, 255);
        particleColor.a = 150;
        ofSetColor(particleColor);

        // Draw multi-layered rotating mandala patterns
        for (int layer = 0; layer < 3; layer++) {
            for (int j = 0; j < numSides; j++) {
                float rotationAngle = j * TWO_PI / numSides + ofGetElapsedTimef() * layerRotationSpeeds[layer];
                float radiusMod = 1 + 0.1 * sin(ofGetElapsedTimef() + i * 0.2); // Pulse effect

                float rotatedX = particles[i]->getPosition().x * cos(rotationAngle) - particles[i]->getPosition().y * sin(rotationAngle);
                float rotatedY = particles[i]->getPosition().x * sin(rotationAngle) + particles[i]->getPosition().y * cos(rotationAngle);

                ofPushMatrix();
                ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
                ofDrawCircle(rotatedX * radiusMod, rotatedY * radiusMod, particles[i]->getRadius());
                ofPopMatrix();
            }
        }
    }
}
