#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetWindowShape(720, 1080);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableDepthTest();

    font.load("Georgia.ttf", 30, true, true, true);
    time = 0;

    vector<string> phrases = {
        "Dream in Code",
        "Spirits of Light",
        "Creative Flow",
        "Generative Mind",
        "Art Meets Logic",
        "Digital Poetry",
        "Algorithmic Soul",
        "Neon Thought",
        "Sine Wave Vision",
        "Kinetic Silence"
    };

    for (int i = 0; i < 120; i++) {
        words.push_back(phrases[i % phrases.size()]);
    }


    cam.setDistance(800);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();

    for (int i = 0; i < words.size(); i++) {
        float angle = time * 10 + i * 6;
        float radius = 80 + i * 6 + sin(time + i * 0.3) * 30;
        float x = cos(ofDegToRad(angle)) * radius;
        float y = sin(ofDegToRad(angle)) * radius;
        float z = sin(time * 0.4 + i * 0.1) * 300;

        float scale = 1.0 + 0.3 * sin(time + i * 0.2);
        float alpha = ofMap(i, 0, words.size(), 255, 40);

        ofPushMatrix();
        ofTranslate(x, y, z);
        ofRotateDeg(angle + sin(time + i * 0.4) * 30);
        ofScale(scale, scale);
        ofSetColor(255, alpha);
        font.drawString(words[i], 0, 0);
        ofPopMatrix();
    }

    cam.end();
}
