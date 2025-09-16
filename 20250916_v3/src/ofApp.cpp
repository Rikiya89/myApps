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
	
	time = 0;
	numRibbons = 15;
	numCrystals = 8;
	showWireframe = false;
	
	flowField.cols = 20;
	flowField.rows = 15;
	flowField.depth = 20;
	flowField.scale = 30;
	flowField.vectors.resize(flowField.cols * flowField.rows * flowField.depth);
	
	ribbons.resize(numRibbons);
	for(int i = 0; i < numRibbons; i++){
		ribbons[i].position = ofVec3f(
			ofRandom(-300, 300),
			ofRandom(-400, 400),
			ofRandom(-300, 300)
		);
		ribbons[i].velocity = ofVec3f(
			ofRandom(-0.5, 0.5),
			ofRandom(-0.5, 0.5),
			ofRandom(-0.5, 0.5)
		);
		ribbons[i].life = 1.0;
		ribbons[i].maxPoints = 80;
	}
	
	crystalMeshes.resize(numCrystals);
	crystalPositions.resize(numCrystals);
	crystalRotations.resize(numCrystals);
	crystalScales.resize(numCrystals);
	
	for(int i = 0; i < numCrystals; i++){
		crystalPositions[i] = ofVec3f(
			ofRandom(-200, 200),
			ofRandom(-300, 300),
			ofRandom(-200, 200)
		);
		crystalRotations[i] = ofRandom(0, 360);
		crystalScales[i] = ofRandom(20, 60);
		
		crystalMeshes[i].setMode(OF_PRIMITIVE_TRIANGLES);
		
		int segments = 8;
		float height = crystalScales[i];
		float radius = crystalScales[i] * 0.3;
		
		for(int j = 0; j < segments; j++){
			float angle1 = (j * TWO_PI) / segments;
			float angle2 = ((j + 1) * TWO_PI) / segments;
			
			ofVec3f v1(cos(angle1) * radius, -height/2, sin(angle1) * radius);
			ofVec3f v2(cos(angle2) * radius, -height/2, sin(angle2) * radius);
			ofVec3f top(0, height/2, 0);
			ofVec3f bottom(0, -height/2, 0);
			
			crystalMeshes[i].addVertex(v1);
			crystalMeshes[i].addVertex(v2);
			crystalMeshes[i].addVertex(top);
			
			crystalMeshes[i].addVertex(v1);
			crystalMeshes[i].addVertex(bottom);
			crystalMeshes[i].addVertex(v2);
			
			ofColor col = themeColors[j % themeColors.size()];
			col.a = 150;
			for(int k = 0; k < 6; k++){
				crystalMeshes[i].addColor(col);
			}
		}
	}
	
	light.setPosition(0, -200, 200);
	light.setDiffuseColor(ofFloatColor(0.8, 0.8, 1.0));
	light.setAmbientColor(ofFloatColor(0.2, 0.2, 0.3));
	
	ofEnableDepthTest();
	ofEnableLighting();
	ofBackground(15, 15, 25);
}

