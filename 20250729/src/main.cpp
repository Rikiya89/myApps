#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(720, 1080); // Instagram Reels format (9:16 aspect ratio)
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	settings.setPosition(ofVec2f(300, 100)); // Position window for easy recording

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
