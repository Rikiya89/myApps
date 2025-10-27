#include "ofApp.h"

// Setup function to load image and initialize particles with increased density
void ofApp::setup(){
    ofSetWindowShape(1280, 720);
    dahlia.load("dahlia.jpg");
    dahlia.resize(1280, 720);
    ofPixels & pixels = dahlia.getPixels();

    // Loop through each pixel with reduced step to create more particles
    for (int y = 0; y < dahlia.getHeight(); y += 4){  // Reduced from 6 to 3
        for (int x = 0; x < dahlia.getWidth(); x += 4){  // Reduced from 6 to 3
            ofColor color = pixels.getColor(x, y);
            if (color.getBrightness() > 100) { // Only take bright pixels
                Particle p;
                p.position = ofVec2f(x, y);
                p.velocity = ofVec2f(0, 0);
                p.color = color;
                p.size = ofRandom(1.0, 2.5); // Adjusted size for denser particles
                p.opacity = ofRandom(80, 150); // Adjusted opacity range for visual clarity
                particles.push_back(p);
            }
        }
    }
    ofSetBackgroundColor(0);
}

// Update function to modify particle positions
void ofApp::update(){
    for (auto & p : particles) {
        // Use Perlin noise to set the angle of particle movement
        float angle = ofNoise(p.position.x * noiseScale, p.position.y * noiseScale, ofGetElapsedTimef() * 0.1) * TWO_PI;
        
        // Convert angle to a velocity vector
        ofVec2f direction = ofVec2f(cos(angle), sin(angle));
        p.velocity += direction * 0.1;  // Acceleration factor
        p.velocity.limit(1.2); // Limit max speed for smoother motion

        // Update position based on velocity
        p.position += p.velocity * speed;

        // Wrap-around screen edges for seamless looping
        if (p.position.x < 0) p.position.x = ofGetWidth();
        if (p.position.x > ofGetWidth()) p.position.x = 0;
        if (p.position.y < 0) p.position.y = ofGetHeight();
        if (p.position.y > ofGetHeight()) p.position.y = 0;

        // Adjust particle opacity for fading effect
        p.opacity = ofClamp(p.opacity + ofRandom(-1, 1), 80, 150); // Random opacity flicker
    }
}

// Draw function to render particles
void ofApp::draw(){
    for (auto & p : particles) {
        ofSetColor(p.color, p.opacity); // Use dynamic opacity
        ofDrawCircle(p.position, p.size); // Draw each particle with varying size
    }
}
