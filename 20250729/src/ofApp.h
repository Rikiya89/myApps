#pragma once

#include "ofMain.h"

struct TextParticle {
    ofVec2f position;
    ofVec2f targetPosition;
    ofVec2f velocity;
    ofVec2f originalPosition;
    float life;
    float maxLife;
    float alpha;
    float size;
    float rotation;
    float rotationSpeed;
    char character;
    bool isActive;
    float animationPhase;
    float scale;
    float targetScale;
    ofVec2f offset;
    float springForce;
    float damping;
    int characterIndex;
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
		
		void createTextParticles(string text);
		void animateTextIn();
		void animateTextOut();
		void explodeText();
		void reformText();
		void spiralAnimation();
		void waveAnimation();
		void morphAnimation();
		float easeInOutCubic(float t);
		float easeOutElastic(float t);
		float easeInOutBack(float t);
		
		vector<TextParticle> textParticles;
		vector<ofVec2f> particleTrails;
		ofTrueTypeFont font;
		string currentText;
		string inputText;
		string customText;
		vector<string> instagramTexts;
		int currentTextIndex;
		bool isUsingCustomText;
		float time;
		float animationTime;
		bool isAnimating;
		bool showTrails;
		bool isInputMode;
		ofFbo fbo;
		ofFbo trailFbo;
		int animationMode;
		float mouseInfluenceRadius;
		
		enum AnimationState {
			IDLE,
			FORMING,
			DISPLAYING,
			EXPLODING,
			REFORMING,
			SPIRALING,
			WAVING,
			MORPHING
		} animationState;
};
