#pragma once

#include "ofMain.h"

class Particle {
public:
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f acc;
    float lifespan;
    float maxLife;
    float size;
    
    Particle(float x, float y);
    void update();
    void applyForce(ofVec2f force);
    void draw();
    bool isDead();
};

class ofApp : public ofBaseApp{
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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    // Particle system
    vector<Particle> particles;
    int maxParticles;
    
    // Geometric patterns
    float angleOffset;
    float radiusOffset;
    int numShapes;
    
    // Animation variables
    float time;
    float noiseScale;
    float noiseSpeed;
    
    // Visual modes
    int visualMode;
    bool showParticles;
    bool showGeometry;
    bool invertColors;
    
    // Frame buffer for trails
    ofFbo fbo;
    
    // Helper functions
    void addParticle(float x, float y);
    void drawGeometricPattern();
    void drawNoiseField();
    void drawRadialBurst();
    void clearBackground();
};
