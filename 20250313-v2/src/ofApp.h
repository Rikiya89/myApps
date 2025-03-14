#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    vector<vector<int>> grid;
    ofColor colors[5] = {ofColor::black, ofColor::white, ofColor::blue, ofColor::red, ofColor::green};
    ofVec2f playerPos;
    ofVec2f enemyPos;
    int GRID_SIZE = 20;
    int COLS = 36;
    int ROWS = 54;
    float timeElapsed;
};
