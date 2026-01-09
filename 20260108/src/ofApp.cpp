#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0);
	ofEnableDepthTest();
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetCircleResolution(64);
	ofSetSphereResolution(24);

	// Golden ratio and golden angle
	phi = (1.0 + sqrt(5.0)) / 2.0;
	goldenAngle = 137.5077640500378;

	// Generate Fibonacci sequence
	generateFibonacci(25);

	// Setup camera
	cam.setDistance(600);
	cam.setNearClip(0.1);
	cam.setFarClip(10000);
	cam.disableMouseInput();

	// Initialize particles
	initParticles(200);

	// Setup FBO for glow effect
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	blurFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	time = 0;
	camAngle = 0;
	camHeight = 0;
}

//--------------------------------------------------------------
void ofApp::generateFibonacci(int count){
	fibonacci.clear();
	fibonacci.push_back(1);
	fibonacci.push_back(1);
	for(int i = 2; i < count; i++){
		fibonacci.push_back(fibonacci[i-1] + fibonacci[i-2]);
	}
}

//--------------------------------------------------------------
void ofApp::initParticles(int count){
	particles.clear();
	for(int i = 0; i < count; i++){
		Particle p;
		float angle = i * goldenAngle * DEG_TO_RAD;
		float radius = ofRandom(100, 400);
		p.pos = glm::vec3(
			cos(angle) * radius,
			ofRandom(-300, 300),
			sin(angle) * radius
		);
		p.vel = glm::vec3(
			ofRandom(-0.5, 0.5),
			ofRandom(-0.3, 0.3),
			ofRandom(-0.5, 0.5)
		);
		p.size = ofRandom(1, 4);
		p.alpha = ofRandom(50, 200);
		p.life = ofRandom(0, 100);
		p.maxLife = ofRandom(100, 300);
		particles.push_back(p);
	}
}

