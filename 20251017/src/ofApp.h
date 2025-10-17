#pragma once

#include "ofMain.h"

struct Particle {
    ofVec3f position;
    ofVec3f velocity;
    ofColor color;
    float size;
    float noiseOffsetX;
    float noiseOffsetY;
    float noiseOffsetZ;
    float life;
    float rotationSpeed;
    deque<ofVec3f> trail;
};

struct GeometricShape {
    ofVec3f position;
    ofVec3f rotation;
    ofVec3f rotationSpeed;
    float size;
    ofColor color;
    int type; // 0: box, 1: sphere, 2: cone, 3: icosphere
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // 3D generative art variables
    vector<Particle> particles;
    vector<GeometricShape> shapes;
    vector<ofColor> palette;
    ofEasyCam cam;
    float time;
    float rotationAngle;
    int numParticles;

    ofLight pointLight;
    ofLight ambientLight;
    ofMaterial material;

    // Flow field for particle movement
    vector<vector<vector<ofVec3f>>> flowField;
    int flowFieldResolution;

    void initializePalette();
    void createParticles();
    void createGeometricShapes();
    void createFlowField();
    void drawFlowRibbons();
    void drawEnergyRings();
    void drawCrystalStructure();
};
