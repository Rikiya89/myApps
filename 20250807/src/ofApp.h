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
		vector<ofColor> palette;
		vector<ofVec3f> particles;
		vector<ofVec3f> velocities;
		vector<float> sizes;
		vector<ofColor> particleColors;
		vector<int> shapeTypes;
		vector<float> rotationAngles;
		vector<ofVec3f> rotationAxis;
		
		ofEasyCam cam;
		float time;
		int numParticles;
		
		vector<ofVec3f> trailPoints;
		vector<ofColor> trailColors;
		float cameraRotationSpeed;
		bool autoRotateCamera;
		
		bool useAdditiveBlend;
		vector<ofVec3f> starField;
		int numStars;
		float globalRotateSpeed;
		float fogNear, fogFar;
		
		void initializePalette();
		void createParticles();
		void updateParticles();
		void drawParticles();
		void drawCrystal(float size, float rotation, ofVec3f axis);
		void drawStar(float size, float rotation);
		void drawDiamond(float size, float rotation, ofVec3f axis);
		ofColor getColorFromPalette(float t);
		void initStarField();
		void drawStarField();
		float fogAlpha(const ofVec3f& p) const;
};
