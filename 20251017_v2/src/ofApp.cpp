#include "ofApp.h"
#include <algorithm>
#include <cmath>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetBackgroundColor(ofColor::black);
	ofEnableSmoothing();
	ofSetCircleResolution(120);
	regeneratePatterns();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);

	if(patterns.empty()){
		return;
	}

	const float t = ofGetElapsedTimef();
	const float width = static_cast<float>(ofGetWidth());
	const float height = static_cast<float>(ofGetHeight());
	const float cellW = width / static_cast<float>(cols);
	const float cellH = height / static_cast<float>(rows);
	const float baseSize = std::min(cellW, cellH);

	ofPushStyle();
	ofSetColor(ofColor::white);
	ofNoFill();

	for(std::size_t i = 0; i < patterns.size(); ++i){
		const auto& pattern = patterns[i];
		const int cx = static_cast<int>(i % cols);
		const int cy = static_cast<int>(i / cols);
		const float x = (static_cast<float>(cx) + 0.5f) * cellW;
		const float y = (static_cast<float>(cy) + 0.5f) * cellH;
		const float size = baseSize * pattern.scale;
		const float wobble = std::sin(t * 0.35f + pattern.noiseOffset) * 12.0f * pattern.detail;

		ofPushMatrix();
		ofTranslate(x, y);
			ofRotateDeg(pattern.rotation + wobble + t * pattern.rotationSpeed);
		ofSetLineWidth(1.0f + pattern.detail * 1.5f);

		switch(pattern.type){
			case 0:
				drawMandalaPattern(pattern, size, t);
				break;
			case 1:
				drawTrianglePattern(pattern, size, t);
				break;
			case 2:
				drawFlowerPattern(pattern, size, t);
				break;
			default:
				drawLatticePattern(pattern, size, t);
				break;
		}

		ofPopMatrix();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'r' || key == 'R'){
		regeneratePatterns();
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
void ofApp::regeneratePatterns(){
	patterns.clear();
	patterns.resize(static_cast<std::size_t>(cols * rows));

	for(auto& pattern : patterns){
		pattern.type = static_cast<int>(ofRandom(4));
		pattern.rotation = std::round(ofRandom(0.0f, 7.0f)) * 45.0f;
		pattern.rotationSpeed = ofRandom(-35.0f, 35.0f);
		if(std::abs(pattern.rotationSpeed) < 5.0f){
			pattern.rotationSpeed = (pattern.rotationSpeed < 0.0f) ? -12.0f : 12.0f;
		}
		pattern.noiseOffset = ofRandom(1000.0f);
		pattern.scale = ofRandom(0.65f, 0.95f);
		pattern.detail = ofRandom(0.2f, 1.0f);
	}
}

//--------------------------------------------------------------
void ofApp::drawMandalaPattern(const CellPattern& pattern, float size, float t) const{
	const float radius = size * 0.45f;
	const int rings = 3 + static_cast<int>(std::round(pattern.detail * 4.0f));

	for(int r = 1; r <= rings; ++r){
		const float pct = static_cast<float>(r) / static_cast<float>(rings + 1);
		ofDrawCircle(0.0f, 0.0f, radius * pct);
	}

	int spokes = 6 + static_cast<int>(std::round(pattern.detail * 6.0f));
	const float wobble = std::sin(t * 0.4f + pattern.noiseOffset) * 0.25f;
	for(int i = 0; i < spokes; ++i){
		float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(spokes) + wobble;
		float x = std::cos(angle) * radius;
		float y = std::sin(angle) * radius;
		ofDrawLine(-x, -y, x, y);
	}

	const int orbitCount = 6;
	const float orbitRadius = radius * 0.65f;
	const float smallRadius = radius * 0.12f;
	for(int i = 0; i < orbitCount; ++i){
		float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(orbitCount)
			+ t * 0.25f * (pattern.detail + 0.6f);
		float x = std::cos(angle) * orbitRadius;
		float y = std::sin(angle) * orbitRadius;
		ofDrawCircle(x, y, smallRadius);
	}
}

//--------------------------------------------------------------
void ofApp::drawTrianglePattern(const CellPattern& pattern, float size, float t) const{
	const float radius = size * 0.48f;
	const int layers = 2 + static_cast<int>(std::round(pattern.detail * 4.0f));

	for(int i = 0; i < layers; ++i){
		float pct = static_cast<float>(i + 1) / static_cast<float>(layers + 1);
		float r = radius * pct;
		float spin = std::sin(t * 0.3f + pattern.noiseOffset + i * 0.6f) * 18.0f;

		ofPushMatrix();
		ofRotateDeg(spin);
		drawEquilateralTriangle(r);
		ofRotateDeg(60.0f);
		drawEquilateralTriangle(r);
		ofPopMatrix();
	}

	float innerPulse = ofMap(std::sin(t * 0.6f + pattern.noiseOffset), -1.0f, 1.0f, 0.3f, 0.8f);
	ofDrawCircle(0.0f, 0.0f, radius * 0.2f * innerPulse);
}

//--------------------------------------------------------------
void ofApp::drawFlowerPattern(const CellPattern& pattern, float size, float t) const{
	const float baseRadius = size * 0.23f;
	const float pulse = ofMap(std::sin(t * 0.45f + pattern.noiseOffset), -1.0f, 1.0f, 0.85f, 1.15f);
	const float radius = baseRadius * pulse;
	const float outerLimit = size * 0.45f;
	const float maxOrbit = std::max(outerLimit - radius, radius);

	ofDrawCircle(0.0f, 0.0f, radius);

	const int petals = 6;
	const int rings = 1 + static_cast<int>(std::round(pattern.detail * 2.0f));
	for(int ring = 1; ring <= rings; ++ring){
		float pct = (rings > 1) ? static_cast<float>(ring - 1) / static_cast<float>(rings - 1) : 0.0f;
		float ringRadius = ofLerp(radius, maxOrbit, pct);
		for(int i = 0; i < petals; ++i){
			float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(petals);
			float x = std::cos(angle) * ringRadius;
			float y = std::sin(angle) * ringRadius;
			ofDrawCircle(x, y, radius);
		}
	}

	const int chords = petals;
	const float chordRadius = ofLerp(radius * 0.8f, outerLimit * 0.95f, pattern.detail);
	for(int i = 0; i < chords; ++i){
		float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(chords) + t * 0.15f;
		float x1 = std::cos(angle) * chordRadius;
		float y1 = std::sin(angle) * chordRadius;
		float x2 = std::cos(angle + PI) * chordRadius;
		float y2 = std::sin(angle + PI) * chordRadius;
		ofDrawLine(x1, y1, x2, y2);
	}
}

//--------------------------------------------------------------
void ofApp::drawLatticePattern(const CellPattern& pattern, float size, float t) const{
	const float half = size * 0.47f;
	int divisions = 3 + static_cast<int>(std::round(pattern.detail * 5.0f));
	divisions = std::max(divisions, 2);

	for(int i = 0; i < divisions; ++i){
		float pct = (divisions > 1) ? static_cast<float>(i) / static_cast<float>(divisions - 1) : 0.0f;
		float offset = ofLerp(-half, half, pct);
		ofDrawLine(offset, -half, offset, half);
		ofDrawLine(-half, offset, half, offset);
	}

	const float wave = std::sin(t * 0.6f + pattern.noiseOffset) * 0.2f;
	const float inner = half * (0.6f + wave);
	ofDrawLine(-inner, -inner, inner, inner);
	ofDrawLine(-inner, inner, inner, -inner);
	ofDrawRectangle(-half, -half, half * 2.0f, half * 2.0f);
}

//--------------------------------------------------------------
void ofApp::drawEquilateralTriangle(float radius) const{
	ofPolyline polyline;
	for(int i = 0; i < 3; ++i){
		float angle = ofDegToRad(-90.0f + static_cast<float>(i) * 120.0f);
		polyline.addVertex(std::cos(angle) * radius, std::sin(angle) * radius);
	}
	polyline.close();
	polyline.draw();
}
