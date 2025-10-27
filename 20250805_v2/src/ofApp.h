#pragma once

#include "ofMain.h"

struct Particle {
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofColor color;
    float life;
    float maxLife;
    float size;
    float phase;
    float swayAmount;
    ofVec3f originalPos;
};

struct BioTree {
    ofVec3f position;
    float height;
    float pulseSpeed;
    float phase;
    float swayPhase;
    float swayAmount;
    float growthPhase;
    ofColor baseColor;
    vector<ofVec3f> branches;
    vector<float> branchPhases;
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
		
		// 3D scene methods
		void setupScene();
		void updateParticles();
		void updateTrees();
		void drawParticles();
		void drawTrees();
		void drawGroundGlow();
		
		// Pandora color palette
		ofColor getPandoraColor(int type);
		
		// Scene objects
		vector<Particle> particles;
		vector<BioTree> bioTrees;
		
		// Scene parameters
		ofEasyCam cam;
		float time;
		ofLight light;
		
		// Animation parameters
		bool useAutoCam;
		ofVec3f cameraTarget;
		float cameraRadius;
		float cameraAngle;
		float masterPulse;
		
		// Pandora colors
		ofColor biolumBlue;
		ofColor biolumPurple;
		ofColor biolumTeal;
		ofColor biolumGreen;
		ofColor biolumViolet;
};
