#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Generative Grid Art");
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	ofBackground(20, 24, 28);
	
	setupColors();
	gridSize = 24;
	cols = 30;
	rows = 45;
	time = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	time += ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	ofFill();
	drawGrid();
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	for(int i = 0; i < 50; i++){
		float particleX = ofNoise(i * 0.1, time * 0.2) * ofGetWidth();
		float particleY = ofNoise(i * 0.2, time * 0.3) * ofGetHeight();
		float particleSize = ofNoise(i * 0.3, time * 0.4) * 3 + 1;
		
		int colorIndex = i % themeColors.size();
		ofColor particleColor = themeColors[colorIndex];
		particleColor.a = 30 + sin(time * 2 + i) * 20;
		
		ofSetColor(particleColor);
		ofDrawCircle(particleX, particleY, particleSize);
	}
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::setupColors(){
	themeColors.clear();
	themeColors.push_back(ofColor::fromHex(0x362d78));
	themeColors.push_back(ofColor::fromHex(0x523fa3));
	themeColors.push_back(ofColor::fromHex(0x916ccc));
	themeColors.push_back(ofColor::fromHex(0xbda1e5));
	themeColors.push_back(ofColor::fromHex(0xc8c0e9));
	themeColors.push_back(ofColor::fromHex(0x84bae7));
	themeColors.push_back(ofColor::fromHex(0x516ad4));
	themeColors.push_back(ofColor::fromHex(0x333f87));
	themeColors.push_back(ofColor::fromHex(0x293039));
	themeColors.push_back(ofColor::fromHex(0x283631));
}

//--------------------------------------------------------------
void ofApp::drawGrid(){
	float cellSize = gridSize;
	
	for(int col = 0; col < cols; col++){
		for(int row = 0; row < rows; row++){
			drawGenerativePattern(col, row, cellSize);
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawGenerativePattern(int col, int row, float cellSize){
	float x = col * cellSize;
	float y = row * cellSize;
	
	float noiseVal1 = ofNoise(col * 0.08, row * 0.08, time * 0.3);
	float noiseVal2 = ofNoise(col * 0.15, row * 0.15, time * 0.7);
	float wave1 = sin(time * 1.5 + col * 0.4 + row * 0.3) * 0.5 + 0.5;
	float wave2 = cos(time * 0.8 + col * 0.2 + row * 0.5) * 0.5 + 0.5;
	
	int colorIndex1 = (int)(noiseVal1 * themeColors.size()) % themeColors.size();
	int colorIndex2 = (int)(noiseVal2 * themeColors.size()) % themeColors.size();
	
	ofColor color1 = themeColors[colorIndex1];
	ofColor color2 = themeColors[colorIndex2];
	
	ofColor blendedColor = color1.lerp(color2, wave1);
	blendedColor.a = 120 + wave2 * 135;
	
	ofPushMatrix();
	ofTranslate(x + cellSize/2, y + cellSize/2);
	
	float rotation = noiseVal1 * 180 + time * 20 + col * 2;
	ofRotateDeg(rotation);
	
	float scale = 0.4 + wave1 * 0.8;
	ofScale(scale, scale);
	
	int patternType = (int)(noiseVal2 * 6);
	
	ofSetColor(blendedColor);
	
	switch(patternType) {
		case 0: {
			ofDrawCircle(0, 0, cellSize * 0.45);
			ofSetColor(blendedColor.r, blendedColor.g, blendedColor.b, 60);
			ofDrawCircle(0, 0, cellSize * 0.6);
			break;
		}
		case 1: {
			float rectSize = cellSize * 0.35;
			ofDrawRectRounded(-rectSize/2, -rectSize/2, rectSize, rectSize, 4);
			ofSetColor(color2.r, color2.g, color2.b, 80);
			ofDrawRectRounded(-rectSize*0.3, -rectSize*0.3, rectSize*0.6, rectSize*0.6, 2);
			break;
		}
		case 2: {
			float triSize = cellSize * 0.4;
			ofDrawTriangle(-triSize/2, triSize/2, triSize/2, triSize/2, 0, -triSize/2);
			ofSetColor(color1.r, color1.g, color1.b, 100);
			ofDrawTriangle(-triSize*0.3, triSize*0.3, triSize*0.3, triSize*0.3, 0, -triSize*0.3);
			break;
		}
		case 3: {
			float starSize = cellSize * 0.3;
			ofBeginShape();
			for(int i = 0; i < 10; i++){
				float angle = i * 36;
				float radius = (i % 2 == 0) ? starSize : starSize * 0.5;
				float px = cos(ofDegToRad(angle)) * radius;
				float py = sin(ofDegToRad(angle)) * radius;
				ofVertex(px, py);
			}
			ofEndShape(true);
			break;
		}
		case 4: {
			float petalSize = cellSize * 0.25;
			for(int i = 0; i < 8; i++){
				ofPushMatrix();
				ofRotateDeg(i * 45);
				ofSetColor(blendedColor.r, blendedColor.g, blendedColor.b, 80 + i * 10);
				ofDrawEllipse(petalSize/2, 0, petalSize, petalSize * 0.5);
				ofPopMatrix();
			}
			break;
		}
		case 5: {
			float spiralRadius = cellSize * 0.2;
			ofBeginShape();
			ofNoFill();
			ofSetLineWidth(2);
			for(float t = 0; t < 6.28 * 3; t += 0.3){
				float r = spiralRadius * (t / (6.28 * 3));
				float px = cos(t) * r;
				float py = sin(t) * r;
				ofVertex(px, py);
			}
			ofEndShape();
			ofFill();
			break;
		}
	}
	
	ofPopMatrix();
}
