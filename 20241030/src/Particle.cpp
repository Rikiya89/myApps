#include "Particle.h"

// Initialize properties as before
Particle::Particle() {
    origin = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
    position = origin;
    velocity = ofVec2f(ofRandom(-2, 2), ofRandom(-2, 2));
    acceleration = ofVec2f(0, 0);
    baseSize = ofRandom(3, 8);
    lifespan = 255;

    color = ofColor(0, 255, 255);
    angle = ofRandom(TWO_PI);

    maxForce = 0.05;
    maxSpeed = 2;
}

void Particle::update(const ofVec2f& attractor, float time, bool repel) {
    // Call flocking behavior before other updates
    flock(vector<Particle>());  // Pass a vector as required by the method signature

    // Circular motion
    angle += 0.02;
    ofVec2f waveOffset(sin(angle) * 2, cos(angle) * 2);
    position += waveOffset;

    ofVec2f centerDirection = origin - position;
    centerDirection.normalize();
    centerDirection *= 0.05;
    acceleration += centerDirection;

    // Attraction/repulsion to mouse
    ofVec2f direction = repel ? position - attractor : attractor - position;
    direction.normalize();
    direction *= repel ? 0.2 : 0.05;
    acceleration += direction;

    velocity += acceleration;
    position += velocity;
    acceleration *= 0;
    lifespan -= 1.0;

    // Color change based on distance to attractor
    float distToMouse = position.distance(attractor);
    color = ofColor::fromHsb(fmin(255, distToMouse), 255, lifespan);

    baseSize = ofMap(sin(ofGetElapsedTimef() * 3), -1, 1, 3, 8);
}

// Flocking behavior, including alignment and cohesion
void Particle::flock(const vector<Particle>& particles) {
    ofVec2f alignment = align(particles);
    ofVec2f cohesion = this->cohesion(particles); // Ensure `this->` to call method correctly

    acceleration += alignment;
    acceleration += cohesion;
}

// Align particles within perception radius
ofVec2f Particle::align(const vector<Particle>& particles) {
    float perceptionRadius = 50;
    ofVec2f steering;
    int total = 0;

    for (const auto& other : particles) {
        float d = position.distance(other.position);
        if (other.position != position && d < perceptionRadius) {
            steering += other.velocity;
            total++;
        }
    }
    if (total > 0) {
        steering /= total;
        steering = steering.getNormalized() * maxSpeed - velocity;
        steering.limit(maxForce);
    }
    return steering;
}

// Cohesion - move towards the average position of nearby particles
ofVec2f Particle::cohesion(const vector<Particle>& particles) {
    float perceptionRadius = 50;
    ofVec2f steering;
    int total = 0;

    for (const auto& other : particles) {
        float d = position.distance(other.position);
        if (other.position != position && d < perceptionRadius) {
            steering += other.position;
            total++;
        }
    }
    if (total > 0) {
        steering /= total;
        steering -= position;
        steering = steering.getNormalized() * maxSpeed - velocity;
        steering.limit(maxForce);
    }
    return steering;
}

void Particle::draw() {
    ofSetColor(color, lifespan);
    ofDrawCircle(position, baseSize);

    // Optional glow effect
    for (int i = 0; i < 3; i++) {
        ofSetColor(color, (lifespan / (i + 1)) * 0.3);
        ofDrawCircle(position, baseSize + i * 2);
    }
}
