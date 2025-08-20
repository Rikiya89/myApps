#include "ofApp.h"

static inline glm::vec2 rotate2D(const glm::vec2& p, float a){
    float s = sinf(a), c = cosf(a);
    return { c*p.x - s*p.y, s*p.x + c*p.y };
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetCircleResolution(180);
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    center = { ofGetWidth() * 0.5f, ofGetHeight() * 0.5f };
    ofBackground(0); // default: black bg
}

//--------------------------------------------------------------
void ofApp::update(){
    t = ofGetElapsedTimef();

    if (animate){
        // Tasteful, slow morph between elegant families
        float u = 0.5f * (1.0f + sinf(t * 0.25f));
        float v = 0.5f * (1.0f + sinf(t * 0.17f + 2.1f));
        float w = 0.5f * (1.0f + sinf(t * 0.21f + 5.3f));

        // m sets rotational symmetry / petals
        m  = 3.0f + 6.0f * u;       // [3, 9]
        // n’s shape the curvature of tips/valleys
        n1 = 0.2f + 2.2f * v;       // keep > 0
        n2 = 0.15f + 2.0f * (1.0f - u);
        n3 = 0.20f + 2.0f * w;
    }
}

//--------------------------------------------------------------
float ofApp::supershape(float theta,
                        float m, float n1, float n2, float n3,
                        float a, float b) const{
    float nn1 = std::max(0.0001f, n1);

    float t1 = powf(fabsf(cosf(m * theta * 0.25f) / a), n2);
    float t2 = powf(fabsf(sinf(m * theta * 0.25f) / b), n3);
    float denom = powf(t1 + t2, 1.0f / nn1);

    if (denom < 1e-6f) return 0.0f;
    return 1.0f / denom;
}

//--------------------------------------------------------------
void ofApp::buildLayer(std::vector<glm::vec2>& pts, float scale, float rotation) const{
    pts.clear();
    pts.reserve(samples);

    for(int i = 0; i < samples; ++i){
        float theta = ofMap(i, 0, samples, 0.0f, TWO_PI, false);
        float r = supershape(theta, m, n1, n2, n3) * baseRadius * scale;

        glm::vec2 p = { r * cosf(theta), r * sinf(theta) };
        p = rotate2D(p, rotation);
        p += center;
        pts.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!inverted) ofBackground(0);
    else          ofBackground(255);

    // Main ink color: white on black, black on white
    ofSetColor(inverted ? 0 : 255, 230);

    std::vector<glm::vec2> pts;
    for(int L = 0; L < layers; ++L){
        float s = powf(layerScale, (float)L);            // geometric shrink
        float r = rotPerLayer * (float)L + rotSpeed * t; // per-layer + time rot
        buildLayer(pts, s, r);

        // ---- Outline
        if (drawOutline){
            ofSetLineWidth(outlineWeight);
            ofPolyline poly;
            // (Fix for oF 0.12) add vertices from vec2 manually
            for(const auto& p : pts) poly.addVertex(p.x, p.y);
            poly.close();
            poly.draw();
        }

        // ---- Optional radials
        if (drawRadial){
            ofSetLineWidth(0.7f * outlineWeight);
            for(int i = 0; i < samples; i += 30){
                ofDrawLine(center, pts[i]);
            }
        }

        // ---- Maurer-style chords
        if (drawChords){
            ofSetLineWidth(chordWeight);
            int N = (int)pts.size();
            for(int i = 0; i < N; i += chordSkip){
                int j = (i + chordStep) % N;
                int a = ofMap(L, 0, layers - 1, 220, 28, true); // fade with depth
                ofSetColor(inverted ? 0 : 255, a);
                ofDrawLine(pts[i], pts[j]);
            }
        }
    }

    // HUD
//    ofSetColor(inverted ? 0 : 255, 255);
//    ofDrawBitmapStringHighlight(
//        "Monochrome Supershape — keys: [Space]=animate  [I]=invert  [S]=save\n"
//        "[1][2][3]=presets  Up/Down=layers  Left/Right=chordStep  MouseWheel=size\n"
//        "m=" + ofToString(m,2) + "  n1=" + ofToString(n1,2) +
//        "  n2=" + ofToString(n2,2) + "  n3=" + ofToString(n3,2),
//        16, 24
//    );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case ' ': animate = !animate; break;
        case 'i': inverted = !inverted; break;
        case 's': ofSaveScreen("supershape_bw_" + ofGetTimestampString() + ".png"); break;

        // Elegant preset families
        case '1': m = 5.0f; n1 = 0.3f; n2 = 0.2f; n3 = 1.7f; break;  // spiky petals
        case '2': m = 6.0f; n1 = 1.0f; n2 = 1.0f; n3 = 1.0f; break;  // rounded flower
        case '3': m = 8.0f; n1 = 0.2f; n2 = 2.5f; n3 = 0.4f; break;  // superellipse-ish

        case OF_KEY_UP:    layers    = ofClamp(layers + 1, 4, 72); break;
        case OF_KEY_DOWN:  layers    = ofClamp(layers - 1, 4, 72); break;
        case OF_KEY_RIGHT: chordStep = ofClamp(chordStep + 1, 1, samples - 1); break;
        case OF_KEY_LEFT:  chordStep = ofClamp(chordStep - 1, 1, samples - 1); break;

        // toggles
        case 'o': drawOutline = !drawOutline; break;
        case 'c': drawChords  = !drawChords;  break;
        case 'r': drawRadial  = !drawRadial;  break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int, int, float, float scrollY){
    baseRadius = ofClamp(baseRadius + scrollY * 12.0f, 80.0f, 1000.0f);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    center = { w * 0.5f, h * 0.5f };
}
