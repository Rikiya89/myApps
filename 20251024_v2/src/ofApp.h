#pragma once

#include "ofMain.h"

// Ghost particle structure
struct Ghost {
    ofVec2f pos;
    ofVec2f vel;
    float size;
    float alpha;
    float phase;
    float floatSpeed;
    vector<ofVec2f> trail;
};

// Skull structure
struct Skull {
    ofVec2f pos;
    float rotation;
    float rotationSpeed;
    float size;
    float alpha;
    float bobPhase;
};

// Spider structure
struct Spider {
    ofVec2f pos;
    float threadLength;
    float swingPhase;
    float swingSpeed;
    float size;
};

// Candle structure
struct Candle {
    ofVec2f pos;
    float height;
    float flameFlicker;
    float flickerSpeed;
    float glowIntensity;
};

// Creepy eyes structure
struct CreepyEyes {
    ofVec2f pos;
    float blinkPhase;
    float blinkSpeed;
    bool isBlinking;
    float eyeDistance;
};

// Pentagram structure
struct Pentagram {
    ofVec2f pos;
    float size;
    float rotationPhase;
    float rotationSpeed;
    float glowPhase;
};

// Coffin structure
struct Coffin {
    ofVec2f pos;
    float width;
    float height;
    float shakePhase;
    float rotation;
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

		// Custom methods
		void drawGhost(Ghost& ghost);
		void drawSkull(Skull& skull);
		void drawSpider(Spider& spider);
		void drawMoon();
		void drawBats();
		void drawCandle(Candle& candle);
		void drawCreepyEyes(CreepyEyes& eyes);
		void drawPentagram(Pentagram& pentagram);
		void drawCoffin(Coffin& coffin);
		void drawClouds();

		// Member variables
		vector<Ghost> ghosts;
		vector<Skull> skulls;
		vector<Spider> spiders;
		vector<ofVec2f> batPositions;
		vector<float> batPhases;
		vector<Candle> candles;
		vector<CreepyEyes> creepyEyes;
		vector<Pentagram> pentagrams;
		vector<Coffin> coffins;
		vector<ofVec2f> cloudPositions;
		vector<float> cloudSpeeds;

		float time;
		ofVec2f moonPos;
		float moonGlow;
};
