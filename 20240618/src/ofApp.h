#pragma once

#include "ofMain.h"

class Particle {
public:
    ofVec2f position;
    ofVec2f velocity;
    float size;
    ofColor color;

    Particle(ofVec2f pos, ofVec2f vel, float s, ofColor col)
        : position(pos), velocity(vel), size(s), color(col) {}

    void update() {
        position += velocity;
        if (position.x < 0 || position.x > ofGetWidth()) {
            velocity.x *= -1;
        }
        if (position.y < 0 || position.y > ofGetHeight()) {
            velocity.y *= -1;
        }
    }

    void draw() {
        ofSetColor(color);
        ofDrawCircle(position, size);
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);

    vector<Particle> particles;
    int numParticles = 200;
    ofVec2f mousePos;
};
