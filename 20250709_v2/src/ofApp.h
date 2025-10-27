#pragma once

#include "ofMain.h"

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

private:
    struct FlowerParticle {
        ofVec2f position;
        ofVec2f velocity;
        float life;
        float maxLife;
        float size;
        float brightness;
        ofColor color;
        bool isNodeParticle;
    };
    
    struct GeometryNode {
        ofVec2f position;
        float radius;
        float pulsePhase;
        float intensity;
        ofColor color;
    };
    
    vector<FlowerParticle> particles;
    vector<GeometryNode> flowerNodes;
    
    float time;
    float breathingPhase;
    ofVec2f centerPoint;
    
    // Flower of Life parameters
    float baseRadius;
    vector<ofVec2f> flowerCenters;
    
    // Clean visual effects
    ofFbo glowFbo;
    ofFbo particleFbo;
    
    // Elegant color palette
    vector<ofColor> sacredColors;
    
    void setupFlowerOfLife();
    void updateParticles();
    void updateGeometryNodes();
    void addFlowerParticle();
    
    void drawFlowerOfLife();
    void drawGeometryNodes();
    void drawParticles();
    void drawSacredConnections();
    void drawCenterMandala();
    
    ofVec2f getFlowTowardsFlower(ofVec2f pos);
    ofColor getSacredColor(float phase);
    bool isInsideFlowerCircle(ofVec2f pos);
};
