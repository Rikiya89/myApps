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
		struct Ribbon {
			float angle = 0.0f;
			float radius = 0.0f;
			float height = 0.0f;
			float twist = 0.0f;
			float width = 0.0f;
			float noiseOffset = 0.0f;
			ofFloatColor colorA;
			ofFloatColor colorB;
		};

		struct PortalLayer {
			float radius = 0.0f;
			float thickness = 0.0f;
			float speed = 0.0f;
			float noiseOffset = 0.0f;
			ofFloatColor color;
		};

		struct FogLayer {
			glm::vec3 basePos = glm::vec3(0.0f);
			float radius = 0.0f;
			float speed = 0.0f;
			float noiseOffset = 0.0f;
			ofFloatColor color;
		};

		ofFloatColor hexToColor(const std::string& hex);
		void buildPalette();
		void ensureFbos();
		void renderScene(float time);
		void drawBackgroundGradient();
		void drawRibbons(float time);
		void drawPortal(float time);
		void drawSparks(float time);
		void drawFogLayers(float time);

		ofEasyCam cam;
		ofFbo sceneFbo;
		ofFbo trailFbo;
		std::vector<ofFloatColor> palette;
		std::vector<Ribbon> ribbons;
		std::vector<PortalLayer> portalLayers;
		std::vector<FogLayer> fogLayers;
		std::vector<glm::vec3> sparks;
		float noiseSeed = 0.0f;
};
