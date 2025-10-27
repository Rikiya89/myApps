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
		struct FlowField {
			vector<ofVec3f> vectors;
			int cols, rows, depth;
			float scale;
		};
		
		struct Particle {
			ofVec3f position;
			ofVec3f velocity;
			ofVec3f acceleration;
			float size;
			float age;
			float maxAge;
			bool alive;
			vector<ofVec3f> trail;
		};
		
		struct Wave {
			float frequency;
			float amplitude;
			float phase;
			ofVec3f direction;
		};
		
		vector<Particle> particles;
		vector<Wave> waves;
		FlowField flowField;
		
		ofEasyCam cam;
		float time;
		int numParticles;
		float noiseScale;
		float cameraRotationSpeed;
		bool autoRotateCamera;
		bool showFlowField;
		bool showConnections;
		
		bool useAdditiveBlend;
		vector<ofVec3f> starField;
		int numStars;
		float globalRotateSpeed;
		float fogNear, fogFar;
		
		void createFlowField();
		void updateFlowField();
		void createParticles();
		void updateParticles();
		void drawParticles();
		void drawFlowField();
		void drawConnections();
		void drawWaveInterference();
		void drawGeometricPatterns();
		ofVec3f getFlowFieldVector(ofVec3f pos);
		void initializePalette();
		void drawCrystal(float size, float rotation, ofVec3f axis);
		void drawStar(float size, float rotation);
		void drawDiamond(float size, float rotation, ofVec3f axis);
		ofColor getColorFromPalette(float t);
		void initStarField();
		void drawStarField();
		float fogAlpha(const ofVec3f& p) const;
};
