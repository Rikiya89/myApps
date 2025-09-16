#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
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
	
	gridSize = 40;
	cols = ofGetWidth() / gridSize;
	rows = ofGetHeight() / gridSize;
	noiseScale = 0.005;
	time = 0;
	
	ofSetBackgroundColor(themeColors[8]);
	ofEnableSmoothing();
	ofSetCircleResolution(64);
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.003;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetBackgroundColor(themeColors[8]);
	
	drawSpiritualGrid();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	drawFlowingEnergy();
	drawSacredGeometry();
	drawSpiritualParticles();
	
	ofDisableBlendMode();
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

void ofApp::drawFlowingEnergy(){
	ofSetLineWidth(2);
	
	for(int i = 0; i < 8; i++){
		float centerX = ofGetWidth() / 2;
		float centerY = ofGetHeight() / 2;
		
		float radius = 200 + i * 50;
		float angleOffset = time * 0.5 + i * PI / 4;
		
		ofColor energyColor = themeColors[i % themeColors.size()];
		energyColor.a = ofMap(sin(time * 0.8 + i), -1, 1, 30, 100);
		ofSetColor(energyColor);
		
		ofNoFill();
		ofBeginShape();
		for(float angle = 0; angle < TWO_PI; angle += 0.1){
			float noiseVal = ofNoise(cos(angle + angleOffset) * 0.5, sin(angle + angleOffset) * 0.5, time * 0.2);
			float r = radius + noiseVal * 100;
			float x = centerX + cos(angle) * r;
			float y = centerY + sin(angle) * r;
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
}

void ofApp::drawSacredGeometry(){
	float centerX = ofGetWidth() / 2;
	float centerY = ofGetHeight() / 2;
	
	drawMandalaPattern(centerX, centerY, 150, 8);
	
	for(int i = 0; i < 3; i++){
		float x = centerX + cos(time * 0.3 + i * TWO_PI / 3) * 200;
		float y = centerY + sin(time * 0.3 + i * TWO_PI / 3) * 200;
		drawMandalaPattern(x, y, 80, 6);
	}
	
	ofSetColor(themeColors[2], 40);
	ofSetLineWidth(1);
	ofNoFill();
	for(int i = 0; i < 12; i++){
		float angle = i * TWO_PI / 12;
		float x1 = centerX + cos(angle) * 100;
		float y1 = centerY + sin(angle) * 100;
		float x2 = centerX + cos(angle) * 300;
		float y2 = centerY + sin(angle) * 300;
		ofDrawLine(x1, y1, x2, y2);
	}
}

void ofApp::drawMandalaPattern(float x, float y, float radius, int petals){
	ofPushMatrix();
	ofTranslate(x, y);
	ofRotateDeg(time * 10);
	
	ofColor mandalaColor = themeColors[3];
	mandalaColor.a = ofMap(sin(time * 0.7), -1, 1, 50, 120);
	ofSetColor(mandalaColor);
	
	ofNoFill();
	ofSetLineWidth(1.5);
	
	for(int layer = 0; layer < 3; layer++){
		float layerRadius = radius * (0.3 + layer * 0.3);
		ofDrawCircle(0, 0, layerRadius);
		
		for(int i = 0; i < petals; i++){
			float angle = i * TWO_PI / petals;
			float petalX = cos(angle) * layerRadius;
			float petalY = sin(angle) * layerRadius;
			
			ofDrawCircle(petalX, petalY, layerRadius * 0.3);
			
			if(layer == 0){
				ofDrawLine(0, 0, petalX, petalY);
			}
		}
	}
	
	ofPopMatrix();
}

void ofApp::drawSpiritualGrid(){
	for(int x = 0; x < cols; x++) {
		for(int y = 0; y < rows; y++) {
			float xPos = x * gridSize;
			float yPos = y * gridSize;
			
			float noiseValue = ofNoise(x * noiseScale, y * noiseScale, time * 0.3);
			float noiseValue2 = ofNoise((x + 100) * noiseScale, (y + 100) * noiseScale, time * 0.2);
			float spiritualNoise = ofNoise(x * noiseScale * 2, y * noiseScale * 2, time * 0.1);
			
			int colorIndex = (int)(noiseValue * themeColors.size()) % themeColors.size();
			ofColor currentColor = themeColors[colorIndex];
			
			float alpha = ofMap(noiseValue2, 0, 1, 30, 120);
			currentColor.a = alpha;
			
			ofSetColor(currentColor);
			
			float size = ofMap(noiseValue, 0, 1, gridSize * 0.2, gridSize * 0.6);
			float rotation = spiritualNoise * TWO_PI * 0.3;
			
			ofPushMatrix();
			ofTranslate(xPos + gridSize/2, yPos + gridSize/2);
			ofRotateDeg(ofRadToDeg(rotation));
			
			float centerX = ofGetWidth() / 2;
			float centerY = ofGetHeight() / 2;
			float distFromCenter = ofDist(xPos + gridSize/2, yPos + gridSize/2, centerX, centerY);
			float maxDist = ofDist(0, 0, centerX, centerY);
			float centerInfluence = 1.0 - (distFromCenter / maxDist);
			
			if(spiritualNoise > 0.7) {
				ofNoFill();
				ofSetLineWidth(1 + centerInfluence * 2);
				ofDrawCircle(0, 0, size/2);
				if(centerInfluence > 0.5){
					for(int i = 0; i < 6; i++){
						float angle = i * TWO_PI / 6;
						float px = cos(angle) * size/3;
						float py = sin(angle) * size/3;
						ofDrawCircle(px, py, size/8);
					}
				}
			} else if(noiseValue > 0.4) {
				ofFill();
				ofDrawCircle(0, 0, size/2);
				if(centerInfluence > 0.3){
					ofColor innerColor = themeColors[3];
					innerColor.a = alpha * 0.5;
					ofSetColor(innerColor);
					ofDrawCircle(0, 0, size/4);
				}
			} else {
				ofFill();
				ofDrawTriangle(-size/3, size/3, size/3, size/3, 0, -size/3);
			}
			
			ofPopMatrix();
			
			if(spiritualNoise > 0.85 && centerInfluence > 0.2) {
				ofColor connectionColor = themeColors[4];
				connectionColor.a = 40;
				ofSetColor(connectionColor);
				ofSetLineWidth(1);
				ofDrawLine(xPos + gridSize/2, yPos + gridSize/2, centerX, centerY);
			}
		}
	}
}

void ofApp::drawSpiritualParticles(){
	for(int i = 0; i < 30; i++){
		float noiseX = ofNoise(i * 0.1, time * 0.05);
		float noiseY = ofNoise(i * 0.1 + 100, time * 0.05);
		float noiseZ = ofNoise(i * 0.1 + 200, time * 0.08);
		
		float x = ofMap(noiseX, 0, 1, 0, ofGetWidth());
		float y = ofMap(noiseY, 0, 1, 0, ofGetHeight());
		float size = ofMap(noiseZ, 0, 1, 3, 12);
		
		ofColor particleColor = themeColors[(int)(noiseZ * themeColors.size()) % themeColors.size()];
		particleColor.a = ofMap(sin(time * 0.4 + i * 0.1), -1, 1, 20, 80);
		ofSetColor(particleColor);
		
		ofFill();
		ofDrawCircle(x, y, size);
		
		if(i % 8 == 0){
			float prevX = ofMap(ofNoise(i * 0.1, time * 0.05 - 0.01), 0, 1, 0, ofGetWidth());
			float prevY = ofMap(ofNoise(i * 0.1 + 100, time * 0.05 - 0.01), 0, 1, 0, ofGetHeight());
			
			particleColor.a = 25;
			ofSetColor(particleColor);
			ofSetLineWidth(1.5);
			ofDrawLine(x, y, prevX, prevY);
		}
	}
}
