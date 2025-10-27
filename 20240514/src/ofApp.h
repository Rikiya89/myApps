#pragma once

#include "ofMain.h"

class Particle {
public:
    ofPoint position;
    ofPoint velocity;
    ofColor color;
    float radius;
    float speed;

    Particle(float x, float y, float radius, ofColor color) {
        position.set(x, y);
        this->radius = radius;
        this->color = color;
        speed = ofRandom(0.5, 2.0);
        velocity.set(ofRandom(-1, 1), ofRandom(-1, 1));
    }

    void update(float time) {
        float noise = ofNoise(position.x * 0.01, position.y * 0.01, time * 0.1) * TWO_PI;
        velocity.x = cos(noise) * speed;
        velocity.y = sin(noise) * speed;
        position += velocity;
        radius = ofMap(ofNoise(position.x * 0.01, position.y * 0.01, time * 0.2), 0, 1, 1, 5);
        color.setHueAngle(fmod(color.getHueAngle() + 0.1, 360)); // Gradually change the hue
    }

    void draw() {
        ofSetColor(color, 200); // Set alpha for slight transparency
        ofDrawCircle(position, radius);
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    vector<Particle> particles;
    float time;
    ofFbo fbo; // Framebuffer object for blending effects
    ofColor bgColor1, bgColor2; // Background colors for gradient
    ofPoint mousePos;
};
