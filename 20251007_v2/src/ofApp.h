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
    struct Particle {
        ofVec3f position;
        ofVec3f velocity;
        float size;
        float rotation;
        float rotationSpeed;
        float life;
        float phase;
        int type;
    };

    struct RingSystem {
        float radius;
        float rotation;
        float speed;
        int numPoints;
        float waveOffset;
    };

    vector<Particle> particles;
    vector<RingSystem> rings;
    ofEasyCam cam;
    float time;
    int maxParticles;
    float pulseAmount;
    float breatheAmount;

    void addParticle();
    void drawGeometricShapes();
    void drawFlowingLines();
    void drawPulsingSpheres();
};
