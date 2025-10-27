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
		ofEasyCam camera;
		vector<ofColor> palette;
		vector<ofVec3f> particles;
		vector<ofVec3f> velocities;
		vector<float> particleSizes;
		vector<ofColor> particleColors;
		vector<float> particleLife;
		vector<float> particleMaxLife;
		float time;
		int numParticles;
		ofMesh mesh;
		float noiseScale;
		float waveAmplitude;
		
		// Additional visual elements
		vector<ofVec3f> flowerPositions;
		vector<float> flowerRotations;
		vector<ofColor> flowerColors;
		ofMesh ribbonMesh;
		ofFbo fbo;
		ofShader blurShader;
		
		// Flowing structures
		vector<vector<ofVec3f>> flowingPaths;
		vector<ofColor> pathColors;
		int numPaths;
		
		// Light system
		vector<ofVec3f> lightPositions;
		vector<ofColor> lightColors;
		vector<float> lightIntensities;
		
};
