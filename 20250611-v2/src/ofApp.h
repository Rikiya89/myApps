#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofTrueTypeFont font;
    float time;
    vector<string> words;
    ofEasyCam cam;
};
