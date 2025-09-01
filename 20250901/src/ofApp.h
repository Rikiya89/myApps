#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;

private:
    // ---- helpers ----
    static ofFloatColor HEX(int hex);
    float super(float theta, float m, float n1, float n2, float n3) const;
    void rebuildMesh(float t);
    ofFloatColor paletteSample(float t) const;
    void rotatePalette();

    // ---- scene ----
    ofEasyCam cam;
    ofMesh mesh;                 // triangles
    ofMesh points;               // points layer

    // shaders
    ofShader meshShader;         // fresnel rim
    ofShader pointShader;        // round, soft points

    // parameters
    int   resU = 140;            // longitude steps
    int   resV = 70;             // latitude steps
    float radius = 240.f;
    float timeScale = 0.25f;
    bool  paused = false;
    bool  drawWire = false;
    bool  drawFaces = true;
    bool  autoRotate = true;
    bool  glowOn = true;

    // point sprite size (screen pixels; shader attenuates slightly with distance)
    float pointSize = 3.0f;

    // color palette (provided)
    std::vector<ofFloatColor> palette;

    // rim lighting controls
    ofFloatColor rimColor = ofFloatColor(0.54f, 0.67f, 0.97f); // from #8aa5f6
    float rimStrength = 0.8f; // 0..1

    // bg
    ofColor bg = ofColor(8, 12, 22); // deep navy
};
