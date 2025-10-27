#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofEnableDepthTest();
	ofEnableLighting();
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	ofSetBackgroundColor(0, 0, 0);
	
	gridSizeX = 15;
	gridSizeY = 15;
	gridSizeZ = 15;
	spacing = 30.0f;
	time = 0.0f;
	noiseScale = 0.008f;
	waveSpeed = 1.5f;
	morphFactor = 1.0f;
	performanceLevel = 1;
	
	pointLight.setPointLight();
	pointLight.setPosition(200, -200, 300);
	pointLight.setDiffuseColor(ofColor(255, 255, 255));
	pointLight.setSpecularColor(ofColor(255, 255, 255));
	pointLight.setAttenuation(0.005f, 0.0005f, 0.00005f);
	
	material.setShininess(80);
	material.setSpecularColor(ofColor(255, 255, 255));
	material.setDiffuseColor(ofColor(255, 255, 255));
	
	grid.resize(gridSizeX);
	for(int x = 0; x < gridSizeX; x++) {
		grid[x].resize(gridSizeY);
		for(int y = 0; y < gridSizeY; y++) {
			grid[x][y].resize(gridSizeZ);
			for(int z = 0; z < gridSizeZ; z++) {
				GridPoint& point = grid[x][y][z];
				point.position = ofVec3f(
					(x - gridSizeX/2) * spacing,
					(y - gridSizeY/2) * spacing,
					(z - gridSizeZ/2) * spacing
				);
				point.velocity = ofVec3f(0, 0, 0);
				point.phase = ofRandom(TWO_PI);
				point.amplitude = ofRandom(10, 40);
				point.brightness = 1.0f;
				point.normal = ofVec3f(0, 0, 1);
			}
		}
	}
	
	particles.reserve(200);
	
	cam.setDistance(1000);
	cam.setNearClip(1.0f);
	cam.setFarClip(5000.0f);
}

