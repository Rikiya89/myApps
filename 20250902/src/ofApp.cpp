// 3D generative sacred geometry in black & white
#include "ofApp.h"

using glm::vec2;
using glm::vec3;

static inline float easeInOutCubic(float x){
    x = ofClamp(x, 0.0f, 1.0f);
    return x < 0.5f ? 4.0f*x*x*x : 1.0f - powf(-2.0f*x + 2.0f, 3.0f)/2.0f;
}

void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofSetCircleResolution(128);

    // Theme: black background, white lines
    ofBackground(0);
    ofSetColor(255);

    // Camera setup for immersive feel
    cam.setDistance(820.0f);
    cam.setNearClip(0.1f);
    cam.setFarClip(5000.0f);

    // Core platonic primitive (wireframe)
    ico = ofIcoSpherePrimitive(baseRadius * 0.32f, 1);

    // Build base geometry
    buildFibonacciSphere(numPoints, baseRadius * 0.72f);
    buildGreatCircleAxes(numCircles);
    buildEdges();
}

void ofApp::update(){
    if(!paused){
        t = ofGetElapsedTimef();
    }
}

void ofApp::draw(){
    // Optional invert (still monochrome)
    if(invert){
        ofBackground(255);
        ofSetColor(0);
    }else{
        ofBackground(0);
        ofSetColor(255);
    }

    // Slight additive glow for luminous lines
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    cam.begin();

    // Global gentle rotation for immersion
    float rotA = 10.0f * sinf(t * 0.27f);
    float rotB = 20.0f * sinf(t * 0.19f + 1.3f);
    float rotC = 15.0f * sinf(t * 0.23f + 2.1f);
    ofPushMatrix();
    ofRotateDeg(rotA, 1, 0, 0);
    ofRotateDeg(rotB, 0, 1, 0);
    ofRotateDeg(rotC, 0, 0, 1);

    // Breathing scale and phase
    float breathe = 0.5f + 0.5f * sinf(t * 0.6f);
    float breathE = easeInOutCubic(breathe);
    float radius = baseRadius * (0.85f + 0.12f * breathE);
    float phase = t * 0.35f;

    // Layer 1: Great circles
    ofSetLineWidth(1.4f);
    drawGreatCircles(radius, phase);

    // Layer 2: Fibonacci web connections
    ofSetLineWidth(1.0f);
    drawFibonacciWeb(radius * 0.98f, phase);

    // Layer 3: Platonic core (wireframe)
    ofSetLineWidth(2.0f);
    drawPlatonicCore(baseRadius * 0.34f, phase);

    ofPopMatrix();
    cam.end();

    ofDisableBlendMode();
}

void ofApp::keyPressed(int key){
    if(key == ' '){
        paused = !paused;
    } else if(key == 'i' || key == 'I'){
        invert = !invert;
    } else if(key == 's' || key == 'S'){
        ofSaveScreen("sacred_frame_" + ofToString(ofGetFrameNum(), 5, '0') + ".png");
    }
}

// --- Helpers ---

void ofApp::buildFibonacciSphere(int n, float radius){
    spherePoints.clear();
    spherePoints.reserve(n);

    const float goldenAngle = (float) (PI * (3.0 - sqrt(5.0))); // ~2.399963
    for(int i = 0; i < n; ++i){
        float y = 1.0f - (i + 0.5f) * (2.0f / n);     // y in (1..-1)
        float r = sqrtf(std::max(0.0f, 1.0f - y*y));  // radius at y
        float theta = i * goldenAngle;
        float x = cosf(theta) * r;
        float z = sinf(theta) * r;
        spherePoints.emplace_back(radius * x, radius * y, radius * z);
    }
}

void ofApp::buildGreatCircleAxes(int n){
    circleAxes.clear();
    circleAxes.reserve(n);

    // Deterministic axes evenly spaced using Fibonacci on unit sphere
    const float goldenAngle = (float) (PI * (3.0 - sqrt(5.0)));
    for(int i = 0; i < n; ++i){
        float y = 1.0f - (i + 0.5f) * (2.0f / n);
        float r = sqrtf(std::max(0.0f, 1.0f - y*y));
        float theta = i * goldenAngle * 0.5f; // half density for axes variety
        float x = cosf(theta) * r;
        float z = sinf(theta) * r;
        glm::vec3 axis = glm::normalize(glm::vec3(x, y, z));
        if(glm::length(axis) > 0.0f) circleAxes.push_back(axis);
    }
}

void ofApp::buildEdges(){
    edges.clear();
    edges.reserve(numPoints * 3);

    // Connect along spiral ordering and with offset steps (sacred weave)
    int n = (int)spherePoints.size();
    if(n < 4) return;

    auto addEdge = [&](int a, int b){
        if(a == b) return;
        if(a > b) std::swap(a, b);
        edges.emplace_back(a, b);
    };

    int stepA = std::max(1, int(roundf(n / 2.618f))); // ~1/phi^2
    int stepB = std::max(1, int(roundf(n / 1.618f))); // ~1/phi

    for(int i = 0; i < n; ++i){
        addEdge(i, (i + 1) % n);          // spiral neighbor
        addEdge(i, (i + stepA) % n);      // golden step A
        addEdge(i, (i + stepB) % n);      // golden step B
    }
}

