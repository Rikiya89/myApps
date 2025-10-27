#pragma once

#include "ofMain.h"
#include <memory>
#include <array>

struct Particle {
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f rotation;
    ofVec3f rotationSpeed;
    float scale;
    float life;
    float age;
    float maxAge;
    int shapeType;
    ofColor baseColor;
    ofVec3f birthPosition;
    float energy;
    bool isDead;
    float sparklePhase;
    float breathingPhase;
};

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;

private:
    static constexpr int MAX_PARTICLES = 300;
    static constexpr int NUM_BACKGROUND_SHAPES = 30;
    static constexpr float WORLD_SIZE = 500.0f;
    static constexpr float CONNECTION_DISTANCE = 150.0f;
    
    void initializeColorPalettes();
    void initializeParticles();
    void updateParticles();
    void drawConnections();
    void drawParticles();
    void drawBackgroundShapes();
    void drawParticleTrails();
    void updateCamera();
    void drawStarField();
    void drawGalaxyEffects();
    void setupPostProcessing();
    void applyBloomEffect();
    void drawAtmosphericEffects();
    void updateParticleLife();
    void spawnNewParticles();
    
    std::array<ofColor, 12> primaryColors;
    std::array<ofColor, 8> accentColors;
    std::vector<Particle> particles;
    std::vector<std::vector<ofVec3f>> particleTrails;
    
    ofEasyCam cam;
    float globalTime;
    float wavePhase;
    float energyLevel;
    bool showTrails;
    bool mouseDragging;
    bool cameraStabilized;
    bool autoOrbit;
    
    ofVec3f mouseForce;
    ofVec3f worldCenter;
    ofVec3f targetCameraPos;
    ofVec3f smoothCameraPos;
    float cameraDistance;
    float targetCameraDistance;
    float cameraOrbitSpeed;
    ofVec2f cameraAngles;
    ofVec2f targetCameraAngles;
    float cameraSmoothing;
    
    ofMesh connectionMesh;
    ofLight ambientLight;
    ofLight pointLight;
    ofLight secondaryLight;
    
    ofFbo mainFbo;
    ofFbo bloomFbo;
    ofShader bloomShader;
    ofShader particleShader;
    
    std::vector<ofVec3f> starField;
    std::vector<float> starBrightness;
    std::vector<ofColor> galaxyColors;
    
    float bloomIntensity;
    float atmosphericDensity;
    bool enableBloom;
    bool enableStarField;
    bool enableGalaxyMode;
    
    ofSoundPlayer ambientSound;
    float musicReactivity;
};
