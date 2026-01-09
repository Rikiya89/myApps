#pragma once

#include "ofMain.h"

struct Particle {
	glm::vec3 pos;
	glm::vec3 vel;
	float size;
	float alpha;
	float life;
	float maxLife;
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

		// Fibonacci sequence
		vector<float> fibonacci;
		void generateFibonacci(int count);

		// 3D elements
		ofEasyCam cam;

		// Golden ratio constants
		float phi;
		float goldenAngle;

		// Animation
		float time;

		// Drawing methods
		void drawGoldenSpiral();
		void drawPhyllotaxis();
		void drawFibonacciTower();
		void drawFloatingParticles();
		void drawConnectionWeb();

		// Particles
		vector<Particle> particles;
		void initParticles(int count);
		void updateParticles();

		// FBO for glow effect
		ofFbo fbo;
		ofFbo blurFbo;

		// Smooth camera rotation
		float camAngle;
		float camHeight;
};
