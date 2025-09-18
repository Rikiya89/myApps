#pragma once

#include "ofMain.h"

struct Particle {
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f noiseOffset;
    float size;
    float alpha;
    int type;
    ofVec3f rotation;
    ofVec3f rotationSpeed;
    int colorIndex;
    float pulsePhase;
    float pulseSpeed;
    vector<ofVec3f> trail;
    float energy;
    float targetEnergy;
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
		
		void drawOverlay();
		void saveScreenshot();
		
		vector<Particle> particles;
		ofEasyCam cam;
		float time;
		float cameraRadius;
		float cameraHeight;
		int numParticles;
		float connectionDistance;
		ofVec3f worldCenter;
		vector<ofColor> colorPalette;
		ofFbo fbo;
		ofShader blurShader;
		bool useShader;
		float globalPulse;
		vector<ofVec3f> attractors;
		
		ofTrueTypeFont titleFont;
		ofTrueTypeFont captionFont;
		bool showOverlay;
		string currentCaption;
		string currentTitle;
		bool saveNextFrame;
		int screenshotCounter;
		
};
