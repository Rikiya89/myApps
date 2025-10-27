#pragma once

#include "ofMain.h"

struct Particle {
    ofVec3f pos;
    ofVec3f vel;
    ofColor color;
    float size;
    float life;
    float maxLife;
    bool isBioluminescent;
    float pulseOffset;
    int trailIndex;
};

struct FloatingIsland {
    ofVec3f pos;
    vector<ofVec3f> vertices;
    ofColor color;
    float rotation;
    float rotSpeed;
    float scale;
    float bobOffset;
    vector<ofVec3f> crystalPositions;
    vector<float> crystalSizes;
};

struct Tendril {
    vector<ofVec3f> points;
    ofColor color;
    float thickness;
    float phase;
    float speed;
    vector<ofVec3f> leafPositions;
};

struct EnergyOrb {
    ofVec3f pos;
    ofVec3f orbitCenter;
    float orbitRadius;
    float orbitSpeed;
    float orbitAngle;
    ofColor color;
    float size;
    float pulsePhase;
};

struct JellyCreature {
    ofVec3f pos;
    ofVec3f vel;
    vector<ofVec3f> tentacles;
    ofColor color;
    float size;
    float pulsePhase;
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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    // Pandora color palette
    vector<ofColor> palette;

    // Camera
    ofEasyCam cam;
    float cameraRotation;

    // Camera animation
    enum CameraMode {
        ORBIT,
        FLY_THROUGH,
        ZOOM_IN,
        SPIRAL,
        FOCUS_ELEMENT
    };
    CameraMode currentCameraMode;
    float cameraModeTime;
    float cameraModeDuration;
    ofVec3f cameraTarget;
    int focusedElementIndex;

    // Particles (bioluminescent spores)
    vector<Particle> particles;
    int maxParticles;

    // Floating islands
    vector<FloatingIsland> islands;

    // Tendrils (glowing vines)
    vector<Tendril> tendrils;

    // Energy orbs
    vector<EnergyOrb> energyOrbs;

    // Jelly creatures
    vector<JellyCreature> jellyCreatures;

    // Particle trails
    vector<vector<ofVec3f>> particleTrails;

    // Time
    float time;

    // Lighting
    ofLight mainLight;
    ofLight rimLight;

    // Methods
    void createParticle(ofVec3f position);
    void createIsland(ofVec3f position);
    void createTendril(ofVec3f start, ofVec3f end);
    void createEnergyOrb(ofVec3f center);
    void createJellyCreature(ofVec3f position);
    void drawParticles();
    void drawIslands();
    void drawTendrils();
    void drawAtmosphere();
    void drawEnergyOrbs();
    void drawJellyCreatures();
    void drawEnvironmentalEffects();
    void updateCamera();
    void switchCameraMode();
};
