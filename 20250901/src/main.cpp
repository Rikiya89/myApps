#include "ofMain.h"
#include "ofApp.h"

int main() {
    ofSetupOpenGL(720, 1080, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
