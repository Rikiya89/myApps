#pragma once

#include "ofMain.h"

struct Particle {
    ofVec2f position;
    ofVec2f velocity;
    float life;
    float maxLife;
    ofColor color;
    float size;
    float rotation;
    float rotationSpeed;
};

struct FlowerPattern {
    ofVec2f position;
    float radius;
    float rotation;
    float rotationSpeed;
    float scalePhase;
    float alpha;
    int colorIndex;
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
private:
    void drawFlowerOfLife(float centerX, float centerY, float radius, int depth, float rotation);
    void drawCircle(float x, float y, float radius, float alpha);
    void updateParticles();
    void drawParticles();
    void addParticle(ofVec2f pos, ofColor color);
    void updateFlowerPatterns();
    void drawFlowerPatterns();
    
    float goldenRatio;
    float time;
    vector<ofColor> colorPalette;
    int currentColorIndex;
    float colorTransition;
    float breathingPhase;
    float pulseIntensity;
    float masterRotation;
    
    vector<Particle> particles;
    vector<FlowerPattern> flowerPatterns;
    int maxParticles;
    
    float rotationSpeed;
    float scaleOscillation;
    float centerPulse;
};