//--------------------------------------------------------------
void ofApp::update(){
	time += ofGetLastFrameTime();
	
	for(int x = 0; x < flowField.cols; x++){
		for(int y = 0; y < flowField.rows; y++){
			for(int z = 0; z < flowField.depth; z++){
				int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
				
				float noiseValue = ofNoise(
					x * 0.1, 
					y * 0.1, 
					z * 0.1, 
					time * 0.3
				);
				
				float angle = noiseValue * TWO_PI * 4;
				float elevation = (ofNoise(x * 0.05, y * 0.05, z * 0.05, time * 0.2) - 0.5) * PI;
				
				flowField.vectors[index] = ofVec3f(
					cos(angle) * cos(elevation),
					sin(elevation),
					sin(angle) * cos(elevation)
				);
			}
		}
	}
	
	for(int i = 0; i < numRibbons; i++){
		ofVec3f gridPos = ribbons[i].position / flowField.scale;
		
		int x = (int)(gridPos.x + flowField.cols/2);
		int y = (int)(gridPos.y + flowField.rows/2);
		int z = (int)(gridPos.z + flowField.depth/2);
		
		if(x >= 0 && x < flowField.cols && y >= 0 && y < flowField.rows && z >= 0 && z < flowField.depth){
			int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
			ofVec3f force = flowField.vectors[index];
			
			ribbons[i].velocity += force * 0.1;
			ribbons[i].velocity *= 0.95;
		}
		
		ribbons[i].position += ribbons[i].velocity;
		
		if(ribbons[i].position.x > 400) ribbons[i].position.x = -400;
		if(ribbons[i].position.x < -400) ribbons[i].position.x = 400;
		if(ribbons[i].position.y > 500) ribbons[i].position.y = -500;
		if(ribbons[i].position.y < -500) ribbons[i].position.y = 500;
		if(ribbons[i].position.z > 400) ribbons[i].position.z = -400;
		if(ribbons[i].position.z < -400) ribbons[i].position.z = 400;
		
		ribbons[i].points.push_back(ribbons[i].position);
		
		int colorIndex = (int)(ofNoise(time * 0.5 + i * 0.1) * themeColors.size()) % themeColors.size();
		ofColor ribbonColor = themeColors[colorIndex];
		ribbonColor.a = ofMap(ribbons[i].life, 0, 1, 50, 200);
		ribbons[i].colors.push_back(ribbonColor);
		
		if(ribbons[i].points.size() > ribbons[i].maxPoints){
			ribbons[i].points.erase(ribbons[i].points.begin());
			ribbons[i].colors.erase(ribbons[i].colors.begin());
		}
		
		ribbons[i].life = ofMap(ribbons[i].points.size(), 0, ribbons[i].maxPoints, 0.2, 1.0);
	}
	
	for(int i = 0; i < numCrystals; i++){
		crystalRotations[i] += ofNoise(i * 0.1, time * 0.5) * 2.0;
		
		float breathe = sin(time * 2.0 + i * 0.5) * 0.1 + 1.0;
		crystalScales[i] = crystalScales[i] * 0.99 + (ofRandom(20, 60) * breathe) * 0.01;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	light.enable();
	cam.begin();
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	for(int i = 0; i < numCrystals; i++){
		ofPushMatrix();
		ofTranslate(crystalPositions[i]);
		ofRotateYDeg(crystalRotations[i]);
		ofScale(1.0, crystalScales[i] / 40.0, 1.0);
		
		if(showWireframe){
			ofNoFill();
			ofSetLineWidth(2);
		} else {
			ofFill();
		}
		
		ofColor crystalColor = themeColors[i % themeColors.size()];
		crystalColor.a = 120;
		ofSetColor(crystalColor);
		
		crystalMeshes[i].draw();
		ofPopMatrix();
	}
	
	ofDisableLighting();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	for(int i = 0; i < numRibbons; i++){
		if(ribbons[i].points.size() > 1){
			ofNoFill();
			ofSetLineWidth(3);
			
			for(int j = 1; j < ribbons[i].points.size(); j++){
				float alpha = ofMap(j, 0, ribbons[i].points.size()-1, 20, 200);
				ofColor lineColor = ribbons[i].colors[j];
				lineColor.a = alpha;
				ofSetColor(lineColor);
				
				ofVec3f p1 = ribbons[i].points[j-1];
				ofVec3f p2 = ribbons[i].points[j];
				
				ofDrawLine(p1, p2);
				
				ofPushMatrix();
				ofTranslate(p2);
				float size = ofMap(j, 0, ribbons[i].points.size()-1, 1, 5);
				ofDrawSphere(0, 0, 0, size);
				ofPopMatrix();
			}
		}
	}
	
	if(showWireframe){
		ofSetColor(themeColors[2], 50);
		ofNoFill();
		ofSetLineWidth(1);
		
		float scale = flowField.scale;
		for(int x = 0; x < flowField.cols; x++){
			for(int y = 0; y < flowField.rows; y++){
				for(int z = 0; z < flowField.depth; z++){
					int index = x + y * flowField.cols + z * flowField.cols * flowField.rows;
					
					ofVec3f pos(
						(x - flowField.cols/2) * scale,
						(y - flowField.rows/2) * scale,
						(z - flowField.depth/2) * scale
					);
					
					ofVec3f vec = flowField.vectors[index] * scale * 0.5;
					ofDrawLine(pos, pos + vec);
				}
			}
		}
	}
	
	ofDisableBlendMode();
	cam.end();
	light.disable();
	
	ofSetColor(themeColors[4], 150);
	string info = "FPS: " + ofToString(ofGetFrameRate(), 1) + "\n";
	info += "Ribbons: " + ofToString(numRibbons) + "\n";
	info += "Crystals: " + ofToString(numCrystals) + "\n";
	info += "Press 'w' for wireframe\nPress 'r' to reset\nPress SPACE for new crystals";
	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		for(int i = 0; i < numCrystals; i++){
			crystalPositions[i] = ofVec3f(
				ofRandom(-200, 200),
				ofRandom(-300, 300),
				ofRandom(-200, 200)
			);
			crystalRotations[i] = ofRandom(0, 360);
			crystalScales[i] = ofRandom(20, 60);
		}
	}
	
	if(key == 'r' || key == 'R'){
		time = 0;
		for(int i = 0; i < numRibbons; i++){
			ribbons[i].points.clear();
			ribbons[i].colors.clear();
			ribbons[i].position = ofVec3f(
				ofRandom(-300, 300),
				ofRandom(-400, 400),
				ofRandom(-300, 300)
			);
			ribbons[i].velocity = ofVec3f(
				ofRandom(-0.5, 0.5),
				ofRandom(-0.5, 0.5),
				ofRandom(-0.5, 0.5)
			);
		}
	}
	
	if(key == 'w' || key == 'W'){
		showWireframe = !showWireframe;
	}
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
	ofVec3f mousePos3D = cam.screenToWorld(ofVec3f(x, y, 0));
	
	for(int i = 0; i < numRibbons; i++){
		float distance = ribbons[i].position.distance(mousePos3D);
		if(distance < 150){
			ofVec3f attraction = (mousePos3D - ribbons[i].position).normalized() * 2.0;
			ribbons[i].velocity += attraction;
		}
	}
	
	for(int i = 0; i < numCrystals; i++){
		float distance = crystalPositions[i].distance(mousePos3D);
		if(distance < 100){
			crystalScales[i] *= 1.2;
			crystalRotations[i] += 45;
		}
	}
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
