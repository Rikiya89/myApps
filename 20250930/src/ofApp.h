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

		vector<ofVec2f> particles;
		vector<ofVec2f> velocities;
		vector<float> particleSizes;
		vector<ofPolyline> trails;
		vector<float> particlePhases;
		vector<ofColor> particleColors;
		float time;
		ofFbo fbo;
		float breathePulse;

		void drawSacredGeometry();
		void drawFlowerOfLife(float x, float y, float radius, int petals);
		void drawMetatronsCube(float x, float y, float radius);
		void drawSacredGeometryAtPosition(float x, float y, float scale);
		void drawTorusField(float x, float y, float radius, float scale);
};
