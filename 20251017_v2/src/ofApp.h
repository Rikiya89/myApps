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
			struct CellPattern {
				int type = 0;
				float rotation = 0.0f;
				float rotationSpeed = 0.0f;
				float noiseOffset = 0.0f;
				float scale = 1.0f;
				float detail = 0.0f;
			};

		void drawMandalaPattern(const CellPattern& pattern, float size, float t) const;
		void drawTrianglePattern(const CellPattern& pattern, float size, float t) const;
		void drawFlowerPattern(const CellPattern& pattern, float size, float t) const;
		void drawLatticePattern(const CellPattern& pattern, float size, float t) const;
		void drawEquilateralTriangle(float radius) const;
		void regeneratePatterns();

		std::vector<CellPattern> patterns;
			int cols = 9;
			int rows = 16;
	};
