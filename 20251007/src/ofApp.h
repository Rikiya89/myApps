#pragma once

#include "ofMain.h"

class Particle {
public:
    ofVec3f position;
    ofVec3f velocity;
    float size;
    float rotation;
    float rotationSpeed;

    Particle() {
        position = ofVec3f(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
        size = ofRandom(5, 20);
        rotation = ofRandom(0, 360);
        rotationSpeed = ofRandom(-2, 2);
    }

    void update() {
        position += velocity;
        rotation += rotationSpeed;

        // Wrap around the space
        if (position.x > 400) position.x = -400;
        if (position.x < -400) position.x = 400;
        if (position.y > 400) position.y = -400;
        if (position.y < -400) position.y = 400;
        if (position.z > 400) position.z = -400;
        if (position.z < -400) position.z = 400;
    }
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

    private:
        ofEasyCam cam;
        vector<Particle> particles;
        ofMesh mesh;
        float time;
        ofLight light;
};
