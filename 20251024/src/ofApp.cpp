#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofEnableSmoothing();

	// Initialize color palette
	palette.push_back(ofColor::fromHex(0x362d78));
	palette.push_back(ofColor::fromHex(0x523fa3));
	palette.push_back(ofColor::fromHex(0x916ccc));
	palette.push_back(ofColor::fromHex(0xbda1e5));
	palette.push_back(ofColor::fromHex(0xc8c0e9));
	palette.push_back(ofColor::fromHex(0x84bae7));
	palette.push_back(ofColor::fromHex(0x516ad4));
	palette.push_back(ofColor::fromHex(0x333f87));
	palette.push_back(ofColor::fromHex(0x293039));
	palette.push_back(ofColor::fromHex(0x283631));

	// Initialize particles
	for(int i = 0; i < 300; i++){
		Particle p;
		float radius = ofRandom(100, 400);
		float theta = ofRandom(TWO_PI);
		float phi = ofRandom(PI);

		p.position.x = radius * sin(phi) * cos(theta);
		p.position.y = radius * sin(phi) * sin(theta);
		p.position.z = radius * cos(phi);
		p.velocity = ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5));
		p.size = ofRandom(2, 8);
		p.color = palette[int(ofRandom(palette.size()))];
		p.life = ofRandom(0.5, 1.0);
		particles.push_back(p);
	}

	time = 0;

	// Setup lighting
	light.setPointLight();
	light.setPosition(200, 200, 200);

	ofBackground(palette[8]);
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.01;

	// Update particles
	for(auto& p : particles){
		p.position += p.velocity;

		// Create orbital motion
		float angle = time * 0.5;
		float radius = p.position.length();

		// Add wave motion
		p.position.x += sin(time + p.position.y * 0.01) * 0.5;
		p.position.y += cos(time + p.position.z * 0.01) * 0.5;
		p.position.z += sin(time + p.position.x * 0.01) * 0.5;

		// Respawn particles that drift too far
		if(p.position.length() > 600){
			float theta = ofRandom(TWO_PI);
			float phi = ofRandom(PI);
			float rad = ofRandom(100, 400);

			p.position.x = rad * sin(phi) * cos(theta);
			p.position.y = rad * sin(phi) * sin(theta);
			p.position.z = rad * cos(phi);
			p.color = palette[int(ofRandom(palette.size()))];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofEnableLighting();
	light.enable();

	// Draw rotating spheres
	for(int i = 0; i < 5; i++){
		ofPushMatrix();

		float angle = time + i * TWO_PI / 5;
		float radius = 200 + i * 30;

		ofTranslate(
			cos(angle) * radius,
			sin(angle * 1.3) * radius * 0.7,
			sin(angle * 0.8) * radius
		);

		ofRotateDeg(time * 30 + i * 20, 1, 1, 0);

		ofSetColor(palette[i % palette.size()], 150);
		ofDrawSphere(20 + i * 5);

		ofPopMatrix();
	}

	// Draw icosphere at center
	ofPushMatrix();
	ofRotateDeg(time * -25, 1, 1, 1);
	ofSetColor(palette[6], 120);
	ofDrawIcoSphere(0, 0, 0, 80);
	ofPopMatrix();

	// Draw particles
	ofDisableLighting();
	for(const auto& p : particles){
		ofSetColor(p.color, p.life * 255);
		ofDrawSphere(p.position, p.size);
	}

	// Draw connecting lines between nearby particles
	ofSetLineWidth(1);
	for(int i = 0; i < particles.size(); i++){
		for(int j = i + 1; j < particles.size(); j++){
			float dist = particles[i].position.distance(particles[j].position);
			if(dist < 80){
				float alpha = ofMap(dist, 0, 80, 100, 0);
				ofSetColor(palette[3], alpha);
				ofDrawLine(particles[i].position, particles[j].position);
			}
		}
	}

	// Draw spiraling ribbon
	ofSetColor(palette[4], 80);
	ofNoFill();
	ofSetLineWidth(2);
	ofBeginShape();
	for(int i = 0; i < 200; i++){
		float t = i / 200.0;
		float spiralAngle = t * TWO_PI * 8 + time;
		float spiralRadius = 100 + sin(t * PI * 4 + time) * 50;
		float x = cos(spiralAngle) * spiralRadius;
		float y = t * 400 - 200;
		float z = sin(spiralAngle) * spiralRadius;
		ofVertex(x, y, z);
	}
	ofEndShape();
	ofFill();

	light.disable();
	cam.end();
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