//--------------------------------------------------------------
void ofApp::update(){
	time += ofGetLastFrameTime();
	morphFactor = (sin(time * 0.3f) + 1.0f) * 0.5f;
	
	pointLight.setPosition(
		sin(time * 0.5f) * 400,
		cos(time * 0.3f) * 300,
		sin(time * 0.4f) * 200 + 200
	);
	
	updateGrid();
	updateParticles();
	spawnParticles();
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	
	pointLight.enable();
	material.begin();
	
	drawGrid();
	
	if(performanceLevel <= 2) {
		drawRibbons();
		drawSecretarialPatterns();
	}
	
	if(performanceLevel == 1) {
		drawFractalPatterns();
	}
	
	material.end();
	pointLight.disable();
	
	if(performanceLevel <= 2) {
		ofDisableLighting();
		drawParticles();
		ofEnableLighting();
	}
	
	cam.end();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case '1':
			waveSpeed += 0.5f;
			break;
		case '2':
			waveSpeed = max(0.1f, waveSpeed - 0.5f);
			break;
		case '3':
			noiseScale += 0.001f;
			break;
		case '4':
			noiseScale = max(0.001f, noiseScale - 0.001f);
			break;
		case '5':
			spacing += 5.0f;
			break;
		case '6':
			spacing = max(10.0f, spacing - 5.0f);
			break;
		case 'r':
			time = 0;
			for(int x = 0; x < gridSizeX; x++) {
				for(int y = 0; y < gridSizeY; y++) {
					for(int z = 0; z < gridSizeZ; z++) {
						grid[x][y][z].phase = ofRandom(TWO_PI);
					}
				}
			}
			break;
		case 's':
			ofSaveScreen("generative_art_" + ofToString(ofGetFrameNum()) + ".png");
			break;
		case 'b':
			ofSetBackgroundColor(ofGetBackgroundColor() == ofColor::black ? ofColor(20, 20, 20) : ofColor::black);
			break;
		case 'p':
			performanceLevel = (performanceLevel % 3) + 1;
			if(performanceLevel == 1) {
				gridSizeX = gridSizeY = gridSizeZ = 15;
			} else if(performanceLevel == 2) {
				gridSizeX = gridSizeY = gridSizeZ = 10;
			} else {
				gridSizeX = gridSizeY = gridSizeZ = 8;
			}
			
			grid.clear();
			grid.resize(gridSizeX);
			for(int x = 0; x < gridSizeX; x++) {
				grid[x].resize(gridSizeY);
				for(int y = 0; y < gridSizeY; y++) {
					grid[x][y].resize(gridSizeZ);
					for(int z = 0; z < gridSizeZ; z++) {
						GridPoint& point = grid[x][y][z];
						point.position = ofVec3f(
							(x - gridSizeX/2) * spacing,
							(y - gridSizeY/2) * spacing,
							(z - gridSizeZ/2) * spacing
						);
						point.velocity = ofVec3f(0, 0, 0);
						point.phase = ofRandom(TWO_PI);
						point.amplitude = ofRandom(10, 40);
						point.brightness = 1.0f;
						point.normal = ofVec3f(0, 0, 1);
					}
				}
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	float normalizedX = (float)x / ofGetWidth();
	float normalizedY = (float)y / ofGetHeight();
	
	waveSpeed = 0.5f + normalizedX * 3.0f;
	noiseScale = 0.005f + normalizedY * 0.02f;
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

//--------------------------------------------------------------
void ofApp::updateGrid() {
	for(int x = 0; x < gridSizeX; x++) {
		for(int y = 0; y < gridSizeY; y++) {
			for(int z = 0; z < gridSizeZ; z++) {
				GridPoint& point = grid[x][y][z];
				
				ofVec3f displacement = getSecretarialPattern(x, y, z, time);
				point.velocity = displacement * waveSpeed;
				
				point.position = ofVec3f(
					(x - gridSizeX/2) * spacing + displacement.x,
					(y - gridSizeY/2) * spacing + displacement.y,
					(z - gridSizeZ/2) * spacing + displacement.z
				);
				
				point.normal = calculateNormal(x, y, z);
				
				float distanceFromCenter = point.position.length();
				float depthFactor = ofMap(distanceFromCenter, 0, 600, 1.0f, 0.4f);
				point.brightness = ofMap(sin(distanceFromCenter * 0.01f + time * 2.0f), -1, 1, 0.4f, 1.0f) * depthFactor;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawGrid() {
	for(int x = 0; x < gridSizeX; x++) {
		for(int y = 0; y < gridSizeY; y++) {
			for(int z = 0; z < gridSizeZ; z++) {
				GridPoint& point = grid[x][y][z];
				
				float intensity = (sin(point.phase + time * 3.0f) + 1.0f) * 0.5f;
				float finalBrightness = (intensity * point.brightness * 0.7f) + 0.3f;
				
				ofSetColor(255 * finalBrightness, 255 * finalBrightness, 255 * finalBrightness, 255);
				
				float sphereSize = 2.5f + sin(point.phase + time * 2.0f) * 1.5f;
				ofDrawSphere(point.position, sphereSize);
				
				if(x < gridSizeX - 1) {
					float connectionStrength = (point.brightness + grid[x+1][y][z].brightness) * 0.4f + 0.2f;
					ofSetColor(255 * connectionStrength, 255 * connectionStrength, 255 * connectionStrength, 180);
					ofDrawLine(point.position, grid[x+1][y][z].position);
				}
				if(y < gridSizeY - 1) {
					float connectionStrength = (point.brightness + grid[x][y+1][z].brightness) * 0.4f + 0.2f;
					ofSetColor(255 * connectionStrength, 255 * connectionStrength, 255 * connectionStrength, 180);
					ofDrawLine(point.position, grid[x][y+1][z].position);
				}
				if(z < gridSizeZ - 1) {
					float connectionStrength = (point.brightness + grid[x][y][z+1].brightness) * 0.4f + 0.2f;
					ofSetColor(255 * connectionStrength, 255 * connectionStrength, 255 * connectionStrength, 180);
					ofDrawLine(point.position, grid[x][y][z+1].position);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawSecretarialPatterns() {
	ofSetColor(255, 200);
	ofNoFill();
	
	float t = time * 0.5f;
	
	for(int layer = 0; layer < 5; layer++) {
		ofPushMatrix();
		ofRotateYDeg(t * 10 + layer * 72);
		ofRotateXDeg(sin(t + layer) * 20);
		
		float radius = 200 + layer * 50;
		int segments = 8 + layer * 4;
		
		for(int i = 0; i < segments; i++) {
			float angle1 = (i / float(segments)) * TWO_PI;
			float angle2 = ((i + 1) / float(segments)) * TWO_PI;
			
			float wave1 = sin(angle1 * 3 + t) * 30;
			float wave2 = sin(angle2 * 3 + t) * 30;
			
			ofVec3f p1(
				cos(angle1) * (radius + wave1),
				sin(angle1) * (radius + wave1) * 0.5f,
				sin(angle1 * 2 + t) * 50
			);
			
			ofVec3f p2(
				cos(angle2) * (radius + wave2),
				sin(angle2) * (radius + wave2) * 0.5f,
				sin(angle2 * 2 + t) * 50
			);
			
			float alpha = 180 - layer * 20;
			ofSetColor(255, 255, 255, alpha);
			ofDrawLine(p1, p2);
			
			if(i % 2 == 0) {
				ofSetColor(255, 255, 255, alpha * 0.7f);
				ofDrawLine(p1, ofVec3f(0, 0, 0));
			}
		}
		ofPopMatrix();
	}
	
	ofFill();
}

//--------------------------------------------------------------
ofVec3f ofApp::getSecretarialPattern(int x, int y, int z, float t) {
	float nx = x * noiseScale;
	float ny = y * noiseScale;
	float nz = z * noiseScale;
	
	float wave1 = sin(nx * 5 + t * waveSpeed) * cos(ny * 3 + t * waveSpeed * 0.7f);
	float wave2 = cos(nz * 4 + t * waveSpeed * 0.8f) * sin(nx * 2 + t * waveSpeed);
	float wave3 = sin(ny * 6 + nz * 2 + t * waveSpeed * 1.2f);
	
	float noise1 = ofNoise(nx + t * 0.1f, ny, nz) * 2.0f - 1.0f;
	float noise2 = ofNoise(nx, ny + t * 0.1f, nz) * 2.0f - 1.0f;
	float noise3 = ofNoise(nx, ny, nz + t * 0.1f) * 2.0f - 1.0f;
	
	float spiral = atan2(y - gridSizeY/2, x - gridSizeX/2) + t * 0.5f;
	float spiralWave = sin(spiral * 8) * cos(z * 0.2f + t);
	
	return ofVec3f(
		(wave1 + noise1 * 0.5f + spiralWave * 0.3f) * spacing * 0.2f,
		(wave2 + noise2 * 0.5f + sin(spiral * 3) * 0.3f) * spacing * 0.2f,
		(wave3 + noise3 * 0.5f + cos(spiral * 2 + t) * 0.4f) * spacing * 0.2f
	);
}

//--------------------------------------------------------------
ofVec3f ofApp::calculateNormal(int x, int y, int z) {
	ofVec3f normal(0, 0, 1);
	
	if(x > 0 && x < gridSizeX-1 && y > 0 && y < gridSizeY-1 && z > 0 && z < gridSizeZ-1) {
		ofVec3f dx = grid[x+1][y][z].position - grid[x-1][y][z].position;
		ofVec3f dy = grid[x][y+1][z].position - grid[x][y-1][z].position;
		normal = dx.getCrossed(dy).normalize();
	}
	
	return normal;
}

//--------------------------------------------------------------
void ofApp::updateParticles() {
	for(auto it = particles.begin(); it != particles.end();) {
		it->life += ofGetLastFrameTime();
		it->alpha = ofMap(it->life, 0, it->maxLife, 1.0f, 0.0f);
		
		it->velocity *= 0.98f;
		it->position += it->velocity * ofGetLastFrameTime();
		
		if(it->life > it->maxLife) {
			it = particles.erase(it);
		} else {
			++it;
		}
	}
}

//--------------------------------------------------------------
void ofApp::spawnParticles() {
	if(ofGetFrameNum() % 8 == 0 && particles.size() < 100) {
		Particle p;
		
		int x = ofRandom(gridSizeX);
		int y = ofRandom(gridSizeY);
		int z = ofRandom(gridSizeZ);
		
		p.position = grid[x][y][z].position + ofVec3f(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10));
		p.velocity = grid[x][y][z].velocity * ofRandom(0.5f, 2.0f) + ofVec3f(ofRandom(-20, 20), ofRandom(-20, 20), ofRandom(-20, 20));
		p.life = 0;
		p.maxLife = ofRandom(3.0f, 6.0f);
		p.size = ofRandom(1.5f, 3.5f);
		p.alpha = 1.0f;
		
		particles.push_back(p);
	}
}

//--------------------------------------------------------------
void ofApp::drawParticles() {
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	for(const auto& particle : particles) {
		ofSetColor(255 * particle.alpha, 255 * particle.alpha, 255 * particle.alpha, 255 * particle.alpha);
		ofDrawSphere(particle.position, particle.size);
	}
	
	ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::drawFractalPatterns() {
	ofSetColor(255, 180);
	ofNoFill();
	
	float t = time * 0.8f;
	int maxDepth = 3;
	
	function<void(ofVec3f, float, int)> drawFractalBranch = [&](ofVec3f center, float size, int depth) {
		if(depth <= 0 || size < 5.0f) return;
		
		float angleStep = TWO_PI / 4;
		for(int i = 0; i < 4; i++) {
			float angle = angleStep * i + t * (depth * 0.5f);
			float radius = size * (0.6f + sin(t + depth) * 0.2f);
			
			ofVec3f point = center + ofVec3f(
				cos(angle) * radius,
				sin(angle) * radius * cos(t * 0.3f + depth),
				sin(t * 0.4f + angle + depth) * radius * 0.5f
			);
			
			float alpha = 120.0f * (depth / float(maxDepth)) + 60.0f;
			ofSetColor(255, 255, 255, alpha);
			ofDrawLine(center, point);
			
			if(depth > 1) {
				drawFractalBranch(point, size * 0.7f, depth - 1);
			}
		}
	};
	
	for(int i = 0; i < 3; i++) {
		ofPushMatrix();
		ofRotateYDeg(i * 120 + t * 20);
		drawFractalBranch(ofVec3f(0, 0, 0), 150.0f + sin(t + i) * 50.0f, maxDepth);
		ofPopMatrix();
	}
	
	ofFill();
}

//--------------------------------------------------------------
void ofApp::drawRibbons() {
	ofSetColor(255, 160);
	ofNoFill();
	
	float t = time * 0.6f;
	int numRibbons = 4;
	
	for(int ribbon = 0; ribbon < numRibbons; ribbon++) {
		ofBeginShape();
		
		float ribbonPhase = (ribbon / float(numRibbons)) * TWO_PI;
		float ribbonRadius = 200 + ribbon * 30;
		
		for(int i = 0; i <= 50; i++) {
			float progress = i / 50.0f;
			float angle = progress * TWO_PI * 3 + ribbonPhase + t;
			
			float wave1 = sin(angle * 2 + t) * 30;
			float wave2 = cos(angle * 3 + t * 0.7f) * 20;
			float wave3 = sin(progress * PI * 4 + t) * 40;
			
			ofVec3f point(
				cos(angle) * (ribbonRadius + wave1) + wave3,
				sin(angle) * (ribbonRadius + wave1) * 0.6f + wave2,
				sin(angle * 1.5f + t + ribbonPhase) * 100 + cos(progress * PI * 2 + t) * 50
			);
			
			float alpha = 120 + sin(progress * PI * 2 + t) * 60;
			ofSetColor(255, 255, 255, alpha);
			ofVertex(point);
		}
		
		ofEndShape(false);
	}
	
	ofFill();
}
