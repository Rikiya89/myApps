#pragma once

#include "ofMain.h"
#include <random>

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
        enum PatternType { Lissajous = 0, Rose = 1, Hypotrochoid = 2, Superformula = 3, Truchet = 4 };

        struct CellParams {
            // Common
            PatternType type = Lissajous;
            float weight = 1;   // stroke weight
            float phase = 0;    // phase offset for animation / noise
            float angleDeg = 0; // rotation per cell
            bool invert = false; // invert cell background vs global

            // Lissajous
            int a = 1;        // X frequency
            int b = 1;        // Y frequency

            // Rose (r = R*cos(k*theta)); k = kNum/kDen
            int kNum = 2;
            int kDen = 3;

            // Hypotrochoid params
            int ht_R = 5;     // big radius (integer)
            int ht_r = 3;     // small radius (integer)
            float ht_d = 3.f; // pen offset

            // Superformula params
            float sf_m = 5.f;
            float sf_n1 = 0.3f, sf_n2 = 1.7f, sf_n3 = 1.7f;
            float sf_a = 1.f, sf_b = 1.f;
        };

        // Grid config (2:3 aspect -> 6x9 grid)
        int cols = 6;
        int rows = 9;
        float margin = 40.f; // outer margin
        std::vector<CellParams> cells; // size rows*cols

        // Draw toggles / params
        bool showGrid = false;
        bool drawSacred = true;
        bool animate = true;
        int curveSamples = 600; // points per curve
        float timeScale = 0.6f; // animation speed

        // Visual FX
        bool invertGlobal = false; // white bg, black strokes
        bool noiseWarp = true;     // Perlin warp lines
        float noiseAmp = 6.0f;     // pixels
        float noiseScale = 0.006f; // spatial frequency
        float noiseTime = 0.25f;   // temporal frequency
        bool weaveOverlay = true;    // grid wire overlay
        bool weaveAnimate = false;   // steady grid wires by default
        bool weaveStraight = true;   // draw straight grid lines (not wavy)

        // Recording (image sequence)
        bool recording = false;
        int  recFrame = 0;
        float recFps = 60.f;
        std::string recDir;

        // Random state
        uint32_t seed = 1;
        std::mt19937 rng;

        // Helpers
        void regenerate(uint32_t newSeed);
        void drawGridLines(float x, float y, float cw, float ch);
        void drawSacredGeometryOverlay(float cx, float cy, float baseR);
        void drawWeaveOverlay(float x, float y, float gridW, float gridH, float cw, float ch, float t, const ofColor& c);

        // Patterns
        void drawPatternInCell(int ci, int cj, float x, float y, float cw, float ch, const CellParams& p, float t, const ofColor& stroke);
        void drawLissajousInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly);
        void drawRoseInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly);
        void drawHypotrochoidInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly);
        void drawSuperformulaInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly);
        void drawTruchetInCell(float cw, float ch, int ci, int cj, const CellParams& p, float t);

        // Warp
        glm::vec2 warpPoint(const glm::vec2& pt, float t, float phase) const;

        // Recording helpers
        void startRecording();
        void stopRecording();
};
