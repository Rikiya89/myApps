#pragma once
#include "ofMain.h"

class Particle {
public:
    Particle();
    void update(const ofVec2f& attractor, float time, bool repel);
    void flock(const vector<Particle>& particles); // Flocking behavior
    void draw();

    ofVec2f position, velocity, acceleration;
    float lifespan;
    ofColor color;
    float baseSize;

private:
    float angle;
    ofVec2f origin;
    ofVec2f align(const vector<Particle>& particles);
    ofVec2f cohesion(const vector<Particle>& particles);
    float maxForce;
    float maxSpeed;
};
