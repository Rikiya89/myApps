#pragma once

#include "ofMain.h"

class Particle {
public:
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    float lifespan;
    float maxLife;
    ofColor color;
    float size;
    float noiseOffset;
    float phase; // For oscillating effects
    float glowIntensity;
    
    Particle();
    void update(float time);
    void applyForce(ofVec3f force);
    bool isDead();
    void draw();
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
    
private:
    // Camera
    ofEasyCam cam;
    
    // Particles
    vector<Particle> particles;
    int maxParticles;
    
    // Visual parameters
    float time;
    float rotationSpeed;
    ofVec3f attractorPos;
    float attractorStrength;
    
    // Mesh for efficient rendering
    ofMesh particleMesh;
    ofVboMesh ribbonMesh;
    
    // Colors - expanded palette
    ofColor color1, color2, color3, color4, color5;
    vector<ofColor> colorPalette;
    
    // Lighting
    ofLight light1, light2, light3;
    
    // Post-processing
    ofFbo fbo;
    ofFbo glowFbo;
    bool useFbo;
    
    // Visual effects
    vector<ofVec3f> attractorTrail;
    int maxTrailLength;
    
    // Shader for glow effect
    ofShader glowShader;
    
    // Helper methods
    void addParticle();
    ofVec3f calculateAttraction(ofVec3f pos);
    ofColor interpolateColor(float t);
    void drawRibbons();
    void drawGlowingSpheres();
    void updateColorPalette(float time);
};
