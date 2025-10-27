// ofApp.cpp
#include "ofApp.h"

void ofApp::setup() {
    ofSetWindowTitle("Generative Nebula with Glitters");
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    time = 0.0f;

    int numPoints = 400;
    points.reserve(numPoints);
    for (int i = 0; i < numPoints; i++) {
        float angle = ofRandom(TWO_PI);
        float radius = ofRandom(150, 450);
        points.emplace_back(cos(angle) * radius, sin(angle) * radius);
    }

    trailFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    trailFbo.begin(); ofClear(0, 0, 0, 255); trailFbo.end();
}

void ofApp::update() {
    float dt = ofGetLastFrameTime();
    time += dt;

    // Sparkle generation
    for (auto &pt : points) {
        if (ofRandom(1.0f) < 0.012f) {
            float n = ofNoise(pt.x * 0.002f, pt.y * 0.002f, time * 0.15f);
            float r = ofMap(n, 0, 1, 150, 450);
            float a = atan2(pt.y, pt.x);
            float x = cos(a) * r + ofGetWidth() / 2;
            float y = sin(a) * r + ofGetHeight() / 2;
            Sparkle s;
            s.pos = ofPoint(x, y);
            s.life = 1.0f;
            s.size = ofRandom(3, 8);
            s.angle = ofRandom(TWO_PI);
            sparkles.push_back(s);
        }
    }

    // Update sparkles
    for (int i = sparkles.size() - 1; i >= 0; i--) {
        sparkles[i].life -= dt * 0.9f;
        sparkles[i].angle += dt * 4.0f;
        if (sparkles[i].life <= 0) sparkles.erase(sparkles.begin() + i);
    }

    // Fade FBO for trails
    trailFbo.begin();
    ofSetColor(0, 0, 0, 25);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    trailFbo.end();
}

void ofApp::draw() {
    // Draw trail FBO
    trailFbo.draw(0, 0);

    // Draw rotating, scaling point cloud and mesh
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    float rotation = time * 1.5f;
    float scaleAmt = 1.0f + 0.03f * sin(time * 0.3f);
    ofRotateDeg(rotation);
    ofScale(scaleAmt, scaleAmt);

    // Draw mesh lines
    int meshCount = MIN((int)points.size(), 200);
    for (int i = 0; i < meshCount; i++) {
        float ix = points[i].x;
        float iy = points[i].y;
        for (int j = i + 1; j < meshCount; j++) {
            float dx = ix - points[j].x;
            float dy = iy - points[j].y;
            float d = sqrt(dx * dx + dy * dy);
            if (d < 100) {
                float alpha = ofMap(d, 0, 100, 80, 0);
                ofSetColor(180, 200, 255, alpha);
                ofDrawLine(ix, iy, points[j].x, points[j].y);
            }
        }
    }

    // Draw points
    for (auto &pt : points) {
        float n = ofNoise(pt.x * 0.002f, pt.y * 0.002f, time * 0.2f);
        float r = ofMap(n, 0, 1, 150, 450);
        float a = atan2(pt.y, pt.x);
        float x = cos(a) * r;
        float y = sin(a) * r;
        float hue = fmod(ofMap(n, 0, 1, 0, 255) + time * 25, 255);
        float alpha = ofMap(n, 0, 1, 100, 220);
        ofColor col = ofColor::fromHsb(hue, 200, 255, alpha);
        ofSetColor(col);
        float size = ofMap(ofNoise(pt.x * 0.006f, pt.y * 0.006f, time * 0.2f), 0, 1, 4, 10);
        ofDrawCircle(x, y, size);
    }
    ofPopMatrix();

    // Draw pulsating rings
    ofPushStyle();
    ofNoFill();
    float baseAlpha = 30;
    for (int i = 0; i < 4; i++) {
        float radius = ofMap(sin(time * 0.2f + i), -1, 1, 200, 500);
        ofSetColor(255, 255, 255, baseAlpha);
        ofSetLineWidth(1 + i);
        ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, radius);
    }
    ofPopStyle();

    // Draw sparkles
    for (auto &s : sparkles) {
        float a = ofClamp(s.life, 0, 1);
        ofPushMatrix();
        ofTranslate(s.pos);
        ofRotateRad(s.angle);
        float sz = s.size * a;
        ofSetColor(255, 255, 230, a * 255);
        ofDrawEllipse(0, 0, sz * 2, sz * 0.3);
        ofDrawEllipse(0, 0, sz * 0.3, sz * 2);
        ofPopMatrix();
    }
}
