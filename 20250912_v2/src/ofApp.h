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
		struct GridPoint {
			ofVec3f position;
			ofVec3f velocity;
			float phase;
			float amplitude;
			float brightness;
			ofVec3f normal;
		};
		
		struct Particle {
			ofVec3f position;
			ofVec3f velocity;
			float life;
			float maxLife;
			float size;
			float alpha;
		};
		
		vector<vector<vector<GridPoint>>> grid;
		vector<Particle> particles;
		ofEasyCam cam;
		ofLight pointLight;
		ofMaterial material;
		
		int gridSizeX, gridSizeY, gridSizeZ;
		float spacing;
		float time;
		float noiseScale;
		float waveSpeed;
		float morphFactor;
		int performanceLevel;
		
		void updateGrid();
		void updateParticles();
		void drawGrid();
		void drawSecretarialPatterns();
		void drawFractalPatterns();
		void drawRibbons();
		void drawParticles();
		void spawnParticles();
		ofVec3f getSecretarialPattern(int x, int y, int z, float t);
		ofVec3f calculateNormal(int x, int y, int z);
		
};
