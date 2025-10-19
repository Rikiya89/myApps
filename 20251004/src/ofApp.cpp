#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0);  // Pure black background
	ofSetCircleResolution(64);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	time = 0;
	patternMode = 0;

	particles.clear();

	// Sophisticated grayscale palette with varied brightness levels
	vector<ofColor> monochromeColors = {
		ofColor(255),        // Pure brilliant white
		ofColor(245),        // Bright white
		ofColor(220),        // Light white
		ofColor(180),        // Light gray
		ofColor(140),        // Medium gray
		ofColor(100),        // Darker gray
		ofColor(60),         // Deep gray
	};

	// Initialize particles with sophisticated distribution
	for (int i = 0; i < 400; i++) {
		Particle p;

		// Multi-layered distribution for depth
		if (i < 80) {
			// Tight central core
			float r = ofRandom(100);
			float a = ofRandom(TWO_PI);
			p.pos.x = ofGetWidth() / 2 + cos(a) * r;
			p.pos.y = ofGetHeight() / 2 + sin(a) * r;
		} else if (i < 200) {
			// Mid-range orbital layer
			float r = ofRandom(100, 250);
			float a = ofRandom(TWO_PI);
			p.pos.x = ofGetWidth() / 2 + cos(a) * r;
			p.pos.y = ofGetHeight() / 2 + sin(a) * r;
		} else {
			// Dispersed cosmic particles
			p.pos.x = ofRandom(ofGetWidth());
			p.pos.y = ofRandom(ofGetHeight());
		}

		float speed = ofRandom(0.1, 0.5);
		float angle = ofRandom(TWO_PI);
		p.vel.x = cos(angle) * speed;
		p.vel.y = sin(angle) * speed;

		p.radius = ofRandom(1.2, 9);
		p.angle = ofRandom(TWO_PI);
		p.angleVel = ofRandom(-0.012, 0.012);
		p.life = ofRandom(1.0);
		p.phase = ofRandom(TWO_PI);
		p.type = (int)ofRandom(3);
		p.color = monochromeColors[(int)ofRandom(monochromeColors.size())];
		p.energy = ofRandom(0.6, 1.0);
		particles.push_back(p);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.01;

	// Update particles with flow field
	for (auto& p : particles) {
		// Flow field based on Perlin noise
		float noiseScale = 0.003;
		float noiseStrength = 0.15;
		float noiseX = ofNoise(p.pos.x * noiseScale, p.pos.y * noiseScale, time * 0.5) * TWO_PI * 2;
		float noiseY = ofNoise(p.pos.x * noiseScale + 1000, p.pos.y * noiseScale, time * 0.5) * TWO_PI * 2;

		p.vel.x += cos(noiseX) * noiseStrength;
		p.vel.y += sin(noiseY) * noiseStrength;

		// Attraction to center with gentle force
		ofVec2f center(ofGetWidth() / 2, ofGetHeight() / 2);
		ofVec2f toCenter = center - p.pos;
		float dist = toCenter.length();
		if (dist > 0) {
			toCenter.normalize();
			float centerForce = ofMap(dist, 0, 500, 0, 0.05, true);
			p.vel += toCenter * centerForce * 0.1;
		}

		p.vel *= 0.97; // Smooth damping
		p.pos += p.vel;
		p.angle += p.angleVel;
		p.life += 0.003;

		// Wrap around screen
		if (p.pos.x < -20) p.pos.x = ofGetWidth() + 20;
		if (p.pos.x > ofGetWidth() + 20) p.pos.x = -20;
		if (p.pos.y < -20) p.pos.y = ofGetHeight() + 20;
		if (p.pos.y > ofGetHeight() + 20) p.pos.y = -20;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Sophisticated fade effect with dynamic trails
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 20);  // Smooth black fade
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	// Draw sophisticated web of connections
	ofSetLineWidth(0.3);
	int connectionCount = 0;
	for (int i = 0; i < particles.size(); i++) {
		for (int j = i + 1; j < particles.size(); j++) {
			float dist = particles[i].pos.distance(particles[j].pos);
			if (dist < 130) {
				// Enhanced alpha calculation for more drama
				float alpha = ofMap(dist, 0, 130, 70, 0);
				float weight = ofMap(dist, 0, 130, 1.2, 0.15);
				ofSetLineWidth(weight);

				// Sophisticated color blending with brightness consideration
				ofColor mixedColor = particles[i].color.getLerped(particles[j].color, 0.5);

				// Add subtle pulsing to connections
				float pulse = sin(time * 2.0 + i * 0.1 + j * 0.1) * 0.2 + 0.8;
				mixedColor.a = alpha * pulse;
				ofSetColor(mixedColor);
				ofDrawLine(particles[i].pos, particles[j].pos);

				connectionCount++;
				if (connectionCount > 800) break;  // Limit for performance
			}
		}
		if (connectionCount > 800) break;
	}

	// Draw elegant particles with minimalist design
	for (auto& p : particles) {
		float pulse = sin(time * 1.2 + p.phase) * 0.5 + 0.5;
		float breathe = sin(time * 0.6 + p.life * TWO_PI) * 0.25 + 0.75;
		float glow = sin(time * 2.0 + p.phase) * 0.3 + 0.7;

		ofPushMatrix();
		ofTranslate(p.pos);
		ofRotateDeg(ofRadToDeg(p.angle));

		if (patternMode == 0) {
			// Sophisticated layered halos with shimmer
			ofNoFill();
			for (int i = 0; i < 4; i++) {
				float rad = (p.radius + i * 3.5) * breathe;
				float shimmer = sin(time * 3.0 + p.phase + i * 0.5) * 0.15 + 0.85;
				float alpha = ofMap(i, 0, 3, 220, 35) * pulse * p.energy * shimmer;
				ofColor ringColor = p.color;
				ringColor.a = alpha;
				ofSetColor(ringColor);
				ofSetLineWidth(ofMap(i, 0, 3, 1.8, 0.3));
				ofDrawCircle(0, 0, rad);
			}
			// Brilliant multi-layer core
			ofFill();
			for (int i = 2; i >= 0; i--) {
				ofColor coreColor = p.color;
				coreColor.a = ofMap(i, 0, 2, 255, 100) * glow;
				ofSetColor(coreColor);
				ofDrawCircle(0, 0, p.radius * 0.35 * (1 + i * 0.35) * breathe);
			}
		}
		else if (patternMode == 1) {
			// Ornate flower mandala with intricate details
			ofNoFill();
			ofSetLineWidth(1.1);
			int petals = 8;

			// Outer petal layer
			for (int i = 0; i < petals; i++) {
				float angle = (TWO_PI / petals) * i;
				float x1 = cos(angle) * p.radius * 0.7 * breathe;
				float y1 = sin(angle) * p.radius * 0.7 * breathe;
				float x2 = cos(angle) * p.radius * 1.6 * breathe;
				float y2 = sin(angle) * p.radius * 1.6 * breathe;
				float x3 = cos(angle + PI / petals) * p.radius * 1.3 * breathe;
				float y3 = sin(angle + PI / petals) * p.radius * 1.3 * breathe;

				ofColor petalColor = p.color;
				petalColor.a = 160 * pulse * p.energy;
				ofSetColor(petalColor);
				ofDrawLine(0, 0, x2, y2);

				// Petal outlines
				ofColor outlineColor = p.color;
				outlineColor.a = 120 * pulse;
				ofSetColor(outlineColor);
				ofDrawLine(x1, y1, x2, y2);
				ofDrawLine(x2, y2, x3, y3);
			}

			// Inner geometric star
			ofSetLineWidth(0.8);
			for (int i = 0; i < petals; i++) {
				float angle = (TWO_PI / petals) * i + PI / petals;
				ofColor innerColor = p.color;
				innerColor.a = 90 * pulse;
				ofSetColor(innerColor);
				ofDrawLine(0, 0, cos(angle) * p.radius * 0.5 * breathe,
				          sin(angle) * p.radius * 0.5 * breathe);
			}

			// Bright center with glow
			ofFill();
			ofColor centerGlow = ofColor(255);
			centerGlow.a = 180 * glow;
			ofSetColor(centerGlow);
			ofDrawCircle(0, 0, p.radius * 0.4);
			ofColor centerCore = p.color;
			centerCore.a = 255 * glow;
			ofSetColor(centerCore);
			ofDrawCircle(0, 0, p.radius * 0.25);
		}
		else {
			// Complex crystalline sacred geometry
			ofNoFill();
			ofSetLineWidth(0.6);

			// Outer hexagon layer
			int sides = 6;
			ofColor outerColor = p.color;
			outerColor.a = 65 * pulse * p.energy;
			ofSetColor(outerColor);
			ofPushMatrix();
			ofRotateDeg(ofRadToDeg(p.angle) * 0.4);
			ofBeginShape();
			for (int i = 0; i < sides; i++) {
				float angle = (TWO_PI / sides) * i;
				ofVertex(cos(angle) * p.radius * 1.4 * breathe,
				        sin(angle) * p.radius * 1.4 * breathe);
			}
			ofEndShape(true);
			ofPopMatrix();

			// Mid-layer rotating square
			ofColor midColor = p.color;
			midColor.a = 80 * pulse;
			ofSetColor(midColor);
			ofSetLineWidth(0.5);
			ofPushMatrix();
			ofRotateDeg(-ofRadToDeg(p.angle) * 0.4 + 22.5);
			ofBeginShape();
			for (int i = 0; i < 4; i++) {
				float angle = (TWO_PI / 4) * i;
				ofVertex(cos(angle) * p.radius * 0.9 * breathe,
				        sin(angle) * p.radius * 0.9 * breathe);
			}
			ofEndShape(true);
			ofPopMatrix();

			// Inner triangle
			ofColor innerColor = p.color;
			innerColor.a = 95 * pulse;
			ofSetColor(innerColor);
			ofSetLineWidth(0.4);
			ofPushMatrix();
			ofRotateDeg(ofRadToDeg(p.angle) * 0.6);
			ofBeginShape();
			for (int i = 0; i < 3; i++) {
				float angle = (TWO_PI / 3) * i - HALF_PI;
				ofVertex(cos(angle) * p.radius * 0.5 * breathe,
				        sin(angle) * p.radius * 0.5 * breathe);
			}
			ofEndShape(true);
			ofPopMatrix();

			// Brilliant center point with radial lines
			ofSetLineWidth(0.3);
			for (int i = 0; i < 6; i++) {
				float angle = (TWO_PI / 6) * i;
				ofColor rayColor = p.color;
				rayColor.a = 40 * pulse;
				ofSetColor(rayColor);
				ofDrawLine(0, 0, cos(angle) * p.radius * 0.3,
				          sin(angle) * p.radius * 0.3);
			}

			ofFill();
			ofColor centerColor = ofColor(255);
			centerColor.a = 180 * glow;
			ofSetColor(centerColor);
			ofDrawCircle(0, 0, p.radius * 0.2);
		}

		ofPopMatrix();
	}

	// Draw breathtaking background architecture
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(1.2);

	float centerX = ofGetWidth() / 2;
	float centerY = ofGetHeight() / 2;

	// Flowing orbital rings with dynamic movement
	int numSets = 4;
	for (int s = 0; s < numSets; s++) {
		float yOffset = ofGetHeight() / (numSets + 1) * (s + 1) + sin(time * 0.35 + s * 0.6) * 40;
		float xOffset = centerX + sin(time * 0.2 + s * 1.2) * 25;

		for (int i = 0; i < 6; i++) {
			float radius = (i + 1) * 38 + sin(time * 0.6 + i * 0.25 + s * 0.4) * 18;
			float alpha = ofMap(i, 0, 5, 32, 4);

			// Subtle brightness variation
			float brightness = 255 - i * 15;
			ofColor ringColor = ofColor(brightness, alpha);
			ofSetColor(ringColor);
			ofDrawCircle(xOffset, yOffset, radius);
		}
	}

	// Majestic central mandala
	ofPushMatrix();
	ofTranslate(centerX, centerY);

	// Multi-layered rotating sacred geometry
	int sides = 12;
	float baseRadius = 160;

	for (int layer = 0; layer < 5; layer++) {
		float radius = baseRadius + layer * 55 + sin(time * 0.8 + layer * 0.4) * 22;

		// Dynamic brightness and alpha
		float brightness = 255 - layer * 20;
		float alpha = ofMap(layer, 0, 4, 38, 6);
		ofColor layerColor = ofColor(brightness, alpha);

		ofSetColor(layerColor);
		ofRotateDeg(time * 2.2 + layer * 18);

		// Draw intricate polygon
		ofSetLineWidth(1.5);
		ofBeginShape();
		for (int i = 0; i <= sides; i++) {
			float angle = (TWO_PI / sides) * i;
			float x = cos(angle) * radius;
			float y = sin(angle) * radius;
			ofVertex(x, y);
		}
		ofEndShape();

		// Inner star patterns
		if (layer == 0 || layer == 2 || layer == 4) {
			ofSetLineWidth(1.0);
			for (int i = 0; i < sides; i++) {
				float angle = (TWO_PI / sides) * i;
				float angle2 = (TWO_PI / sides) * ((i + 2) % sides);
				ofColor starColor = ofColor(brightness, alpha * 0.6);
				ofSetColor(starColor);
				ofDrawLine(cos(angle) * radius * 0.85, sin(angle) * radius * 0.85,
				          cos(angle2) * radius * 0.85, sin(angle2) * radius * 0.85);
			}
		}

		// Radial connections for specific layers
		if (layer == 1 || layer == 3) {
			ofSetLineWidth(0.8);
			for (int i = 0; i < sides; i += 3) {
				float angle = (TWO_PI / sides) * i;
				ofColor lineColor = ofColor(255, alpha * 0.5);
				ofSetColor(lineColor);
				ofDrawLine(0, 0, cos(angle) * radius * 0.3, sin(angle) * radius * 0.3);
			}
		}
	}

	// Radiant central nucleus with dynamic glow (hollow)
	ofNoFill();
	for (int i = 3; i >= 0; i--) {
		float centerPulse = ofMap(i, 0, 3, 220, 100) + 60 * sin(time * 1.4);
		ofColor centerColor = ofColor(255, centerPulse);
		ofSetColor(centerColor);
		ofSetLineWidth(1.5 - i * 0.3);
		ofDrawCircle(0, 0, 7 + i * 2.5);
	}

	ofPopMatrix();
	ofPopStyle();

	// Elegant UI with enhanced styling
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255, 220);
	string modeNames[] = {"Layered Halos", "Ornate Mandalas", "Crystalline Geometry"};
	ofDrawBitmapString("SPACE: Transform  |  R: Regenerate  |  " + modeNames[patternMode], 20, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		patternMode = (patternMode + 1) % 3;
	}
	if (key == 'r' || key == 'R') {
		setup();
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
