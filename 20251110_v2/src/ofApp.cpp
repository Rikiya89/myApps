#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetCircleResolution(50);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	font.load("arial.ttf", 42, true, true);

	text = "CREATE YOUR OWN MAGIC // DREAM BIG WORK HARD STAY HUMBLE // EVERY MOMENT MATTERS // BE THE ENERGY YOU WANT TO ATTRACT";
	time = 0;
	animationMode = 0;
	mouseInteraction = false;
	mousePos = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);

	// Initialize letters with multi-line layout
	vector<string> words;
	string word = "";
	for(int i = 0; i < text.length(); i++){
		if(text[i] == ' '){
			if(word.length() > 0){
				words.push_back(word);
				word = "";
			}
		} else {
			word += text[i];
		}
	}
	if(word.length() > 0){
		words.push_back(word);
	}

	// Layout words across multiple lines
	float maxWidth = ofGetWidth() - 100;
	float lineHeight = 80;
	float xPos = 50;
	float yPos = 200;
	float lineWidth = 0;
	int letterIndex = 0;

	for(int w = 0; w < words.size(); w++){
		// Calculate word width
		float wordWidth = 0;
		for(int c = 0; c < words[w].length(); c++){
			ofRectangle bbox = font.getStringBoundingBox(string(1, words[w][c]), 0, 0);
			wordWidth += bbox.width + 6;
		}

		// Check if word fits on current line
		if(lineWidth + wordWidth > maxWidth && lineWidth > 0){
			// Move to next line
			xPos = 50;
			yPos += lineHeight;
			lineWidth = 0;
		}

		// Place letters of this word
		for(int c = 0; c < words[w].length(); c++){
			Letter letter;
			letter.character = words[w][c];
			letter.position = ofVec2f(xPos, yPos);
			letter.originalPos = letter.position;
			letter.targetPos = letter.position;
			letter.velocity = ofVec2f(ofRandom(-3, 3), ofRandom(-3, 3));
			letter.rotation = 0;
			letter.rotationSpeed = ofRandom(-3, 3);
			letter.scale = 1.0;
			letter.alpha = 255;
			letter.hue = letterIndex * 5;

			letters.push_back(letter);

			ofRectangle bbox = font.getStringBoundingBox(string(1, words[w][c]), 0, 0);
			xPos += bbox.width + 6;
			lineWidth += bbox.width + 6;
			letterIndex++;
		}

		// Add space after word
		xPos += 20;
		lineWidth += 20;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	time += 0.016;

	// Update particles
	for(int i = particles.size() - 1; i >= 0; i--){
		particles[i].position += particles[i].velocity;
		particles[i].velocity *= 0.98;
		particles[i].life -= 0.02;
		particles[i].alpha = particles[i].life * 255;

		if(particles[i].life <= 0){
			particles.erase(particles.begin() + i);
		}
	}

	for(int i = 0; i < letters.size(); i++){
		// Mouse interaction
		if(mouseInteraction){
			ofVec2f diff = letters[i].position - mousePos;
			float distance = diff.length();
			if(distance < 200){
				float force = (200 - distance) / 200.0;
				letters[i].velocity += diff.getNormalized() * force * 3;
			}
		}

		switch(animationMode){
			case 0: // Flowing wave with depth
			{
				letters[i].targetPos.x = letters[i].originalPos.x + sin(time * 1.5 + i * 0.3) * 30;
				letters[i].targetPos.y = letters[i].originalPos.y + sin(time * 2.5 + i * 0.5) * 120;
				letters[i].rotation = sin(time * 1.2 + i * 0.4) * 25;
				letters[i].scale = 1.0 + sin(time * 3 + i * 0.3) * 0.4;
				letters[i].alpha = 200 + sin(time * 2 + i * 0.2) * 55;
				break;
			}
			case 1: // Explosive scatter and reform
			{
				float cycle = sin(time * 0.4);
				float easing = (1 - cos(cycle * PI)) / 2;

				if(cycle > 0){
					float angle = (float)i / letters.size() * TWO_PI * 3 + time;
					float radius = 250 * easing;
					letters[i].targetPos.x = ofGetWidth() / 2 + cos(angle) * radius;
					letters[i].targetPos.y = ofGetHeight() / 2 + sin(angle) * radius;
					letters[i].scale = 1.5 - easing * 0.7;
				} else {
					letters[i].targetPos = letters[i].originalPos;
					letters[i].scale = 1.0 + abs(cycle) * 0.3;
				}
				letters[i].rotation = sin(time * 3 + i) * 360 * easing;
				letters[i].alpha = 150 + easing * 105;
				break;
			}
			case 2: // 3D rotating spiral
			{
				float angle = time * 0.8 + i * 0.4;
				float radius = 180 + sin(time * 0.5 + i * 0.15) * 80;
				float depth = cos(time + i * 0.3) * 100;

				letters[i].targetPos.x = ofGetWidth() / 2 + cos(angle) * radius;
				letters[i].targetPos.y = ofGetHeight() / 2 + sin(angle) * radius + depth;
				letters[i].rotation = angle * 57.29578 + depth * 0.5;
				letters[i].scale = 0.6 + (depth + 100) / 200.0 * 0.8;
				letters[i].alpha = 120 + (depth + 100) / 200.0 * 135;
				break;
			}
			case 3: // Organic particle flow
			{
				letters[i].velocity.x += ofRandom(-0.8, 0.8) + sin(time + i * 0.1) * 0.3;
				letters[i].velocity.y += ofRandom(-0.8, 0.8) + cos(time * 1.3 + i * 0.1) * 0.3;
				letters[i].velocity *= 0.92;
				letters[i].position += letters[i].velocity;

				// Bouncing with damping
				if(letters[i].position.x < 50 || letters[i].position.x > ofGetWidth() - 50){
					letters[i].velocity.x *= -0.8;
					letters[i].position.x = ofClamp(letters[i].position.x, 50, ofGetWidth() - 50);
				}
				if(letters[i].position.y < 50 || letters[i].position.y > ofGetHeight() - 50){
					letters[i].velocity.y *= -0.8;
					letters[i].position.y = ofClamp(letters[i].position.y, 50, ofGetHeight() - 50);
				}
				letters[i].rotation += letters[i].rotationSpeed;
				letters[i].scale = 0.8 + abs(sin(time * 2 + i * 0.5)) * 0.6;
				letters[i].alpha = 180 + sin(time + i) * 75;

				// Create particles
				if(ofRandom(1) < 0.03){
					Particle p;
					p.position = letters[i].position;
					p.velocity = ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1));
					p.life = 1.0;
					p.size = ofRandom(2, 6);
					p.alpha = 255;
					particles.push_back(p);
				}
				continue;
			}
			case 4: // Vortex attraction
			{
				ofVec2f center(ofGetWidth() / 2, ofGetHeight() / 2);
				ofVec2f toCenter = center - letters[i].position;
				float distance = toCenter.length();
				float angle = atan2(toCenter.y, toCenter.x);

				float targetDist = 200 + sin(time + i * 0.2) * 80;
				angle += time * 0.5 + i * 0.3;

				letters[i].targetPos.x = center.x + cos(angle) * targetDist;
				letters[i].targetPos.y = center.y + sin(angle) * targetDist;
				letters[i].rotation = angle * 57.29578 + time * 50;
				letters[i].scale = 0.7 + sin(time * 2 + i) * 0.5;
				letters[i].alpha = 150 + cos(time + i * 0.5) * 105;
				break;
			}
		}

		// Add trails
		letters[i].trail.push_back(letters[i].position);
		if(letters[i].trail.size() > 8){
			letters[i].trail.erase(letters[i].trail.begin());
		}

		// Smooth movement towards target with easing
		if(animationMode != 3){
			ofVec2f diff = letters[i].targetPos - letters[i].position;
			letters[i].velocity += diff * 0.05;
			letters[i].velocity *= 0.85;
			letters[i].position += letters[i].velocity;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Fade effect for motion blur
	ofSetColor(0, 0, 0, 25);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// Draw background grid lines
	ofSetColor(30, 30, 30);
	ofSetLineWidth(1);
	for(int i = 0; i < ofGetWidth(); i += 60){
		float offset = sin(time * 0.5 + i * 0.01) * 10;
		ofDrawLine(i, 0, i, ofGetHeight());
	}
	for(int i = 0; i < ofGetHeight(); i += 60){
		float offset = cos(time * 0.5 + i * 0.01) * 10;
		ofDrawLine(0, i, ofGetWidth(), i);
	}

	// Draw connecting lines between letters
	if(animationMode == 2 || animationMode == 4){
		ofSetLineWidth(1);
		for(int i = 0; i < letters.size() - 1; i++){
			float distance = letters[i].position.distance(letters[i+1].position);
			if(distance < 150){
				float alpha = ofMap(distance, 0, 150, 100, 0);
				ofSetColor(255, alpha);
				ofDrawLine(letters[i].position, letters[i+1].position);
			}
		}
	}

	// Draw particles
	for(int i = 0; i < particles.size(); i++){
		ofSetColor(255, particles[i].alpha);
		ofDrawCircle(particles[i].position, particles[i].size);
	}

	// Draw letter trails
	for(int i = 0; i < letters.size(); i++){
		for(int j = 0; j < letters[i].trail.size(); j++){
			float alpha = (float)j / letters[i].trail.size() * letters[i].alpha * 0.3;
			float size = (float)j / letters[i].trail.size() * letters[i].scale * 0.5;
			ofSetColor(255, alpha);
			ofDrawCircle(letters[i].trail[j], 3 * size);
		}
	}

	// Draw each letter with transformations
	for(int i = 0; i < letters.size(); i++){
		ofPushMatrix();

		ofTranslate(letters[i].position.x, letters[i].position.y);
		ofRotateDeg(letters[i].rotation);
		ofScale(letters[i].scale, letters[i].scale);

		// Draw shadow for depth
		ofSetColor(50, letters[i].alpha * 0.5);
		ofRectangle bbox = font.getStringBoundingBox(string(1, letters[i].character), 0, 0);
		font.drawString(string(1, letters[i].character), -bbox.width/2 + 3, bbox.height/2 + 3);

		// White text with variable alpha
		ofSetColor(255, letters[i].alpha);
		font.drawString(string(1, letters[i].character), -bbox.width/2, bbox.height/2);

		// Draw glow effect
		ofSetColor(255, letters[i].alpha * 0.2);
		ofNoFill();
		ofSetLineWidth(2);
		ofDrawCircle(0, 0, 30 + sin(time * 3 + i) * 5);
		ofFill();

		ofPopMatrix();
	}

	// Draw mouse interaction zone
	if(mouseInteraction){
		ofSetColor(255, 30);
		ofNoFill();
		ofSetLineWidth(2);
		ofDrawCircle(mousePos, 200);
		ofFill();
	}
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == '1'){
		animationMode = 0;
	} else if(key == '2'){
		animationMode = 1;
	} else if(key == '3'){
		animationMode = 2;
	} else if(key == '4'){
		animationMode = 3;
	} else if(key == '5'){
		animationMode = 4;
	} else if(key == ' '){
		mouseInteraction = !mouseInteraction;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	mousePos.set(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	mousePos.set(x, y);
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
