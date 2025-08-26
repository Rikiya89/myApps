#include "ofApp.h"

// ---------- palette ----------
static ofFloatColor HEX(int hex) {
    return ofFloatColor(ofColor::fromHex(hex));
}

void ofApp::initPalette() {
    palette = {
        HEX(0x5b8bdf),
        HEX(0x3e79c7),
        HEX(0x08584f),
        HEX(0xead3ff),
        HEX(0x102a67),
        HEX(0x3c7dd1),
        HEX(0x6556bd),
        HEX(0x645ebe),
        HEX(0x152d91),
        HEX(0x8aa5f6),
    };
}

// ---------- emitters ----------
void ofApp::initEmitters(int count) {
    emitters.clear();
    emitters.reserve(count);
    for (int i = 0; i < count; ++i) {
        Emitter e;
        e.seedA  = ofRandom(1000.f);
        e.seedB  = ofRandom(1000.f);
        e.speedA = ofRandom(0.14f, 0.30f);
        e.speedB = ofRandom(0.10f, 0.22f);
        e.color  = palette[i % palette.size()];
        emitters.push_back(e);
    }
}

// ---------- lifecycle ----------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);

    initPalette();

    // Allocate fluid
    scale = 1.0f; // 0.75 or 0.5 if you want more FPS
    fluid.allocate(ofGetWidth(), ofGetHeight(), scale);

    // Softer sim to avoid “hot spots”
    // NOTE: if your ofxFluid version doesn’t expose these, comment and use setters below.
    fluid.dissipation         = 0.994f; // dye fades a bit faster
    fluid.velocityDissipation = 0.993f; // velocity fades to keep motion gentle
    fluid.setGravity(ofVec2f(0, 0));

    // If your version only has setters:
    // fluid.setDissipation(0.994f);
    // fluid.setVelocityDissipation(0.993f);

    initEmitters(10);
}

void ofApp::addInkForce(const ofVec2f& pos, const ofVec2f& vel, const ofFloatColor& col, float radPx) {
    fluid.addTemporalForce(pos, vel, col, radPx);
}

void ofApp::update() {
    if (paused) return;

    const float t = ofGetElapsedTimef();
    const float w = ofGetWidth();
    const float h = ofGetHeight();
    const float R = 0.32f * std::min(w, h);
    const float radPx = radiusNorm * std::min(w, h);

    for (int i = 0; i < (int)emitters.size(); ++i) {
        const auto& e = emitters[i];

        const float a = t * e.speedA * TWO_PI + i * 0.37f;
        const float b = t * e.speedB * TWO_PI + i * 0.53f;

        ofVec2f center(w * 0.5f, h * 0.5f);
        ofVec2f p(
            center.x + R * std::cos(a) * ofNoise(e.seedA, t * 0.15f + i),
            center.y + R * std::sin(b) * ofNoise(e.seedB, t * 0.12f + i * 0.2f)
        );

        // gentle swirl velocity
        ofVec2f tangent(-std::sin(a), std::cos(b));
        ofVec2f v = tangent.getNormalized() * forceStrength;

        // subtle breathing, premultiplied with alpha
        float br = ofMap(std::sin(t * 0.7f + i * 0.6f), -1, 1, brightnessMin, brightnessMax);

        ofFloatColor c = emitters[i].color;
        c = gamma(c, 1.18f);     // darken mids/highs slightly
        c *= br;
        c.a = alphaInk;          // translucent ink

        addInkForce(p, v, c, radPx);
    }

    fluid.update();
}

void ofApp::draw() {
    if (invert) ofBackground(255); else ofBackground(0);

    // ALPHA blending avoids additive “blowout”
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    // Draw full screen
    ofSetColor(255); // texture color
    fluid.draw(0, 0, ofGetWidth(), ofGetHeight());

    ofDisableBlendMode();

    if (showInfo) {
        ofDrawBitmapStringHighlight(
            "ofxfluid  multi-emitter palette flow (soft / translucent)\n"
            "Mouse drag: inject ink & velocity\n"
            "[Space] pause/resume   [I] invert   [H] toggle info\n"
            "[1/2] radius -/+   [3/4] force -/+   [S] save frame",
            16, 24
        );
    }
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case ' ': paused = !paused; break;
        case 'i': case 'I': invert = !invert; break;
        case 'h': case 'H': showInfo = !showInfo; break;

        case '1': radiusNorm = std::max(0.004f, radiusNorm * 0.80f); break;
        case '2': radiusNorm = std::min(0.08f,  radiusNorm * 1.20f); break;
        case '3': forceStrength = std::max(0.6f, forceStrength * 0.85f); break;
        case '4': forceStrength = std::min(8.0f, forceStrength * 1.18f);   break;

        case 's': case 'S': {
            auto ts = ofGetTimestampString("%Y%m%d_%H%M%S");
            ofSaveScreen("fluid_"+ts+".png");
            break;
        }
        default: break;
    }
}

void ofApp::mouseDragged(int x, int y, int button) {
    ofVec2f p(x, y);
    if (!mouseInited) { lastMouse = p; mouseInited = true; }

    ofVec2f v = (p - lastMouse) * 0.25f; // soften manual velocity
    lastMouse = p;

    const ofFloatColor& pick = palette[(int)ofRandom(palette.size())];
    ofFloatColor c = gamma(pick, 1.18f);
    c.a = alphaInk * 1.15f; // a touch more for mouse input

    float radPx = radiusNorm * std::min(ofGetWidth(), ofGetHeight()) * 1.2f;
    addInkForce(p, v, c, radPx);
}

void ofApp::windowResized(int w, int h) {
    fluid.allocate(w, h, scale);
}
