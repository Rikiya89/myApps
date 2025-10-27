#pragma once

#include "ofMain.h"

struct Petal {
    ofVec3f position;
    ofVec3f normal;
    ofColor color;
    float size;
    float angle;
    float curvature;
    float life;
};

struct Flower {
    ofVec3f centerPosition;
    ofVec3f stemDirection;
    vector<Petal> petals;
    ofColor centerColor;
    ofColor petalColor;
    float bloomProgress;
    float rotationSpeed;
    float petalCount;
    float flowerSize;
    float energy;
    int flowerType; // 0=rose, 1=lotus, 2=sunflower, 3=sakura, 4=lily
    float phase;
    float stemLength;
    ofVec3f swayOffset;
};

struct ParticleTrail {
    vector<ofVec3f> positions;
    vector<ofColor> colors;
    vector<float> ages;
    int maxLength;
};

struct GeometricShape {
    ofVec3f position;
    ofVec3f prevPosition;
    ofVec3f rotation;
    ofVec3f scale;
    ofColor color;
    ofColor glowColor;
    float phase;
    float energy;
    int shapeType;
    int attractorType; // 0=Lorenz, 1=Rossler, 2=Chen, 3=Fractal, 4=Flower
    ParticleTrail trail;
};

struct BackgroundParticle {
    ofVec3f position;
    ofVec3f velocity;
    ofColor color;
    float life;
    float size;
};

struct FlowerPollen {
    ofVec3f position;
    ofVec3f velocity;
    ofColor color;
    float life;
    float size;
    ofVec3f targetFlower;
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
        // Enhanced color system with flower palettes
        vector<ofColor> dreamyColors;
        vector<ofColor> auroraColors;
        vector<ofColor> flowerColors;
        vector<ofColor> petalColors;
        void setupDreamyPalette();
        void setupAuroraPalette();
        void setupFlowerPalettes();
        ofColor getDreamyColor(float t);
        ofColor getAuroraColor(float t);
        ofColor getFlowerColor(float t);
        ofColor getPetalColor(float t);
        ofColor getGradientColor(float t, float energy);
        
        // Flower system
        vector<Flower> flowers;
        vector<FlowerPollen> pollen;
        void generateFlowers();
        void updateFlowers();
        void updatePollen();
        void drawFlower(const Flower& flower);
        void drawPetal(const Petal& petal);
        void drawStem(const Flower& flower);
        void drawPollen();
        
        // 3D geometry system with trails
        vector<GeometricShape> shapes;
        vector<BackgroundParticle> backgroundParticles;
        void generateShapes();
        void updateShapes();
        void updateTrails();
        void updateBackgroundParticles();
        void drawShape(const GeometricShape& shape);
        void drawTrail(const ParticleTrail& trail);
        void drawBackgroundParticles();
        void drawGlowEffect(const GeometricShape& shape);
        
        // Flower-based mathematical patterns
        ofVec3f calculateFlowerSpiral(float time, int index);
        ofVec3f calculateGoldenRatio(float time, int index);
        ofVec3f calculateFibonacciSpiral(float time, int index);
        ofVec3f calculatePetalPattern(float time, int index);
        ofVec3f calculateRosePattern(float time, int index);
        
        // Advanced mathematical attractors
        ofVec3f calculateLorenzAttractor(float time, int index);
        ofVec3f calculateRosslerAttractor(float time, int index);
        ofVec3f calculateChenAttractor(float time, int index);
        ofVec3f calculateFractalSpiral(float time, int index);
        ofVec3f calculatePosition(float time, int index, float baseRadius);
        ofVec3f calculateRotation(float time, int index);
        float calculateScale(float time, int index);
        float calculateEnergy(float time, int index);
        
        // Flower generation helpers
        void generatePetals(Flower& flower);
        void updateBloomingAnimation(Flower& flower);
        ofVec3f calculatePetalPosition(const Flower& flower, int petalIndex, float bloomProgress);
        
        // Visual effects
        void setupPostProcessing();
        void beginGlowEffect();
        void endGlowEffect();
        void drawFlowerGlow(const Flower& flower);
        
        // Animation and rendering
        float animationTime;
        float globalEnergy;
        float windStrength;
        ofVec3f windDirection;
        ofEasyCam cam;
        vector<ofLight> lights;
        
        // Framebuffers for effects
        ofFbo mainFbo;
        ofFbo glowFbo;
        
        // Parameters
        int numShapes;
        int numFlowers;
        int numBackgroundParticles;
        int numPollen;
        float baseRadius;
        float animationSpeed;
        float trailLength;
        bool enableLighting;
        bool enableGlow;
        bool enableTrails;
        bool enableBackgroundParticles;
        bool enableFlowers;
        bool enablePollen;
        
        // Visual modes
        int visualMode; // 0=mixed, 1=lorenz, 2=rossler, 3=chen, 4=fractal, 5=flowers
        
        void switchVisualMode(int mode);
};

// Mathematical constants for attractors and flower patterns
struct AttractorParams {
    static constexpr float LORENZ_SIGMA = 10.0f;
    static constexpr float LORENZ_RHO = 28.0f;
    static constexpr float LORENZ_BETA = 8.0f/3.0f;
    
    static constexpr float ROSSLER_A = 0.2f;
    static constexpr float ROSSLER_B = 0.2f;
    static constexpr float ROSSLER_C = 5.7f;
    
    static constexpr float CHEN_A = 5.0f;
    static constexpr float CHEN_B = -10.0f;
    static constexpr float CHEN_C = -0.38f;
    
    // Golden ratio and Fibonacci for flower patterns
    static constexpr float GOLDEN_RATIO = 1.618033988749f;
    static constexpr float GOLDEN_ANGLE = 137.5077640845f; // degrees
};