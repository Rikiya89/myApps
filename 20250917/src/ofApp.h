#pragma once

#include "ofMain.h"

struct RibbonLayer {
    float baseY = 0.0f;
    float noiseScale = 0.0f;
    float noiseSpeed = 0.0f;
    float noiseOffset = 0.0f;
    float waveAmplitude = 0.0f;
    float thickness = 0.0f;
    ofFloatColor color;
};

struct HaloDot {
    glm::vec2 anchor = {0.0f, 0.0f};
    float radius = 0.0f;
    float noiseOffset = 0.0f;
    float noiseSpeed = 0.0f;
    ofFloatColor color;
};

struct TextureDot {
    glm::vec2 position = {0.0f, 0.0f};
    float size = 0.0f;
    float alpha = 0.0f;
    float noiseOffset = 0.0f;
    ofFloatColor color;
};

struct Sparkle {
    glm::vec2 position = {0.0f, 0.0f};
    float baseRadius = 0.0f;
    float noiseOffset = 0.0f;
    float noiseSpeed = 0.0f;
    ofFloatColor color;
};

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;

private:
    void setupPalette();
    void setupRibbons();
    void setupHalos();
    void setupBackgroundDust();
    void setupSparkles();
    void drawBackground();
    void drawRibbons();
    void drawHalos();
    void drawBackgroundDust();
    void drawSparkles();

    std::vector<ofFloatColor> palette;
    std::vector<RibbonLayer> ribbons;
    std::vector<HaloDot> halos;
    std::vector<TextureDot> backgroundDust;
    std::vector<Sparkle> sparkles;

    ofFloatColor bgTop;
    ofFloatColor bgBottom;
    ofFloatColor bgMiddle;
    ofFloatColor radialHighlightColor;

    float time = 0.0f;
    float backgroundPulse = 0.0f;
};
