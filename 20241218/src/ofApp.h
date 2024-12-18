// ofApp.h
#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    int resolution = 150; // Reduced grid resolution for lighter load
    float timeStep = 0.02; // Adjust speed of noise animation
    ofColor baseColor = ofColor(255); // Base color for drawing
    float animationSpeed = 0.5; // Speed of animation
    float waveAmplitude = 30; // Reduced amplitude for smoother motion
    float rotationSpeed = 0.02; // Speed of rotational effect
    float pulseFrequency = 0.5; // Frequency of pulse effect
    float gradientSpeed = 0.1; // Speed of gradient shift
};
