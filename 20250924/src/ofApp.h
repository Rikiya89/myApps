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
        vector<ofColor> colorPalette;
        vector<ofVec3f> particles;
        vector<ofVec3f> velocities;
        vector<float> sizes;
        vector<ofColor> particleColors;

        // Fractal trees
        struct FractalBranch {
            ofVec3f start, end;
            float thickness;
            int generation;
            ofColor color;
        };
        vector<FractalBranch> fractalBranches;

        // Wave terrain
        ofMesh terrainMesh;
        vector<vector<float>> terrainHeights;
        int terrainSize;

        // Galaxy spirals
        struct GalaxyPoint {
            ofVec3f position;
            ofVec3f velocity;
            float age;
            ofColor color;
            float size;
        };
        vector<GalaxyPoint> galaxyPoints;

        // Visual effects
        ofFbo mainFbo, blurFbo;
        ofShader blurShader;

        ofEasyCam cam;

        // Cinematic camera system
        struct CameraKeyframe {
            ofVec3f position;
            ofVec3f target;
            float duration;
            float easeType; // 0=linear, 1=ease-in-out, 2=ease-in, 3=ease-out
        };
        vector<CameraKeyframe> cameraKeyframes;
        int currentKeyframe;
        float keyframeProgress;
        bool cinematicMode;
        ofVec3f currentCamPos, currentCamTarget;
        float cameraShakeIntensity;
        bool enableOrganicMovement;

        float time;
        int numParticles;
        float colorShiftSpeed;

        void initializeParticles();
        void updateParticles();
        void drawGenerativeShapes();
        void generateFractalTree(ofVec3f start, ofVec3f direction, float length, float thickness, int generation, int maxGen);
        void drawFractalTrees();
        void updateTerrain();
        void drawTerrain();
        void updateGalaxy();
        void drawGalaxy();
        void setupShaders();
        void setupCameraSequence();
        void updateCinematicCamera();
        float easeInOutCubic(float t);
        float easeInCubic(float t);
        float easeOutCubic(float t);
        ofVec3f getCameraShake();
        ofColor getRandomPaletteColor();
        ofColor getInterpolatedColor(float t);
};