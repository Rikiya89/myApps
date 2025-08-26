#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    void windowResized(int w, int h) override;

private:
    // ---- Palette ----
    std::vector<ofFloatColor> palette;
    static ofFloatColor HEX(int hex);

    // ---- Scene / Anim ----
    float t = 0.0f;
    float speed = 0.25f;
    bool  paused = false;
    bool  invert = false;
    bool  showHud = true;   // toggle with H

    // ---- FBOs ----
    ofFbo trailFbo;
    ofFbo layersFbo;
    ofFbo blurFbo[2];
    float trailFade = 16.0f;   // 0..255

    // ---- Shader ----
    ofShader blurShader;       // separable Gaussian blur

    // ---- Geometry ----
    struct Layer {
        float scale;
        float baseRot; // radians

        // primary supershape
        float m;
        float n1, n2, n3;

        // secondary supershape (blend target)
        float m2;
        float blendSpeed;

        // extra harmonics
        float harmK;        // epicycloid-ish k
        float harmAmp;      // amplitude 0..1 (relative)

        // petal accent & domain warp
        float petalFreq;
        float petalPow;
        float warpAmp;
        float warpFreq;

        // motion
        float wobbleAmp;
        float wobbleFreq;

        // ribbon
        float baseWidth;    // px
        float widthJitter;  // px
        float lineWeight;   // for wire overlay

        ofFloatColor color;
    };
    std::vector<Layer> layers;
    int samples = 1300; // resolution

    // dust (glow specks)
    struct Dust { float r,a,size,w; ofFloatColor c; };
    std::vector<Dust> dust;
    void initDust(int count);
    void updateDust(float dt);
    void drawDust();

    // math
    float supershape(float theta, float m, float n1, float n2, float n3,
                     float a = 1.0f, float b = 1.0f) const;

    // build/draw
    void buildLayerRibbon(ofMesh& mesh, const Layer& L, float time) const; // TRIANGLE_STRIP
    void drawLayers(); // draw centered

    // setup
    void allocateAll(int w, int h);
    void loadShaders();

    // colors
    ofFloatColor bgColor() const;
    ofFloatColor inkColor() const;

    // utils
    static inline unsigned char to8(float c){ return (unsigned char)ofClamp(c * 255.0f, 0.0f, 255.0f); }
    static inline unsigned char clamp8(float a){ return (unsigned char)ofClamp(a, 0.0f, 255.0f); }
};
