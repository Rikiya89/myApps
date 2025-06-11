#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofTrueTypeFont font;
    string message;
    float time;
    ofEasyCam cam;
};
