#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	time = 0;

	// Setup moon
	moonPos.set(360, 200);
	moonGlow = 0;

	// Create ghosts
	for(int i = 0; i < 8; i++){
		Ghost ghost;
		ghost.pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		ghost.vel.set(ofRandom(-0.5, 0.5), ofRandom(-1, -0.2));
		ghost.size = ofRandom(30, 70);
		ghost.alpha = ofRandom(100, 200);
		ghost.phase = ofRandom(TWO_PI);
		ghost.floatSpeed = ofRandom(0.01, 0.03);
		ghosts.push_back(ghost);
	}

	// Create skulls
	for(int i = 0; i < 5; i++){
		Skull skull;
		skull.pos.set(ofRandom(100, ofGetWidth()-100), ofRandom(300, 1000));
		skull.rotation = ofRandom(360);
		skull.rotationSpeed = ofRandom(-0.5, 0.5);
		skull.size = ofRandom(40, 80);
		skull.alpha = ofRandom(150, 255);
		skull.bobPhase = ofRandom(TWO_PI);
		skulls.push_back(skull);
	}

	// Create spiders
	for(int i = 0; i < 6; i++){
		Spider spider;
		spider.pos.set(ofRandom(50, ofGetWidth()-50), ofRandom(50, 300));
		spider.threadLength = ofRandom(100, 300);
		spider.swingPhase = ofRandom(TWO_PI);
		spider.swingSpeed = ofRandom(0.02, 0.05);
		spider.size = ofRandom(15, 30);
		spiders.push_back(spider);
	}

	// Create bats
	for(int i = 0; i < 12; i++){
		batPositions.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(100, 600)));
		batPhases.push_back(ofRandom(TWO_PI));
	}

	// Create floating candles
	for(int i = 0; i < 8; i++){
		Candle candle;
		candle.pos.set(ofRandom(100, ofGetWidth()-100), ofRandom(400, 1100));
		candle.height = ofRandom(30, 50);
		candle.flameFlicker = ofRandom(TWO_PI);
		candle.flickerSpeed = ofRandom(0.1, 0.3);
		candle.glowIntensity = ofRandom(0.5, 1.0);
		candles.push_back(candle);
	}

	// Create creepy eyes in the darkness
	for(int i = 0; i < 10; i++){
		CreepyEyes eyes;
		eyes.pos.set(ofRandom(50, ofGetWidth()-50), ofRandom(200, 1200));
		eyes.blinkPhase = ofRandom(TWO_PI);
		eyes.blinkSpeed = ofRandom(0.01, 0.03);
		eyes.isBlinking = false;
		eyes.eyeDistance = ofRandom(15, 25);
		creepyEyes.push_back(eyes);
	}

	// Create pentagrams
	for(int i = 0; i < 3; i++){
		Pentagram pentagram;
		pentagram.pos.set(ofRandom(150, ofGetWidth()-150), ofRandom(800, 1150));
		pentagram.size = ofRandom(40, 70);
		pentagram.rotationPhase = ofRandom(TWO_PI);
		pentagram.rotationSpeed = ofRandom(0.005, 0.015);
		pentagram.glowPhase = ofRandom(TWO_PI);
		pentagrams.push_back(pentagram);
	}

	// Create coffins
	for(int i = 0; i < 2; i++){
		Coffin coffin;
		coffin.pos.set(ofRandom(150, ofGetWidth()-150), ofRandom(900, 1100));
		coffin.width = ofRandom(50, 80);
		coffin.height = ofRandom(120, 160);
		coffin.shakePhase = ofRandom(TWO_PI);
		coffin.rotation = ofRandom(-15, 15);
		coffins.push_back(coffin);
	}

	// Create clouds
	for(int i = 0; i < 6; i++){
		cloudPositions.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(50, 400)));
		cloudSpeeds.push_back(ofRandom(0.2, 0.6));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.016;
	moonGlow = sin(time * 0.5) * 30 + 50;

	// Update ghosts
	for(auto& ghost : ghosts){
		ghost.pos += ghost.vel;
		ghost.pos.x += sin(ghost.phase) * 2;
		ghost.phase += ghost.floatSpeed;

		// Add to trail
		ghost.trail.push_back(ghost.pos);
		if(ghost.trail.size() > 20){
			ghost.trail.erase(ghost.trail.begin());
		}

		// Wrap around screen
		if(ghost.pos.y < -ghost.size){
			ghost.pos.y = ofGetHeight() + ghost.size;
			ghost.pos.x = ofRandom(ofGetWidth());
		}
		if(ghost.pos.x < -ghost.size) ghost.pos.x = ofGetWidth() + ghost.size;
		if(ghost.pos.x > ofGetWidth() + ghost.size) ghost.pos.x = -ghost.size;
	}

	// Update skulls
	for(auto& skull : skulls){
		skull.rotation += skull.rotationSpeed;
		skull.bobPhase += 0.02;
		skull.pos.y += sin(skull.bobPhase) * 0.5;
	}

	// Update spiders
	for(auto& spider : spiders){
		spider.swingPhase += spider.swingSpeed;
	}

	// Update bats
	for(int i = 0; i < batPositions.size(); i++){
		batPhases[i] += 0.05;
		batPositions[i].x += sin(batPhases[i]) * 3;
		batPositions[i].y += cos(batPhases[i] * 0.5) * 2;

		// Keep bats in bounds
		if(batPositions[i].x < -50){
			batPositions[i].x = ofGetWidth() + 50;
		}
		if(batPositions[i].x > ofGetWidth() + 50){
			batPositions[i].x = -50;
		}
	}

	// Update candles
	for(auto& candle : candles){
		candle.flameFlicker += candle.flickerSpeed;
		candle.pos.y += sin(time + candle.flameFlicker) * 0.3;
	}

	// Update creepy eyes (blinking)
	for(auto& eyes : creepyEyes){
		eyes.blinkPhase += eyes.blinkSpeed;
		if(sin(eyes.blinkPhase) > 0.95){
			eyes.isBlinking = true;
		} else {
			eyes.isBlinking = false;
		}
	}

	// Update pentagrams
	for(auto& pentagram : pentagrams){
		pentagram.rotationPhase += pentagram.rotationSpeed;
		pentagram.glowPhase += 0.03;
	}

	// Update coffins (shaking)
	for(auto& coffin : coffins){
		coffin.shakePhase += 0.05;
	}

	// Update clouds
	for(int i = 0; i < cloudPositions.size(); i++){
		cloudPositions[i].x += cloudSpeeds[i];
		if(cloudPositions[i].x > ofGetWidth() + 100){
			cloudPositions[i].x = -100;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	// Draw clouds
	drawClouds();

	// Draw moon with glow
	drawMoon();

	// Draw creepy eyes in the background
	for(auto& eyes : creepyEyes){
		drawCreepyEyes(eyes);
	}

	// Draw pentagrams
	for(auto& pentagram : pentagrams){
		drawPentagram(pentagram);
	}

	// Draw coffins
	for(auto& coffin : coffins){
		drawCoffin(coffin);
	}

	// Draw spiders
	for(auto& spider : spiders){
		drawSpider(spider);
	}

	// Draw candles
	for(auto& candle : candles){
		drawCandle(candle);
	}

	// Draw ghosts
	for(auto& ghost : ghosts){
		drawGhost(ghost);
	}

	// Draw skulls
	for(auto& skull : skulls){
		drawSkull(skull);
	}

	// Draw bats
	drawBats();
}

//--------------------------------------------------------------
void ofApp::drawGhost(Ghost& ghost){
	ofPushMatrix();
	ofTranslate(ghost.pos);

	// Draw ethereal trail
	for(int i = 0; i < ghost.trail.size()-1; i++){
		float alpha = ofMap(i, 0, ghost.trail.size(), 0, ghost.alpha * 0.3);
		ofSetColor(255, alpha);
		ofVec2f trailPos = ghost.trail[i] - ghost.pos;
		ofDrawCircle(trailPos, ghost.size * 0.3);
	}

	// Ghost body (wispy and organic)
	ofSetColor(255, ghost.alpha);
	ofBeginShape();
	for(int i = 0; i < 360; i += 20){
		float rad = ofDegToRad(i);
		float noise = ofNoise(i * 0.1, time * 0.5, ghost.phase);
		float r = ghost.size * (0.8 + noise * 0.4);
		float x = cos(rad) * r;
		float y = sin(rad) * r - ghost.size * 0.2;
		ofVertex(x, y);
	}
	ofEndShape(true);

	// Ghost face - eyes
	ofSetColor(0);
	float eyeOffset = ghost.size * 0.25;
	ofDrawCircle(-eyeOffset, -ghost.size * 0.1, ghost.size * 0.15);
	ofDrawCircle(eyeOffset, -ghost.size * 0.1, ghost.size * 0.15);

	// Ghost mouth (wavy)
	ofNoFill();
	ofSetLineWidth(3);
	ofBeginShape();
	for(float x = -ghost.size * 0.3; x <= ghost.size * 0.3; x += 5){
		float y = ghost.size * 0.2 + sin(x * 0.2 + time) * 5;
		ofVertex(x, y);
	}
	ofEndShape();
	ofFill();

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawSkull(Skull& skull){
	ofPushMatrix();
	ofTranslate(skull.pos);
	ofRotateDeg(skull.rotation);

	ofSetColor(255, skull.alpha);

	// Skull head
	ofDrawCircle(0, 0, skull.size * 0.6);
	ofDrawRectangle(-skull.size * 0.5, 0, skull.size, skull.size * 0.7);

	// Jaw
	ofDrawCircle(0, skull.size * 0.5, skull.size * 0.4);

	// Eye sockets
	ofSetColor(0);
	ofDrawCircle(-skull.size * 0.25, -skull.size * 0.1, skull.size * 0.2);
	ofDrawCircle(skull.size * 0.25, -skull.size * 0.1, skull.size * 0.2);

	// Nose hole
	ofBeginShape();
	ofVertex(0, skull.size * 0.15);
	ofVertex(-skull.size * 0.1, skull.size * 0.3);
	ofVertex(skull.size * 0.1, skull.size * 0.3);
	ofEndShape(true);

	// Teeth
	float teethY = skull.size * 0.5;
	for(int i = -2; i <= 2; i++){
		ofDrawRectangle(i * skull.size * 0.15, teethY, skull.size * 0.1, skull.size * 0.15);
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawSpider(Spider& spider){
	// Thread
	ofSetColor(255, 150);
	ofSetLineWidth(1);
	ofDrawLine(spider.pos.x, 0, spider.pos.x, spider.pos.y + spider.threadLength);

	// Spider body position with swing
	float swingX = sin(spider.swingPhase) * 50;
	float spiderX = spider.pos.x + swingX;
	float spiderY = spider.pos.y + spider.threadLength;

	ofPushMatrix();
	ofTranslate(spiderX, spiderY);

	// Spider body
	ofSetColor(255);
	ofDrawCircle(0, 0, spider.size);
	ofDrawCircle(0, -spider.size * 0.7, spider.size * 0.6);

	// Spider legs
	ofSetLineWidth(2);
	for(int i = 0; i < 4; i++){
		float angle = ofMap(i, 0, 3, -60, 60);
		float legLength = spider.size * 1.5;

		// Left legs
		ofPushMatrix();
		ofRotateDeg(angle);
		ofDrawLine(0, 0, -legLength, -legLength * 0.5);
		ofDrawLine(-legLength, -legLength * 0.5, -legLength * 1.3, legLength * 0.3);
		ofPopMatrix();

		// Right legs
		ofPushMatrix();
		ofRotateDeg(-angle);
		ofDrawLine(0, 0, legLength, -legLength * 0.5);
		ofDrawLine(legLength, -legLength * 0.5, legLength * 1.3, legLength * 0.3);
		ofPopMatrix();
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawMoon(){
	// Moon glow layers
	for(int i = 5; i > 0; i--){
		ofSetColor(255, 20);
		ofDrawCircle(moonPos, 80 + moonGlow * i * 0.3);
	}

	// Moon
	ofSetColor(255);
	ofDrawCircle(moonPos, 80);

	// Moon craters
	ofSetColor(200);
	ofDrawCircle(moonPos.x - 20, moonPos.y - 15, 15);
	ofDrawCircle(moonPos.x + 25, moonPos.y + 10, 20);
	ofDrawCircle(moonPos.x + 10, moonPos.y - 25, 12);
	ofDrawCircle(moonPos.x - 15, moonPos.y + 20, 10);
}

//--------------------------------------------------------------
void ofApp::drawBats(){
	for(int i = 0; i < batPositions.size(); i++){
		ofPushMatrix();
		ofTranslate(batPositions[i]);

		ofSetColor(255);

		// Bat body
		ofDrawCircle(0, 0, 8);

		// Wing flap animation
		float wingFlap = sin(batPhases[i] * 4) * 15;

		// Left wing
		ofBeginShape();
		ofVertex(0, 0);
		ofVertex(-15 - wingFlap, -10);
		ofVertex(-25 - wingFlap, -5);
		ofVertex(-20 - wingFlap, 5);
		ofVertex(-10, 0);
		ofEndShape(true);

		// Right wing
		ofBeginShape();
		ofVertex(0, 0);
		ofVertex(15 + wingFlap, -10);
		ofVertex(25 + wingFlap, -5);
		ofVertex(20 + wingFlap, 5);
		ofVertex(10, 0);
		ofEndShape(true);

		// Bat ears
		ofDrawTriangle(-3, -5, -6, -12, -1, -8);
		ofDrawTriangle(3, -5, 6, -12, 1, -8);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::drawCandle(Candle& candle){
	ofPushMatrix();
	ofTranslate(candle.pos);

	// Candle body
	ofSetColor(255);
	ofDrawRectangle(-8, 0, 16, candle.height);

	// Wax drips
	ofBeginShape();
	ofVertex(-8, candle.height * 0.3);
	ofVertex(-10, candle.height * 0.5);
	ofVertex(-8, candle.height * 0.6);
	ofEndShape();

	ofBeginShape();
	ofVertex(8, candle.height * 0.5);
	ofVertex(11, candle.height * 0.7);
	ofVertex(8, candle.height * 0.8);
	ofEndShape();

	// Flame
	float flameHeight = 15 + sin(candle.flameFlicker) * 3;
	float flameWidth = 8 + cos(candle.flameFlicker * 1.5) * 2;

	// Flame glow
	for(int i = 3; i > 0; i--){
		ofSetColor(255, 30 * candle.glowIntensity);
		ofDrawEllipse(0, -flameHeight * 0.5, flameWidth * (1 + i), flameHeight * (1 + i * 0.5));
	}

	// Flame itself
	ofSetColor(255);
	ofDrawEllipse(0, -flameHeight * 0.5, flameWidth, flameHeight);

	// Wick
	ofSetColor(200);
	ofDrawRectangle(-1, 0, 2, 5);

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawCreepyEyes(CreepyEyes& eyes){
	ofSetColor(255);

	if(!eyes.isBlinking){
		// Left eye
		ofDrawCircle(eyes.pos.x - eyes.eyeDistance/2, eyes.pos.y, 6);
		// Right eye
		ofDrawCircle(eyes.pos.x + eyes.eyeDistance/2, eyes.pos.y, 6);

		// Pupils
		ofSetColor(0);
		ofDrawCircle(eyes.pos.x - eyes.eyeDistance/2, eyes.pos.y, 3);
		ofDrawCircle(eyes.pos.x + eyes.eyeDistance/2, eyes.pos.y, 3);

		// Eye glow
		ofSetColor(255, 100);
		ofDrawCircle(eyes.pos.x - eyes.eyeDistance/2, eyes.pos.y, 10);
		ofDrawCircle(eyes.pos.x + eyes.eyeDistance/2, eyes.pos.y, 10);
	} else {
		// Blinking - horizontal lines
		ofSetColor(255);
		ofSetLineWidth(2);
		ofDrawLine(eyes.pos.x - eyes.eyeDistance/2 - 6, eyes.pos.y,
				   eyes.pos.x - eyes.eyeDistance/2 + 6, eyes.pos.y);
		ofDrawLine(eyes.pos.x + eyes.eyeDistance/2 - 6, eyes.pos.y,
				   eyes.pos.x + eyes.eyeDistance/2 + 6, eyes.pos.y);
	}
}

//--------------------------------------------------------------
void ofApp::drawPentagram(Pentagram& pentagram){
	ofPushMatrix();
	ofTranslate(pentagram.pos);
	ofRotateDeg(pentagram.rotationPhase * 57.2958); // Convert to degrees

	// Pentagram glow
	float glow = (sin(pentagram.glowPhase) + 1) * 0.5;
	ofSetColor(255, 50 * glow);
	ofDrawCircle(0, 0, pentagram.size * 1.3);

	// Draw pentagram (5-pointed star)
	ofSetColor(255);
	ofNoFill();
	ofSetLineWidth(2);

	ofBeginShape();
	for(int i = 0; i < 5; i++){
		float angle1 = (i * 144 - 90) * DEG_TO_RAD;
		float angle2 = ((i + 2) % 5 * 144 - 90) * DEG_TO_RAD;

		float x1 = cos(angle1) * pentagram.size;
		float y1 = sin(angle1) * pentagram.size;

		ofVertex(x1, y1);
	}
	ofEndShape(true);

	// Draw connecting lines to complete the star
	for(int i = 0; i < 5; i++){
		float angle1 = (i * 144 - 90) * DEG_TO_RAD;
		float angle2 = ((i + 2) % 5 * 144 - 90) * DEG_TO_RAD;

		float x1 = cos(angle1) * pentagram.size;
		float y1 = sin(angle1) * pentagram.size;
		float x2 = cos(angle2) * pentagram.size;
		float y2 = sin(angle2) * pentagram.size;

		ofDrawLine(x1, y1, x2, y2);
	}

	// Circle around pentagram
	ofDrawCircle(0, 0, pentagram.size);

	ofFill();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawCoffin(Coffin& coffin){
	ofPushMatrix();
	ofTranslate(coffin.pos);
	ofRotateDeg(coffin.rotation);

	// Shake effect
	float shakeX = sin(coffin.shakePhase) * 2;
	float shakeY = cos(coffin.shakePhase * 1.3) * 1;
	ofTranslate(shakeX, shakeY);

	ofSetColor(255);

	// Coffin shape (hexagonal)
	ofBeginShape();
	// Top
	ofVertex(-coffin.width * 0.3, -coffin.height * 0.5);
	ofVertex(coffin.width * 0.3, -coffin.height * 0.5);
	// Shoulders
	ofVertex(coffin.width * 0.5, -coffin.height * 0.2);
	// Bottom
	ofVertex(coffin.width * 0.4, coffin.height * 0.5);
	ofVertex(-coffin.width * 0.4, coffin.height * 0.5);
	// Back to top
	ofVertex(-coffin.width * 0.5, -coffin.height * 0.2);
	ofEndShape(true);

	// Cross on coffin
	ofSetColor(0);
	float crossSize = coffin.width * 0.3;
	ofDrawRectangle(-crossSize * 0.15, -crossSize * 0.5, crossSize * 0.3, crossSize);
	ofDrawRectangle(-crossSize * 0.5, -crossSize * 0.15, crossSize, crossSize * 0.3);

	// Coffin handles
	ofSetColor(255);
	ofDrawCircle(-coffin.width * 0.4, 0, 5);
	ofDrawCircle(coffin.width * 0.4, 0, 5);

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawClouds(){
	ofSetColor(255, 60);

	for(int i = 0; i < cloudPositions.size(); i++){
		ofVec2f pos = cloudPositions[i];

		// Draw cloud using multiple circles
		ofDrawCircle(pos.x, pos.y, 25);
		ofDrawCircle(pos.x + 20, pos.y - 5, 20);
		ofDrawCircle(pos.x - 20, pos.y, 18);
		ofDrawCircle(pos.x + 10, pos.y + 5, 22);
		ofDrawCircle(pos.x - 10, pos.y + 8, 20);
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
