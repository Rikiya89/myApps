#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0, 0, 0);
	ofSetFrameRate(60);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableDepthTest();
	
	time = 0;
	numParticles = 1000;
	rotationX = 0;
	rotationY = 0;
	cameraDistance = 600;
	
	colors.push_back(ofColor(255, 182, 255)); // Cotton candy pink
	colors.push_back(ofColor(173, 216, 255)); // Cloud blue
	colors.push_back(ofColor(255, 218, 218)); // Blush peach
	colors.push_back(ofColor(204, 255, 204)); // Whisper green
	colors.push_back(ofColor(230, 191, 255)); // Lilac dream
	colors.push_back(ofColor(191, 239, 255)); // Baby blue
	colors.push_back(ofColor(255, 255, 204)); // Cream yellow
	colors.push_back(ofColor(238, 221, 255)); // Lavender mist
	colors.push_back(ofColor(255, 228, 225)); // Misty rose
	colors.push_back(ofColor(230, 255, 230)); // Honeydew
	colors.push_back(ofColor(224, 255, 255)); // Azure dream
	colors.push_back(ofColor(255, 240, 245)); // Lavender blush
	
	for(int i = 0; i < numParticles; i++){
		float radius = ofRandom(200, 400);
		float theta = ofRandom(0, TWO_PI);
		float phi = ofRandom(0, PI);
		
		float x = radius * sin(phi) * cos(theta);
		float y = radius * sin(phi) * sin(theta);
		float z = radius * cos(phi);
		
		particles.push_back(ofVec3f(x, y, z));
		velocities.push_back(ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5)));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.01;
	rotationX = sin(time * 0.3) * 0.6;
	rotationY += 0.005;
	
	for(int i = 0; i < particles.size(); i++){
		particles[i] += velocities[i];
		
		float distanceFromCenter = particles[i].length();
		if(distanceFromCenter > 500){
			particles[i] = particles[i].normalize() * 500;
			velocities[i] *= -0.8;
		}
		
		float wave = sin(time * 2 + i * 0.1) * 10;
		particles[i] += ofVec3f(wave * cos(time + i), wave * sin(time + i), wave * sin(time * 1.5 + i));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, -cameraDistance);
	ofRotateXDeg(rotationX * 57.2958);
	ofRotateYDeg(rotationY * 57.2958);
	
	for(int i = 0; i < particles.size(); i++){
		ofColor currentColor = colors[i % colors.size()];
		
		float alpha = 180 + 75 * sin(time * 3 + i * 0.05);
		currentColor.a = alpha;
		ofSetColor(currentColor);
		
		float size = 3 + 6 * sin(time * 2 + i * 0.1);
		ofDrawSphere(particles[i].x, particles[i].y, particles[i].z, size);
		
		for(int j = i + 1; j < particles.size(); j++){
			float distance = particles[i].distance(particles[j]);
			if(distance < 60){
				ofColor lineColor = colors[(i + j) % colors.size()];
				lineColor.a = 120 * (1.0 - distance / 60.0);
				ofSetColor(lineColor);
				ofDrawLine(particles[i], particles[j]);
			}
		}
	}
	
	ofPopMatrix();
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
