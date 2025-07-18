#pragma once

#include "ofMain.h"

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef TWO_PI
#define TWO_PI 6.28318530718
#endif

struct Particle {
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f acceleration;
    float life;
    float maxLife;
    float size;
    ofColor color;
    float energy;
    int chakraType;
    float auraRadius;
    float rotationAngle;
    
    Particle(ofVec2f pos, ofVec2f vel, ofColor col, float lifetime, int chakra = 0);
    void update();
    void draw();
    bool isDead();
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
    void windowResized(int w, int h);
    
private:
    vector<Particle> particles;
    vector<ofVec2f> chakraPoints;
    vector<ofColor> chakraColors;
    
    float time;
    float breathingCycle;
    float meditationPhase;
    float cosmicRotation;
    
    // Spiritual parameters
    int maxParticles;
    float energyFlow;
    float harmonyLevel;
    bool showSacredGeometry;
    bool showChakras;
    bool breathingMode;
    bool showLotus;
    bool showMandala;
    bool showAura;
    
    void spawnSpiritualParticles();
    void updateBreathing();
    ofColor getChakraColor(int chakra);
    void drawSacredGeometry();
    void drawChakras();
    void drawEnergyFlow();
    void drawFlowerOfLife();
    void drawMandala();
    void drawLotusPattern();
    void drawSriYantra();
    void drawCosmicBackground();
    void drawAuraField();
    void drawChakraLotus(ofVec2f center, ofColor color, int petals, float size);
};
