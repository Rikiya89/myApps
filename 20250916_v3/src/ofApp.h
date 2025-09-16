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
		vector<ofColor> themeColors;
		
		struct FlowField {
			vector<ofVec3f> vectors;
			int cols, rows, depth;
			float scale;
		} flowField;
		
		struct Ribbon {
			vector<ofVec3f> points;
			vector<ofColor> colors;
			ofVec3f position;
			ofVec3f velocity;
			float life;
			int maxPoints;
		};
		
		vector<Ribbon> ribbons;
		vector<ofMesh> crystalMeshes;
		vector<ofVec3f> crystalPositions;
		vector<float> crystalRotations;
		vector<float> crystalScales;
		
		ofEasyCam cam;
		float time;
		int numRibbons;
		int numCrystals;
		
		ofLight light;
		bool showWireframe;
		
};
