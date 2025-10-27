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

		// 3D Generative Art System
		ofEasyCam cam;
		ofNode cameraTarget;
		
		// Camera animation
		bool smoothCameraMode = true;
		float cameraOrbitRadius = 400.0f;
		float cameraHeight = 100.0f;
		float cameraSpeed = 0.3f;
		
		// Generative parameters
		bool animate = true;
		float time = 0.0f;
		float timeSpeed = 1.0f;
		bool blackOnWhite = false; // false = white on black, true = black on white
		
		// Enhanced visual modes
		bool showTrails = true;
		bool showBloom = true;
		bool showFractals = true;
		bool showFlowFields = true;
		
		// Trail system
		ofFbo trailFbo;
		void setupTrails();
		void updateTrails();
		
		// 3D Structure generation
		struct Node3D {
			glm::vec3 pos;
			glm::vec3 vel;
			float size;
			float age;
			float maxAge;
			bool isAlive;
			int connections;
		};
		
		std::vector<Node3D> nodes;
		std::vector<std::pair<int, int>> connections;
		
		// Generation parameters
		int maxNodes = 200;
		float nodeSpeed = 50.0f;
		float connectionDistance = 80.0f;
		float spawnRate = 0.8f;
		float nodeLife = 8.0f;
		
		// Visual parameters
		float strokeWeight = 1.0f;
		float nodeSize = 2.0f;
		bool showNodes = true;
		bool showConnections = true;
		
		// 3D Noise field
		float noiseScale = 0.01f;
		float noiseStrength = 20.0f;
		
		// Methods
		void updateNodes();
		void updateConnections();
		void drawNodes();
		void drawConnections();
		void spawnNode();
		void applyNoiseForce(Node3D& node);
		float getAlphaByAge(float age, float maxAge);
		void drawWireframeSphere(glm::vec3 pos, float radius, int resolution = 12);
		void drawGenerativeStructures();
		void drawFloatingCubes();
		void drawDynamicLines();
		void drawParticleField();
		void drawSimpleFractals();
		
		// Enhanced visual effects
		void drawFractalTrees();
		void drawFlowingRibbons();
		void drawSacredGeometry();
		void drawLissajousCurves();
		void drawSphericalHarmonics();
		void drawVoronoiCells();
		void drawFlockingBirds();
		void drawCrystalStructures();
		void drawDNAHelices();
		void drawMandalaFlowers();
		void updateSmoothCamera();
		
		// Organic structures
		struct FlowRibbon {
			std::vector<glm::vec3> points;
			std::vector<float> ages;
			glm::vec3 direction;
			float thickness;
			float life;
			float maxLife;
		};
		std::vector<FlowRibbon> ribbons;
		void updateRibbons();
		
		// Crystal system
		struct Crystal {
			glm::vec3 pos;
			glm::vec3 rotation;
			float scale;
			float growth;
			float maxGrowth;
			int type;
		};
		std::vector<Crystal> crystals;
		void updateCrystals();
		void drawCrystal(const Crystal& crystal);
		
		// Geometric primitives
		void drawFractalBranch(glm::vec3 pos, glm::vec3 dir, float length, int depth, float rotation);
		void drawTetrahedron(float size);
		void drawOctahedron(float size);
		void drawIcosahedron(float size);
		void drawDodecahedron(float size);
		void drawCube(float size);
		
		// Particle system for additional effects
		struct Particle3D {
			glm::vec3 pos;
			glm::vec3 vel;
			float life;
			float maxLife;
			float size;
		};
		std::vector<Particle3D> particles3d;
		void updateParticles3D();
		void drawParticles3D();
		void spawnParticle3D(glm::vec3 position);
};
