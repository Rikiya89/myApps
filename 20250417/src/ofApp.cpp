#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableDepthTest();
    ofSetSphereResolution(12);

    // --- GUI -------------------------------------------------
    gui.setup("Settings");
    gui.add(numPoints      .set("Points",          600, 100, 3000));
    gui.add(noiseScale     .set("Noise Scale",   0.003f, 0.0005f, 0.02f));
    gui.add(noiseSpeed     .set("Noise Speed",    0.25f, 0.01f,   2.0f));
    gui.add(connectionDist .set("Connect Dist",     60.f, 10.f,  300.f));
    gui.add(pointSize      .set("Point Size",        2.f,  1.f,   12.f));

    regeneratePoints();
}

//--------------------------------------------------------------
void ofApp::regeneratePoints() {
    points.clear();
    for (int i = 0; i < numPoints; ++i) {
        points.emplace_back(ofRandom(-300, 300),
                            ofRandom(-300, 300),
                            ofRandom(-300, 300));
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    // If the user changed numPoints in the GUI, rebuild the cloud
    if(static_cast<int>(points.size()) != numPoints) regeneratePoints();

    t += ofGetLastFrameTime() * noiseSpeed;

    for (auto& p : points) {
        // 4‑D Perlin noise lookup
        float nx = ofNoise(p.x * noiseScale, p.y * noiseScale, p.z * noiseScale, t);
        float ny = ofNoise(p.y * noiseScale, p.z * noiseScale, p.x * noiseScale, t + 100.0f);
        float nz = ofNoise(p.z * noiseScale, p.x * noiseScale, p.y * noiseScale, t + 200.0f);

        glm::vec3 dir = glm::vec3(nx - 0.5f, ny - 0.5f, nz - 0.5f);
        p += dir * 4.0f;  // step size
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    ofSetColor(255);

    // 3‑D points and lines …
    for(size_t i=0; i<points.size(); ++i){
        ofDrawSphere(points[i], pointSize);
        for(size_t j=i+1; j<points.size(); ++j){
            if(glm::distance(points[i], points[j]) < connectionDist){
                ofDrawLine(points[i], points[j]);
            }
        }
    }
    cam.end();

    //----------------------------------------------------------
    ofDisableDepthTest();     // <‑‑ turn Z‑buffer off for 2‑D overlays
    gui.draw();               // now the labels & sliders are visible
    ofEnableDepthTest();      // (optional) re‑enable for anything later
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'r' || key == 'R') regeneratePoints();   // quick refresh
}