//--------------------------------------------------------------
void ofApp::updateParticles(){
	for(auto& p : particles){
		p.pos += p.vel;
		p.life += 1;

		// Gentle attraction to center
		glm::vec3 toCenter = -p.pos * 0.001f;
		p.vel += toCenter;
		p.vel *= 0.99;

		// Reset if too far or life ended
		if(glm::length(p.pos) > 500 || p.life > p.maxLife){
			float angle = ofRandom(TWO_PI);
			float radius = ofRandom(50, 150);
			p.pos = glm::vec3(cos(angle) * radius, ofRandom(-100, 100), sin(angle) * radius);
			p.life = 0;
			p.vel = glm::vec3(ofRandom(-1, 1), ofRandom(-0.5, 0.5), ofRandom(-1, 1));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time = ofGetElapsedTimef();
	updateParticles();

	// Smooth camera orbit
	camAngle += 0.003;
	camHeight = sin(time * 0.2) * 200;
	float camDist = 600 + sin(time * 0.15) * 100;
	cam.setPosition(
		cos(camAngle) * camDist,
		camHeight,
		sin(camAngle) * camDist
	);
	cam.lookAt(glm::vec3(0, 0, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw to FBO for glow effect
	fbo.begin();
	ofClear(0, 0, 0, 255);

	cam.begin();

	ofEnableBlendMode(OF_BLENDMODE_ADD);

	drawGoldenSpiral();
	drawPhyllotaxis();
	drawFibonacciTower();
	drawFloatingParticles();
	drawConnectionWeb();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	cam.end();
	fbo.end();

	// Draw the main image
	ofSetColor(255);
	fbo.draw(0, 0);

	// Draw subtle vignette
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	ofSetColor(255);
	float cx = ofGetWidth() / 2;
	float cy = ofGetHeight() / 2;
	float maxDist = sqrt(cx*cx + cy*cy);
	for(int i = 0; i < 5; i++){
		float r = maxDist - i * 50;
		ofSetColor(255 - i * 10);
		ofDrawCircle(cx, cy, r);
	}
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void ofApp::drawGoldenSpiral(){
	ofPushMatrix();
	ofRotateYDeg(time * 5);

	int numCurves = 8;
	int pointsPerCurve = 200;

	for(int c = 0; c < numCurves; c++){
		float offsetAngle = c * (360.0 / numCurves);

		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_LINE_STRIP);

		for(int i = 0; i < pointsPerCurve; i++){
			float t = i / (float)pointsPerCurve;
			float angle = t * 6 * PI + offsetAngle * DEG_TO_RAD;
			float radius = pow(phi, angle / (PI/2)) * 3;

			// Add wave motion
			float wave = sin(time * 2 + t * 10) * 10;

			float x = cos(angle) * radius;
			float y = sin(angle) * radius;
			float z = t * 300 - 150 + wave;

			// Fade alpha along the spiral
			float alpha = (1.0 - t) * 180 + 20;
			mesh.addVertex(glm::vec3(x, y, z));
			mesh.addColor(ofFloatColor(1, 1, 1, alpha/255.0));
		}

		ofSetLineWidth(1.5);
		mesh.draw();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawPhyllotaxis(){
	ofPushMatrix();
	ofRotateXDeg(90);
	ofRotateZDeg(-time * 3);

	int numElements = 300;

	for(int i = 0; i < numElements; i++){
		float angle = i * goldenAngle * DEG_TO_RAD;
		float radius = sqrt((float)i) * 12;

		// Breathing motion
		float breath = 1.0 + sin(time * 1.5 + i * 0.02) * 0.15;
		radius *= breath;

		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		float z = sin(time + i * 0.05) * 30;

		// Size based on Fibonacci
		int fibIndex = i % fibonacci.size();
		float size = (fibonacci[fibIndex] / fibonacci[fibonacci.size()-1]) * 8 + 2;
		size *= breath;

		// Alpha gradient from center
		float distFromCenter = radius / (sqrt((float)numElements) * 12);
		float alpha = (1.0 - distFromCenter * 0.7) * 200;

		ofSetColor(255, 255, 255, alpha);

		ofPushMatrix();
		ofTranslate(x, y, z);
		ofRotateXDeg(time * 20 + i);

		// Alternate between circles and diamonds
		if(i % 3 == 0){
			ofNoFill();
			ofSetLineWidth(1);
			ofDrawCircle(0, 0, size);
			ofFill();
			ofDrawCircle(0, 0, size * 0.3);
		} else {
			ofPushMatrix();
			ofRotateZDeg(45);
			ofNoFill();
			ofDrawRectangle(-size/2, -size/2, size, size);
			ofFill();
			ofDrawRectangle(-size/4, -size/4, size/2, size/2);
			ofPopMatrix();
		}

		ofPopMatrix();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawFibonacciTower(){
	ofPushMatrix();
	ofRotateYDeg(time * 8);

	int numLevels = 15;

	for(int level = 0; level < numLevels; level++){
		float y = (level - numLevels/2.0) * 40;
		float levelRadius = 80 + sin(time + level * 0.5) * 20;

		int elementsInLevel = fibonacci[min(level + 3, (int)fibonacci.size()-1)];
		elementsInLevel = min(elementsInLevel, 34);

		for(int i = 0; i < elementsInLevel; i++){
			float angle = (i / (float)elementsInLevel) * TWO_PI + time * (level % 2 == 0 ? 1 : -1) * 0.5;

			float x = cos(angle) * levelRadius;
			float z = sin(angle) * levelRadius;

			float alpha = 150 - abs(level - numLevels/2.0) * 15;

			ofSetColor(255, 255, 255, alpha);

			ofPushMatrix();
			ofTranslate(x, y, z);
			ofRotateYDeg(-angle * RAD_TO_DEG);
			ofRotateXDeg(time * 30);

			float boxSize = 5 + (elementsInLevel - i) * 0.3;

			ofNoFill();
			ofSetLineWidth(1);
			ofDrawBox(boxSize);

			ofPopMatrix();
		}

		// Draw connecting ring
		ofNoFill();
		ofSetColor(255, 255, 255, 40);
		ofSetLineWidth(0.5);

		ofBeginShape();
		for(int i = 0; i <= 60; i++){
			float angle = (i / 60.0) * TWO_PI;
			float x = cos(angle) * levelRadius;
			float z = sin(angle) * levelRadius;
			ofVertex(x, y, z);
		}
		ofEndShape();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawFloatingParticles(){
	ofPushMatrix();

	for(auto& p : particles){
		float lifeRatio = p.life / p.maxLife;
		float fadeAlpha = sin(lifeRatio * PI) * p.alpha;

		ofSetColor(255, 255, 255, fadeAlpha);
		ofFill();

		ofPushMatrix();
		ofTranslate(p.pos);

		// Draw particle with subtle glow
		ofDrawCircle(0, 0, p.size);
		ofSetColor(255, 255, 255, fadeAlpha * 0.3);
		ofDrawCircle(0, 0, p.size * 2);

		ofPopMatrix();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawConnectionWeb(){
	ofPushMatrix();
	ofRotateYDeg(-time * 2);

	// Create vertices based on Fibonacci spiral
	vector<glm::vec3> nodes;
	int numNodes = 50;

	for(int i = 0; i < numNodes; i++){
		float angle = i * goldenAngle * DEG_TO_RAD;
		float radius = sqrt((float)i) * 30;
		float y = sin(time * 0.5 + i * 0.2) * 100;

		nodes.push_back(glm::vec3(
			cos(angle) * radius,
			y,
			sin(angle) * radius
		));
	}

	// Draw connections
	ofSetLineWidth(0.5);
	for(int i = 0; i < numNodes; i++){
		// Connect to Fibonacci-indexed nodes
		for(int f = 0; f < min(8, (int)fibonacci.size()); f++){
			int targetIndex = (i + (int)fibonacci[f]) % numNodes;
			float dist = glm::distance(nodes[i], nodes[targetIndex]);

			if(dist < 200){
				float alpha = (1.0 - dist/200.0) * 60;
				alpha *= 0.5 + sin(time * 2 + i * 0.1) * 0.5;
				ofSetColor(255, 255, 255, alpha);
				ofDrawLine(nodes[i], nodes[targetIndex]);
			}
		}
	}

	// Draw nodes
	for(int i = 0; i < numNodes; i++){
		float pulse = 1.0 + sin(time * 3 + i * 0.3) * 0.3;
		float alpha = 100 + sin(time + i) * 50;
		ofSetColor(255, 255, 255, alpha);
		ofFill();
		ofPushMatrix();
		ofTranslate(nodes[i]);
		ofDrawSphere(2 * pulse);
		ofPopMatrix();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'f'){
		ofToggleFullscreen();
	}
	if(key == 'r'){
		initParticles(200);
	}
	if(key == ' '){
		cam.enableMouseInput();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == ' '){
		cam.disableMouseInput();
	}
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
	fbo.allocate(w, h, GL_RGBA);
	blurFbo.allocate(w, h, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
