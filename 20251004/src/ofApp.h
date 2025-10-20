#pragma once

#include "ofMain.h"

struct Particle {
	ofVec2f pos;
	ofVec2f vel;
	float radius;
	float angle;
	float angleVel;
	float life;
	float phase;
	int type;
	ofColor color;
	float energy;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<Particle> particles;
		float time;
		int patternMode;
};
