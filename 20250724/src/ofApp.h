#pragma once

#include "ofMain.h"

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
		ofEasyCam cam;
		vector<ofVec3f> particles;
		vector<ofColor> particleColors;
		vector<float> particleRotations;
		vector<float> particleScales;
		vector<vector<ofVec3f>> particleTrails;
		vector<ofVec3f> wavePoints;
		vector<float> attractorForces;
		vector<ofVec3f> chakraPositions;
		vector<ofColor> chakraColors;
		vector<ofVec3f> flowerOfLifePoints;
		vector<ofVec3f> merkabaPyramid;
		vector<ofVec3f> mandalaPoints;
		vector<float> goldenSpiral;
		vector<ofVec3f> energyStreams;
		vector<ofVec3f> lightOrbs;
		vector<ofColor> orbColors;
		vector<float> orbSizes;
		vector<ofVec3f> cosmicDust;
		vector<float> particleMorphStates;
		vector<float> particleLifespan;
		vector<ofVec3f> auroraPoints;
		float time;
		int numParticles;
		int numOrbs;
		int numDustParticles;
		float noiseScale;
		float rotationSpeed;
		bool showWireframe;
		bool showTrails;
		bool showWaves;
		bool showChakras;
		bool showSacredGeometry;
		bool showMandala;
		bool showEnergyStreams;
		bool showAurora;
		bool showOrbs;
		bool meditationMode;
		int currentPalette;
		float cameraAutoRotate;
		ofVec3f centerAttractor;
		float attractorStrength;
		float breathingCycle;
		float goldenRatio;
		float morphCycle;
		float auroraCycle;
		
};
