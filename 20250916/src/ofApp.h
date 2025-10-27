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
		// Each particle traces a noise-driven ribbon across the canvas.
		struct FlowParticle {
			glm::vec2 position{};
			glm::vec2 previousPosition{};
			glm::vec2 velocity{};
			float noiseOffset = 0.f;
			float speedBias = 0.f;
			float width = 1.f;
			float symmetryBias = 0.f;
		};

		struct SacredLayer {
			std::vector<glm::vec2> unitVertices;
			float radius = 1.f;
			float rotation = 0.f;
			float rotationSpeed = 0.f;
			float lineWeight = 1.f;
			float radialWeight = 0.6f;
			float brightness = 0.3f;
			int starStep = 0;
			bool drawRadials = false;
			bool drawInnerCircle = false;
			float innerCircleRatio = 0.5f;
		};

		struct OrbitalTrail {
			float radius = 0.f;
			float angle = 0.f;
			float speed = 0.f;
			float thickness = 1.f;
			float modulation = 0.f;
			float length = 0.8f;
			float brightness = 0.4f;
			float noiseOffset = 0.f;
		};

		struct HarmonicRing {
			float radius = 0.f;
			float weight = 1.f;
			float brightness = 0.4f;
			float rotation = 0.f;
			float rotationSpeed = 0.f;
			float pulseOffset = 0.f;
			int petals = 0;
			bool dotted = false;
		};

		struct LightRay {
			float angle = 0.f;
			float speed = 0.f;
			float offset = 0.f;
			float length = 0.f;
			float thickness = 1.f;
			float radiusInner = 0.f;
			float radiusOuter = 0.f;
			float brightness = 0.3f;
			float noiseOffset = 0.f;
		};

		void resetParticles();
		void clearTrails();
		void drawParticleTrail(const FlowParticle &particle, float time);
		void buildVignetteMesh(int width, int height);
		void buildGlowMesh(int width, int height);
		void drawSoftGlow(float time);
		void buildSacredGeometry(int width, int height);
		void updateSacredGeometry(float deltaTime);
		void drawSacredGeometry(float time);
		void buildOrbitalTrails(int width, int height);
		void updateOrbitalTrails(float deltaTime);
		void drawOrbitalTrails(float time);
		void buildHarmonicRings(int width, int height);
		void updateHarmonicRings(float deltaTime);
		void drawHarmonicRings(float time);
		void buildRadiantCore(int width, int height);
		void drawRadiantCore(float time);
		void buildLightRays(int width, int height);
		void updateLightRays(float deltaTime);
		void drawLightRays(float time);
		void drawBloomEcho(float time);

		ofFbo trailFbo;
		std::vector<FlowParticle> particles;
		std::vector<SacredLayer> sacredLayers;
		std::vector<OrbitalTrail> orbitalTrails;
		std::vector<HarmonicRing> harmonicRings;
		std::vector<LightRay> lightRays;
		float flowScale = 0.0012f;
		float timeScale = 0.05f;
		float baseStep = 1.3f;
		float swirlStrength = 0.18f;
		float fadeStrength = 14.f;
		float velocityLerp = 0.085f;
		float wrapMargin = 24.f;
		ofMesh vignetteMesh;
		ofMesh glowMesh;
		float geometryBaseAlpha = 0.17f;
		float geometryPulse = 0.38f;
		float orbitBaseAlpha = 0.18f;
		float compositeZoomStrength = 0.028f;
		float compositeDriftStrength = 16.f;
		float ringAttractorStrength = 0.18f;
		std::vector<glm::vec2> corePetalDirections;
		std::vector<float> coreSparklePhase;
		float lastUpdateTime = 0.f;
};
