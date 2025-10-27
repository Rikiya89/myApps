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
        
        // 3D objects and rendering
        ofEasyCam cam;
        ofLight light;
        
        // Meshes for different mathematical surfaces
        ofMesh mesh1, mesh2, mesh3;
        
        // Color palette
        vector<ofColor> colors;
        
        // Animation
        float time;
        
        // Helper functions
        void setupColors();
        void setupMeshes();
        void createParametricSurface(ofMesh& mesh, int type);
        void drawParticles();
        void drawInfo();
        
        // Mathematical surface calculations
        ofVec3f calculateKleinBottle(float u, float v, float scale);
        ofVec3f calculateTorusKnot(float u, float v, float scale);
        ofVec3f calculateMobiusStrip(float u, float v, float scale);
};
