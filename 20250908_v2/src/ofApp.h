#pragma once

#include "ofMain.h"

enum ParticleType {
    PLANKTON,
    BUBBLE,
    SPARK,
    DUST
};

struct Particle {
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofColor color;
    float life;
    float maxLife;
    float size;
    float rotation;
    float rotationSpeed;
    ParticleType type;
    float trailLength;
    vector<ofVec3f> trail;
    float breathePhase;
    float gentleSpeed;
    ofVec3f homePosition;
};

struct CoralNode {
    ofVec3f position;
    vector<ofVec3f> branches;
    ofColor color;
    float size;
    float pulsePhase;
    float organicFactor;
    float breatheIntensity;
    float gentleGlow;
};

struct Jellyfish {
    ofVec3f position;
    ofVec3f velocity;
    float bellSize;
    float tentacleLength;
    ofColor color;
    float pulsePhase;
    vector<ofVec3f> tentacles;
    float breatheSpeed;
    float gentleDrift;
    ofVec3f targetPosition;
};

struct KelpNode {
    ofVec3f position;
    ofVec3f basePosition;
    float swayPhase;
    float height;
    float gentleSway;
    float breatheAmount;
};

struct MagicOrb {
    ofVec3f position;
    ofVec3f velocity;
    ofColor color;
    float size;
    float energy;
    float pulsePhase;
    float orbitRadius;
    float orbitSpeed;
    ofVec3f orbitCenter;
    vector<ofVec3f> energyTrail;
    float connectionDistance;
};

struct LightBeam {
    ofVec3f startPos;
    ofVec3f endPos;
    ofColor color;
    float intensity;
    float width;
    float dancePhase;
    float lifetime;
};

struct Crystal {
    ofVec3f position;
    ofVec3f rotation;
    ofVec3f rotationSpeed;
    float size;
    ofColor color;
    float glowIntensity;
    float breathePhase;
    int faces;
};

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
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
        ofCamera cam;
        vector<Particle> particles;
        vector<CoralNode> corals;
        vector<Jellyfish> jellyfish;
        vector<vector<KelpNode>> kelpFields;
        vector<MagicOrb> magicOrbs;
        vector<LightBeam> lightBeams;
        vector<Crystal> crystals;
        
        ofColor palette[10];
        
        float time;
        ofMesh oceanFloor;
        ofVbo particleVbo;
        
        // Camera animation
        ofVec3f cameraTarget;
        ofVec3f cameraPosition;
        float cameraRadius;
        float cameraHeight;
        float cameraAngle;
        float cameraSpeed;
        float gentleBreathe;
        float smoothEasing;
        
        // God rays
        ofMesh godRayMesh;
        
        void updateParticles();
        void updateCamera();
        void updateJellyfish();
        void updateKelp();
        void updateMagicOrbs();
        void updateLightBeams();
        void updateCrystals();
        
        void drawParticles();
        void drawCorals();
        void drawOceanFloor();
        void drawJellyfish();
        void drawKelp();
        void drawGodRays();
        void drawCaustics();
        void drawMagicOrbs();
        void drawLightBeams();
        void drawCrystals();
        void drawAurora();
        void drawMysticalRings();
        
        void createOceanFloor();
        void generateCorals();
        void generateJellyfish();
        void generateKelp();
        void createGodRays();
        void generateMagicOrbs();
        void generateCrystals();
        
};
