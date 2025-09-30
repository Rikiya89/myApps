#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	time = 0;
	breathePulse = 0;

	fbo.allocate(2560, 390, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 0, 255);
	fbo.end();

	// Initialize particles - optimized count
	for(int i = 0; i < 200; i++){
		particles.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
		float angle = ofRandom(TWO_PI);
		float speed = ofRandom(0.4, 1.0);
		velocities.push_back(ofVec2f(cos(angle) * speed, sin(angle) * speed));
		particleSizes.push_back(ofRandom(1.5, 3.5));
		trails.push_back(ofPolyline());
		particlePhases.push_back(ofRandom(TWO_PI));
		particleColors.push_back(ofColor(255));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.01;
	breathePulse = sin(time * 0.8) * 0.5 + 0.5;

	// Update particle positions with flow field
	for(int i = 0; i < particles.size(); i++){
		// Add organic flow field influence with vortex
		float noiseX = ofNoise(particles[i].x * 0.003, particles[i].y * 0.003, time * 0.5);
		float noiseY = ofNoise(particles[i].x * 0.003 + 100, particles[i].y * 0.003 + 100, time * 0.5);

		// Add subtle circular flow around centers
		float centerX = ofGetWidth() / 2;
		float centerY = ofGetHeight() / 2;
		float dx = particles[i].x - centerX;
		float dy = particles[i].y - centerY;
		float angle = atan2(dy, dx);
		float dist = sqrt(dx * dx + dy * dy);

		ofVec2f flowForce;
		flowForce.x = cos(noiseX * TWO_PI * 2) * 0.3 + sin(angle + HALF_PI) * 0.05;
		flowForce.y = sin(noiseY * TWO_PI * 2) * 0.3 + cos(angle + HALF_PI) * 0.05;

		velocities[i] += flowForce * 0.12;
		velocities[i] *= 0.97; // Damping
		velocities[i].limit(2.5);

		particles[i] += velocities[i];

		// Add to trail - optimized length
		trails[i].addVertex(particles[i].x, particles[i].y);
		if(trails[i].size() > 50){
			trails[i].getVertices().erase(trails[i].getVertices().begin());
		}

		// Wrap around edges
		if(particles[i].x < 0) particles[i].x = ofGetWidth();
		if(particles[i].x > ofGetWidth()) particles[i].x = 0;
		if(particles[i].y < 0) particles[i].y = ofGetHeight();
		if(particles[i].y > ofGetHeight()) particles[i].y = 0;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Fade effect using FBO - less fade for brighter trails
	fbo.begin();
	ofSetColor(0, 0, 0, 1);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// Draw sacred geometry
	drawSacredGeometry();

	// Draw particle trails with beautiful gradient
	for(int i = 0; i < particles.size(); i++){
		if(trails[i].size() > 1){
			for(int j = 0; j < trails[i].size() - 1; j++){
				float alpha = ofMap(j, 0, trails[i].size(), 50, 255);
				float glow = sin(time * 2 + i * 0.1 + j * 0.05) * 30 + alpha;
				ofSetColor(255, glow);
				ofSetLineWidth(particleSizes[i] * 0.5);
				ofDrawLine(trails[i][j], trails[i][j+1]);
			}
		}
	}

	// Draw connection lines with beautiful gradient
	for(int i = 0; i < particles.size(); i++){
		for(int j = i + 1; j < particles.size(); j++){
			float distance = particles[i].distance(particles[j]);
			if(distance < 100){
				float alpha = ofMap(distance, 0, 100, 230, 25);
				float lineWidth = ofMap(distance, 0, 100, 1.4, 0.4);
				ofSetLineWidth(lineWidth);
				ofSetColor(255, alpha);
				ofDrawLine(particles[i], particles[j]);
			}
		}
	}

	// Draw particles with dynamic ethereal glow
	for(int i = 0; i < particles.size(); i++){
		float pulse = sin(time * 1.5 + particlePhases[i]) * 0.3 + 1;
		float twinkle = sin(time * 3 + i * 0.2) * 0.2 + 0.8;

		// Outermost glow with twinkle
		ofSetColor(255, 60 * twinkle);
		ofDrawCircle(particles[i], particleSizes[i] * 4 * pulse);

		// Outer ethereal glow
		ofSetColor(255, 120 * twinkle);
		ofDrawCircle(particles[i], particleSizes[i] * 2.5 * pulse);

		// Middle glow
		ofSetColor(255, 200 * twinkle);
		ofDrawCircle(particles[i], particleSizes[i] * 1.5 * pulse);

		// Core light with burst
		float burst = sin(time * 4 + i * 0.3) * 0.3 + 1;
		ofSetColor(255, 255);
		ofDrawCircle(particles[i], particleSizes[i] * pulse * burst);
	}

	fbo.end();

	// Draw the FBO to screen
	ofSetColor(255);
	fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::drawSacredGeometry(){
	ofNoFill();

	// Draw 4 sacred geometry centers with breathing animation
	int numCenters = 4;
	float spacing = ofGetWidth() / (numCenters + 1);

	for(int center = 0; center < numCenters; center++){
		float xPos = spacing * (center + 1);
		float yPos = ofGetHeight() / 2 + sin(time * 1.2 + center) * 8;
		float phaseOffset = center * 0.5;
		float scale = 0.45 + sin(time * 0.7 + phaseOffset) * 0.08;
		float rotationSpeed = (center % 2 == 0) ? 1 : -1;

		ofPushMatrix();
		ofTranslate(xPos, yPos);
		ofRotateDeg(time * 3 * rotationSpeed);
		ofTranslate(-xPos, -yPos);
		drawSacredGeometryAtPosition(xPos, yPos, scale);
		ofPopMatrix();

		// Add torus energy fields with pulsing
		drawTorusField(xPos, yPos, (85 + breathePulse * 10) * scale, scale);
	}

	// Draw flowing energy waves with shimmer
	for(float x = 0; x < ofGetWidth(); x += 6){
		float y = ofGetHeight() / 2 + sin(x * 0.01 + time * 2) * 20;
		float alpha = 160 + sin(x * 0.02 + time) * 80;
		float size = 1.2 + sin(x * 0.05 + time * 3) * 0.6;
		float shimmer = sin(x * 0.03 + time * 5) * 0.3 + 1;
		ofSetColor(255, alpha * shimmer);
		ofDrawCircle(x, y, size);
	}

	// Upper wave with dynamic sparkles
	for(float x = 0; x < ofGetWidth(); x += 6){
		float y = ofGetHeight() / 2 - 80 + sin(x * 0.008 + time * 1.5) * 12;
		float alpha = 140 + sin(x * 0.015 + time * 1.2) * 70;
		float size = 1 + cos(x * 0.06 + time * 2.5) * 0.5;
		float twinkle = cos(x * 0.08 + time * 4) * 0.4 + 0.6;
		ofSetColor(255, alpha * twinkle);
		ofDrawCircle(x, y, size);
	}

	// Lower wave with sparkles
	for(float x = 0; x < ofGetWidth(); x += 6){
		float y = ofGetHeight() / 2 + 80 + cos(x * 0.012 + time * 1.8) * 12;
		float alpha = 140 + cos(x * 0.018 + time * 0.8) * 70;
		float size = 1 + sin(x * 0.07 + time * 2) * 0.5;
		float twinkle = sin(x * 0.09 + time * 3.5) * 0.4 + 0.6;
		ofSetColor(255, alpha * twinkle);
		ofDrawCircle(x, y, size);
	}

	// Add interconnecting light beams with traveling pulses
	ofSetLineWidth(0.4);
	for(int i = 0; i < numCenters - 1; i++){
		float x1 = spacing * (i + 1);
		float x2 = spacing * (i + 2);
		float y1 = ofGetHeight() / 2;

		for(float t = 0; t <= 1; t += 0.04){
			float x = ofLerp(x1, x2, t);
			float y = y1 + sin(t * PI + time * 0.8) * 25;
			float alpha = 100 + sin(t * TWO_PI + time * 2) * 60;

			// Traveling pulse effect
			float pulse = sin((t - time * 0.5 + i) * TWO_PI * 2);
			float pulseBoost = max(0.0f, pulse) * 100;

			ofSetColor(255, alpha + pulseBoost);
			float pulseSize = 0.8 + max(0.0f, pulse) * 1.2;
			ofDrawCircle(x, y, pulseSize);
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawSacredGeometryAtPosition(float x, float y, float scale){
	ofPushMatrix();
	ofTranslate(x, y);

	// Central Flower of Life
	float centerPulse = sin(time * 0.8) * 0.15 + 1;
	drawFlowerOfLife(0, 0, 60 * centerPulse * scale, 6);

	// Metatron's Cube rotating slowly
	ofPushMatrix();
	ofRotateDeg(time * 5);
	drawMetatronsCube(0, 0, 140 * scale);
	ofPopMatrix();

	// Outer rotating mandalas
	for(int i = 0; i < 3; i++){
		ofPushMatrix();
		float angle = time * 0.3 + i * TWO_PI / 3;
		float radius = (280 + sin(time + i) * 30) * scale;
		ofTranslate(cos(angle) * radius, sin(angle) * radius);
		ofRotateDeg(time * 10 + i * 120);
		drawFlowerOfLife(0, 0, 35 * scale, 6);
		ofPopMatrix();
	}

	// Sri Yantra inspired triangles
	for(int layer = 0; layer < 3; layer++){
		for(int i = 0; i < 9; i++){
			float angle = i * TWO_PI / 9 + time * 0.15;
			float r = (200 + layer * 40) * scale;
			float size = (20 - layer * 5) * scale;

			float tx = cos(angle) * r;
			float ty = sin(angle) * r;

			ofSetColor(255, 160 + layer * 45);
			ofSetLineWidth(1.2);

			// Upward triangle
			ofDrawTriangle(
				tx, ty - size,
				tx - size * 0.866, ty + size * 0.5,
				tx + size * 0.866, ty + size * 0.5
			);
		}
	}

	// Fibonacci spiral
	ofSetLineWidth(1.8);
	ofBeginShape();
	for(float t = 0; t < TWO_PI * 3; t += 0.1){
		float spiralRadius = t * 15 * scale;
		float spiralAngle = t + time * 0.2;
		float sx = cos(spiralAngle) * spiralRadius;
		float sy = sin(spiralAngle) * spiralRadius;
		float alpha = ofMap(t, 0, TWO_PI * 3, 240, 110);
		ofSetColor(255, alpha);
		ofVertex(sx, sy);
	}
	ofEndShape();

	// Vesica Piscis pattern
	ofSetLineWidth(1.4);
	for(int i = 0; i < 8; i++){
		float angle = i * TWO_PI / 8;
		float breathe = sin(time * 0.7 + i * 0.5) * 10;
		float r = (120 + breathe) * scale;

		float x1 = cos(angle) * r;
		float y1 = sin(angle) * r;
		float x2 = cos(angle + TWO_PI/8) * r;
		float y2 = sin(angle + TWO_PI/8) * r;

		ofSetColor(255, 180);
		ofDrawCircle(x1, y1, 40 * scale);
		ofDrawLine(x1, y1, x2, y2);
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawTorusField(float x, float y, float radius, float scale){
	ofPushMatrix();
	ofTranslate(x, y);

	// Draw toroidal field lines
	int numRings = 6;
	for(int ring = 0; ring < numRings; ring++){
		float ringRadius = radius + ring * 12 * scale;
		float alpha = ofMap(ring, 0, numRings, 190, 70);

		ofNoFill();
		ofSetColor(255, alpha);
		ofSetLineWidth(0.5);

		// Horizontal ellipse - optimized points
		ofBeginShape();
		for(int i = 0; i <= 360; i += 10){
			float angle = ofDegToRad(i);
			float wave = sin(time * 2 + ring * 0.5 + i * 0.02) * 3;
			float rx = cos(angle) * (ringRadius + wave);
			float ry = sin(angle) * (ringRadius * 0.3 + wave * 0.3);
			ofVertex(rx, ry);
		}
		ofEndShape();

		// Add shimmer points - reduced
		for(int i = 0; i < 8; i++){
			float angle = i * TWO_PI / 8 + time * 0.5;
			float px = cos(angle) * ringRadius;
			float py = sin(angle) * ringRadius * 0.3;
			float shimmer = sin(time * 3 + i + ring) * 0.5 + 0.5;
			ofSetColor(255, alpha * shimmer);
			ofDrawCircle(px, py, 1 * scale);
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawFlowerOfLife(float x, float y, float radius, int petals){
	float alphaPulse = sin(time * 1.5) * 40 + 210;
	ofSetColor(255, alphaPulse);
	ofSetLineWidth(1.4);

	// Center circle with glow
	ofDrawCircle(x, y, radius);

	// Petals with rotating phase
	for(int i = 0; i < petals; i++){
		float angle = i * TWO_PI / petals;
		float px = x + cos(angle) * radius;
		float py = y + sin(angle) * radius;
		float petalAlpha = alphaPulse + sin(time * 2 + i) * 30;
		ofSetColor(255, petalAlpha);
		ofDrawCircle(px, py, radius);
	}
}

//--------------------------------------------------------------
void ofApp::drawMetatronsCube(float x, float y, float radius){
	float alphaPulse = sin(time * 1.2) * 40 + 200;
	ofSetColor(255, alphaPulse);
	ofSetLineWidth(1.2);

	vector<ofVec2f> points;

	// Outer circle points
	for(int i = 0; i < 6; i++){
		float angle = i * TWO_PI / 6;
		points.push_back(ofVec2f(x + cos(angle) * radius, y + sin(angle) * radius));
	}

	// Inner circle points
	for(int i = 0; i < 6; i++){
		float angle = i * TWO_PI / 6 + TWO_PI / 12;
		points.push_back(ofVec2f(x + cos(angle) * radius * 0.5, y + sin(angle) * radius * 0.5));
	}

	// Center point
	points.push_back(ofVec2f(x, y));

	// Draw all interconnections with pulse
	for(int i = 0; i < points.size(); i++){
		for(int j = i + 1; j < points.size(); j++){
			float dist = points[i].distance(points[j]);
			if(dist < radius * 1.2){
				float lineAlpha = alphaPulse + sin(time * 3 + i + j) * 30;
				ofSetColor(255, lineAlpha);
				ofDrawLine(points[i], points[j]);
			}
		}
	}

	// Draw circles at points with glow
	for(int i = 0; i < points.size(); i++){
		float pointGlow = sin(time * 2.5 + i * 0.5) * 1 + 2;
		ofDrawCircle(points[i], pointGlow);
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
