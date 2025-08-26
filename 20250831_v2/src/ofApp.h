#pragma once
#include "ofMain.h"
#include "ofxFluid.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void mouseDragged(int x, int y, int button) override;
    void windowResized(int w, int h) override;

private:
    // Fluid
    ofxFluid fluid;
    float    scale = 1.0f;

    struct Emitter {
        float seedA, seedB;
        float speedA, speedB;
        ofFloatColor color;
    };
    std::vector<Emitter> emitters;

    std::vector<ofFloatColor> palette;

    // Artistic params (tuned to avoid “blown white”)
    float forceStrength = 2.4f;   // softer impulses (was 4.0)
    float radiusNorm    = 0.012f; // smaller dye radius (was 0.016)
    float alphaInk      = 0.35f;  // translucent dye
    float brightnessMin = 0.50f;  // subtle breathing
    float brightnessMax = 0.90f;

    bool  paused   = false;
    bool  showInfo = true;
    bool  invert   = false;

    // Mouse helpers
    ofVec2f lastMouse;
    bool    mouseInited = false;

    // helpers
    void addInkForce(const ofVec2f& pos, const ofVec2f& vel, const ofFloatColor& col, float radPx);
    void initPalette();
    void initEmitters(int count);

    // optional gamma to reduce mid/high luminance (prevents “burn”)
    ofFloatColor gamma(const ofFloatColor& c, float g) const {
        return ofFloatColor(powf(c.r, g), powf(c.g, g), powf(c.b, g), c.a);
    }
};