void ofApp::drawGreatCircles(float radius, float phase){
    // Draw a set of animated great circles oriented by axes
    const int segs = 320;
    const float twoPi = TWO_PI;
    const float wobble = 0.2f * sinf(phase * 1.11f);

    for(size_t i = 0; i < circleAxes.size(); ++i){
        float a = ofMap((float)i, 0, (float)circleAxes.size()-1, 0.0f, 1.0f);
        float alpha = 220.0f * (0.25f + 0.75f * powf(sinf((a + phase*0.07f) * TWO_PI*0.5f)*0.5f+0.5f, 1.6f));
        ofSetColor(ofColor(255, ofClamp((int)alpha, 30, 255)));

        glm::vec3 axis = circleAxes[i];
        // Slow axis precession for living motion
        glm::vec3 precess = glm::normalize(glm::vec3(
            axis.x + 0.15f * sinf(phase*0.51f + i*0.3f),
            axis.y + 0.15f * sinf(phase*0.63f + i*0.37f + 1.2f),
            axis.z + 0.15f * sinf(phase*0.57f + i*0.41f + 2.1f)
        ));

        ofPolyline poly;
        poly.getVertices().reserve(segs+1);

        // Build orthonormal basis (u,v,n) where n aligns with axis
        glm::vec3 n = glm::normalize(precess);
        glm::vec3 tmp = fabs(n.y) < 0.99f ? glm::vec3(0,1,0) : glm::vec3(1,0,0);
        glm::vec3 u = glm::normalize(glm::cross(tmp, n));
        glm::vec3 v = glm::normalize(glm::cross(n, u));

        for(int s=0; s<=segs; ++s){
            float t = (float)s / segs;
            float ang = t * twoPi;

            // Subtle radius modulation to hint at flower-of-life layering
            float rMod = 1.0f + 0.03f * sinf(6.0f*ang + phase*1.3f + (float)i*0.4f) + wobble;
            glm::vec3 p = (u * cosf(ang) + v * sinf(ang)) * (radius * rMod);
            poly.addVertex(p);
        }
        poly.setClosed(true);
        poly.draw();
    }
}

void ofApp::drawFibonacciWeb(float radius, float phase){
    // Animated re-projection of fibonacci points with slow twist
    const float twist = 0.35f * sinf(phase * 0.9f);
    const float twoPi = TWO_PI;

    // Draw points softly to add sparkle
    ofSetColor(255, 90);
    ofSetLineWidth(1.0f);
    for(const auto& p : spherePoints){
        glm::vec3 dir = glm::normalize(p);
        glm::vec3 xAxis = glm::normalize(glm::cross(dir, glm::vec3(0,1,0)) + glm::vec3(0.0001f));
        glm::vec3 yAxis = glm::normalize(glm::cross(xAxis, dir));
        float a = twist * 2.0f;
        glm::vec3 offset = (cosf(a)*xAxis + sinf(a)*yAxis) * 2.0f;
        glm::vec3 pos = glm::normalize(p) * radius + offset;
        ofDrawCircle(pos, 0.9f);
    }

    // Draw connections
    ofSetColor(255, 150);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINES);
    lineMesh.getVertices().reserve(edges.size()*2);
    for(const auto &e : edges){
        const glm::vec3 &a = spherePoints[e.x];
        const glm::vec3 &b = spherePoints[e.y];
        // Project to slightly breathing radius with gentle twist
        glm::vec3 an = glm::normalize(a) * (radius * (0.995f + 0.005f * sinf(phase + (float)e.x*0.01f)));
        glm::vec3 bn = glm::normalize(b) * (radius * (0.995f + 0.005f * sinf(phase + (float)e.y*0.01f)));
        lineMesh.addVertex(an);
        lineMesh.addVertex(bn);
    }
    lineMesh.draw();
}

void ofApp::drawPlatonicCore(float radius, float phase){
    ofPushMatrix();
    // Nested, counter-rotating sacred core
    ofRotateDeg(30.0f * sinf(phase*0.7f), 1, 0, 0);
    ofRotateDeg(45.0f * sinf(phase*0.8f + 1.1f), 0, 1, 0);
    ofRotateDeg(25.0f * sinf(phase*0.9f + 2.3f), 0, 0, 1);

    // Icosahedron
    ofSetColor(255, 220);
    ico.setRadius(radius);
    ico.setResolution(1);
    ico.drawWireframe();

    // Inner dodeca-ish feel by scaling and drawing again with slight alpha
    ofSetColor(255, 120);
    ofScale(0.77f, 0.77f, 0.77f);
    ico.drawWireframe();

    // Subtle luminous cross-axes
    ofSetColor(255, 50);
    ofDrawLine(glm::vec3(-radius,0,0), glm::vec3(radius,0,0));
    ofDrawLine(glm::vec3(0,-radius,0), glm::vec3(0,radius,0));
    ofDrawLine(glm::vec3(0,0,-radius), glm::vec3(0,0,radius));

    ofPopMatrix();
}
