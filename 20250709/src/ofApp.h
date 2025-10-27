// ofApp.h
#pragma once
#include "ofMain.h"

class Particle {
public:
    ofVec3f pos;
    ofVec3f vel;
    ofVec3f acc;
    float lifespan;
    float maxLife;
    ofColor color;
    float size;
    float rotation;
    float rotSpeed;
    vector<ofVec3f> trail;
    int maxTrailSize;
    
    Particle(float x, float y, float z = 0) {
        pos.set(x, y, z);
        vel.set(ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-1, 1));
        acc.set(0, 0, 0);
        maxLife = ofRandom(200, 400);
        lifespan = maxLife;
        size = ofRandom(1, 4);
        rotation = ofRandom(TWO_PI);
        rotSpeed = ofRandom(-0.1, 0.1);
        maxTrailSize = 20;
    }
    
    void applyForce(ofVec3f force) {
        acc += force;
    }
    
    void update() {
        vel += acc;
        vel.limit(6);
        pos += vel;
        acc *= 0;
        lifespan -= 0.8;
        rotation += rotSpeed;
        
        // Add to trail
        trail.push_back(pos);
        if (trail.size() > maxTrailSize) {
            trail.erase(trail.begin());
        }
        
        // Wrap around edges smoothly
        if (pos.x < -50) pos.x = ofGetWidth() + 50;
        if (pos.x > ofGetWidth() + 50) pos.x = -50;
        if (pos.y < -50) pos.y = ofGetHeight() + 50;
        if (pos.y > ofGetHeight() + 50) pos.y = -50;
    }
    
    void display() {
        float alpha = ofMap(lifespan, 0, maxLife, 0, 255);
        
        // Draw trail
        if (trail.size() > 1) {
            ofNoFill();
            for (int i = 1; i < trail.size(); i++) {
                float trailAlpha = alpha * (float(i) / trail.size()) * 0.5;
                ofSetColor(color, trailAlpha);
                ofSetLineWidth(size * (float(i) / trail.size()));
                ofDrawLine(trail[i-1].x, trail[i-1].y, trail[i].x, trail[i].y);
            }
            ofFill();
        }
        
        // Draw particle with glow effect
        for (int i = 3; i > 0; i--) {
            float glowAlpha = alpha / (i * 2);
            ofSetColor(color, glowAlpha);
            ofDrawCircle(pos.x, pos.y, size * i * (lifespan / maxLife));
        }
        
        // Draw core
        ofSetColor(255, alpha);
        ofDrawCircle(pos.x, pos.y, size * 0.5);
    }
    
    bool isDead() {
        return lifespan < 0.0;
    }
};

class FlowField {
public:
    vector<vector<ofVec3f>> field;
    int cols, rows;
    float scale;
    float zoff;
    
    FlowField(int w, int h, float s) {
        scale = s;
        cols = w / scale;
        rows = h / scale;
        field.resize(cols, vector<ofVec3f>(rows));
        zoff = 0;
    }
    
    void update(float noiseScale, float time) {
        float xoff = 0;
        for (int i = 0; i < cols; i++) {
            float yoff = 0;
            for (int j = 0; j < rows; j++) {
                float angle = ofNoise(xoff, yoff, zoff) * TWO_PI * 4;
                field[i][j].set(cos(angle), sin(angle), 0);
                yoff += noiseScale;
            }
            xoff += noiseScale;
        }
        zoff += 0.003;
    }
    
    ofVec3f lookup(ofVec3f position) {
        int col = ofClamp(position.x / scale, 0, cols - 1);
        int row = ofClamp(position.y / scale, 0, rows - 1);
        return field[col][row];
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
private:
    vector<Particle> particles;
    FlowField* flowField;
    ofVec3f attractorPos;
    float time;
    bool showTrails;
    bool showField;
    bool isPaused;
    ofFbo fbo;
    ofFbo glowFbo;
    int colorMode;
    float backgroundOpacity;
    ofShader blurShader;
    
    // Visual parameters
    float noiseScale;
    float noiseStrength;
    float attractorStrength;
    float particleSpawnRate;
    
    void createParticleBurst(float x, float y, int count);
    ofColor getParticleColor(Particle& p);
};
