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
	
	numParticles = 200;
	time = 0;
	
	particles.resize(numParticles);
	velocities.resize(numParticles);
	sizes.resize(numParticles);
	colorIndices.resize(numParticles);
	
	for(int i = 0; i < numParticles; i++){
		particles[i] = ofVec3f(
			ofRandom(-300, 300),
			ofRandom(-400, 400),
			ofRandom(-200, 200)
		);
		velocities[i] = ofVec3f(
			ofRandom(-1, 1),
			ofRandom(-1, 1),
			ofRandom(-1, 1)
		);
		sizes[i] = ofRandom(2, 15);
		colorIndices[i] = (int)ofRandom(themeColors.size());
	}
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for(int x = -5; x <= 5; x++){
		for(int z = -5; z <= 5; z++){
			meshVertices.push_back(ofVec3f(x * 50, 0, z * 50));
			meshColors.push_back(ofFloatColor(themeColors[x % themeColors.size()]));
		}
	}
	
	ofSetSphereResolution(8);
	ofEnableDepthTest();
	ofBackground(30, 30, 40);
}

//--------------------------------------------------------------
void ofApp::update(){
	time += ofGetLastFrameTime();
	
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
	for(int i = 0; i < numParticles; i++){
		ofVec3f noise = ofVec3f(
			ofNoise(particles[i].x * 0.01, time * 0.5),
			ofNoise(particles[i].y * 0.01, time * 0.5),
			ofNoise(particles[i].z * 0.01, time * 0.5)
		);
		noise = (noise - 0.5) * 2;
		
		velocities[i] += noise * 0.1;
		velocities[i] *= 0.98;
		particles[i] += velocities[i];
		
		if(particles[i].x > 360) particles[i].x = -360;
		if(particles[i].x < -360) particles[i].x = 360;
		if(particles[i].y > 540) particles[i].y = -540;
		if(particles[i].y < -540) particles[i].y = 540;
		if(particles[i].z > 300) particles[i].z = -300;
		if(particles[i].z < -300) particles[i].z = 300;
	}
	
	for(int x = 0; x < 10; x++){
		for(int z = 0; z < 10; z++){
			int index = x * 11 + z;
			if(index < meshVertices.size()){
				float height = ofNoise(x * 0.1, z * 0.1, time * 0.3) * 100;
				meshVertices[index].y = height;
				
				float colorFactor = height / 100.0;
				int colorIndex = (int)(colorFactor * themeColors.size()) % themeColors.size();
				ofColor col = themeColors[colorIndex];
				col.a = 150;
				meshColors[index] = ofFloatColor(col);
			}
		}
	}
	
	for(int x = 0; x < 9; x++){
		for(int z = 0; z < 9; z++){
			int i1 = x * 11 + z;
			int i2 = x * 11 + z + 1;
			int i3 = (x + 1) * 11 + z;
			int i4 = (x + 1) * 11 + z + 1;
			
			if(i4 < meshVertices.size()){
				mesh.addVertex(meshVertices[i1]);
				mesh.addVertex(meshVertices[i2]);
				mesh.addVertex(meshVertices[i3]);
				
				mesh.addVertex(meshVertices[i2]);
				mesh.addVertex(meshVertices[i4]);
				mesh.addVertex(meshVertices[i3]);
				
				mesh.addColor(meshColors[i1]);
				mesh.addColor(meshColors[i2]);
				mesh.addColor(meshColors[i3]);
				
				mesh.addColor(meshColors[i2]);
				mesh.addColor(meshColors[i4]);
				mesh.addColor(meshColors[i3]);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableAlphaBlending();
	
	mesh.draw();
	
	for(int i = 0; i < numParticles; i++){
		ofPushMatrix();
		ofTranslate(particles[i]);
		
		ofColor col = themeColors[colorIndices[i]];
		float alpha = ofMap(sin(time * 2 + i * 0.1), -1, 1, 50, 200);
		col.a = alpha;
		ofSetColor(col);
		
		float rotationSpeed = 0.5 + i * 0.01;
		ofRotateYDeg(time * rotationSpeed * 30);
		ofRotateXDeg(time * rotationSpeed * 20);
		
		ofDrawSphere(0, 0, 0, sizes[i]);
		ofPopMatrix();
	}
	
	for(int i = 0; i < numParticles; i += 3){
		if(i + 2 < numParticles){
			ofSetColor(themeColors[i % themeColors.size()], 30);
			ofDrawLine(particles[i], particles[i + 1]);
			ofDrawLine(particles[i + 1], particles[i + 2]);
			ofDrawLine(particles[i + 2], particles[i]);
		}
	}
	
	ofDisableBlendMode();
	cam.end();
	
	ofSetColor(255, 100);
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate(), 1), 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		for(int i = 0; i < numParticles; i++){
			particles[i] = ofVec3f(
				ofRandom(-300, 300),
				ofRandom(-400, 400),
				ofRandom(-200, 200)
			);
			velocities[i] = ofVec3f(
				ofRandom(-2, 2),
				ofRandom(-2, 2),
				ofRandom(-2, 2)
			);
			colorIndices[i] = (int)ofRandom(themeColors.size());
		}
	}
	
	if(key == 'r' || key == 'R'){
		time = 0;
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
	float mouseInfluence = 100.0;
	ofVec3f mousePos3D = cam.screenToWorld(ofVec3f(x, y, 0));
	
	for(int i = 0; i < numParticles; i++){
		float distance = particles[i].distance(mousePos3D);
		if(distance < mouseInfluence){
			ofVec3f force = (particles[i] - mousePos3D).normalized() * (mouseInfluence - distance) * 0.1;
			velocities[i] += force;
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
