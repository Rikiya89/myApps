#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void windowResized(int w, int h) override;

private:
    // Gielis Superformula (Supershape)
    // r(θ) = ( |cos(m θ/4)/a|^n2 + |sin(m θ/4)/b|^n3 )^(-1/n1)
    float supershape(float theta,
                     float m, float n1, float n2, float n3,
                     float a = 1.0f, float b = 1.0f) const;

    // Build one supershape layer at a given scale and rotation
    void buildLayer(std::vector<glm::vec2>& pts, float scale, float rotation) const;

    // Parameters (monochrome)
    bool   inverted    = false;  // white bg, black lines if true
    bool   drawOutline = true;
    bool   drawChords  = true;
    bool   drawRadial  = false;
    bool   animate     = true;

    // Supershape params (animated over time)
    float  m  = 7.0f;
    float  n1 = 0.3f;
    float  n2 = 0.2f;
    float  n3 = 1.7f;

    // Composition
    int    samples      = 900;       // points per curve
    int    layers       = 24;        // stacked curves
    float  layerScale   = 0.96f;     // shrink per layer
    float  baseRadius   = 300.0f;    // overall size
    float  rotPerLayer  = 0.12f;     // rotation offset per layer (radians)
    float  rotSpeed     = 0.25f;     // time rotation

    // Maurer-style chords
    int    chordStep     = 47;       // connect i -> i + chordStep
    int    chordSkip     = 3;        // draw every Nth chord point
    float  outlineWeight = 1.4f;
    float  chordWeight   = 0.9f;

    float     t = 0.0f;              // time
    glm::vec2 center;                 // screen center
};
