#include "ofApp.h"

#include <algorithm>
#include <sstream>

namespace {
constexpr int kRibbonSteps = 180;
constexpr int kPortalSegments = 220;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetCircleResolution(72);

	buildPalette();
	noiseSeed = ofRandom(1000.0f);

	ofEnableDepthTest();
	cam.setNearClip(0.1f);
	cam.setFarClip(5000.0f);
	cam.disableMouseInput();
	cam.setDistance(850.0f);

	const int ribbonCount = 14;
	ribbons.reserve(ribbonCount);
	for (int i = 0; i < ribbonCount; ++i) {
		Ribbon ribbon;
		ribbon.angle = ofRandom(TWO_PI);
		ribbon.radius = ofRandom(160.0f, 420.0f);
		ribbon.height = ofRandom(520.0f, 820.0f);
		ribbon.twist = ofRandom(2.5f, 4.5f);
		ribbon.width = ofRandom(10.0f, 28.0f);
		ribbon.noiseOffset = ofRandom(1000.0f);
		ribbon.colorA = palette[i % palette.size()];
		ribbon.colorB = palette[(i * 3 + 5) % palette.size()];
		ribbons.push_back(ribbon);
	}

	const int sparkCount = 280;
	sparks.reserve(sparkCount);
	for (int i = 0; i < sparkCount; ++i) {
		sparks.emplace_back(
			ofRandom(-480.0f, 480.0f),
			ofRandom(-720.0f, 720.0f),
			ofRandom(-480.0f, 480.0f)
		);
	}

	const int portalCount = 7;
	portalLayers.reserve(portalCount);
	for(int i = 0; i < portalCount; ++i){
		PortalLayer layer;
		const float pct = portalCount > 1 ? static_cast<float>(i) / (portalCount - 1) : 0.0f;
		layer.radius = ofLerp(140.0f, 420.0f, pct);
		layer.thickness = ofRandom(8.0f, 24.0f);
		layer.speed = ofRandom(0.35f, 0.85f);
		layer.noiseOffset = ofRandom(1000.0f);
		const ofFloatColor colorA = palette[(i * 2 + 3) % palette.size()];
		const ofFloatColor colorB = palette[(i * 3 + 5) % palette.size()];
		layer.color = colorA.getLerped(colorB, 0.35f);
		portalLayers.push_back(layer);
	}

	const int fogCount = 28;
	fogLayers.reserve(fogCount);
	for(int i = 0; i < fogCount; ++i){
		FogLayer fog;
		fog.basePos = glm::vec3(
			ofRandom(-420.0f, 420.0f),
			ofRandom(-300.0f, 320.0f),
			ofRandom(-420.0f, 420.0f)
		);
		fog.radius = ofRandom(200.0f, 420.0f);
		fog.speed = ofRandom(0.18f, 0.42f);
		fog.noiseOffset = ofRandom(1000.0f);
		fog.color = palette[(i * 7 + 2) % palette.size()];
		fogLayers.push_back(fog);
	}

	ensureFbos();
	trailFbo.begin();
	ofClear(0, 0, 0, 0);
	trailFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	const float time = ofGetElapsedTimef();

	ensureFbos();
	renderScene(time);

	trailFbo.begin();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(0, 0, 0, 255 * 0.14f);
	ofDrawRectangle(0, 0, trailFbo.getWidth(), trailFbo.getHeight());
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255, 255, 255, 130);
	sceneFbo.draw(0, 0);
	trailFbo.end();

	ofSetColor(255);
	sceneFbo.draw(0, 0);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255, 255, 255, 150);
	trailFbo.draw(0, 0);
	ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::renderScene(float time){
	sceneFbo.begin();
	ofClear(0, 0, 0, 0);

	drawBackgroundGradient();

	ofEnableDepthTest();

	const float azimuth = fmodf(time * 14.0f, 360.0f);
	const float polar = ofMap(sin(time * 0.32f), -1.0f, 1.0f, 32.0f, 82.0f);
	const float radius = ofMap(sin(time * 0.24f), -1.0f, 1.0f, 680.0f, 960.0f);

	const glm::vec3 target(
		sin(time * 0.17f) * 70.0f,
		sin(time * 0.31f) * 110.0f,
		cos(time * 0.21f) * 40.0f
	);
	cam.orbitDeg(azimuth, polar, radius, target);

	cam.begin();
	ofPushMatrix();
	ofRotateDeg(sin(time * 0.2f) * 8.0f, 0.0f, 1.0f, 0.0f);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	drawRibbons(time);

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	drawPortal(time);
	drawSparks(time);
	drawFogLayers(time);

	ofDisableAlphaBlending();

	ofPopMatrix();
	cam.end();

	ofDisableDepthTest();
	sceneFbo.end();
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
	ensureFbos();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
ofFloatColor ofApp::hexToColor(const std::string& hex){
	if(hex.empty()){
		return ofFloatColor::white;
	}

	std::string cleanHex = hex;
	if(cleanHex[0] == '#'){
		cleanHex = cleanHex.substr(1);
	}

	unsigned int value = 0;
	std::stringstream ss;
	ss << std::hex << cleanHex;
	ss >> value;

	const auto r = static_cast<unsigned int>((value >> 16) & 0xff);
	const auto g = static_cast<unsigned int>((value >> 8) & 0xff);
	const auto b = static_cast<unsigned int>(value & 0xff);

	return ofFloatColor(r / 255.f, g / 255.f, b / 255.f, 1.0f);
}

//--------------------------------------------------------------
void ofApp::buildPalette(){
	const std::vector<std::string> hexValues = {
		"#362d78",
		"#523fa3",
		"#916ccc",
		"#bda1e5",
		"#c8c0e9",
		"#84bae7",
		"#516ad4",
		"#333f87",
		"#293039",
		"#283631"
	};

	palette.clear();
	palette.reserve(hexValues.size());
	for(const auto& hex : hexValues){
		palette.emplace_back(hexToColor(hex));
	}
}

//--------------------------------------------------------------
void ofApp::ensureFbos(){
	const int width = ofGetWidth();
	const int height = ofGetHeight();
	if(width <= 0 || height <= 0){
		return;
	}

	const bool needsAllocation =
		!sceneFbo.isAllocated() ||
		static_cast<int>(sceneFbo.getWidth()) != width ||
		static_cast<int>(sceneFbo.getHeight()) != height;

	if(!needsAllocation){
		return;
	}

	ofFbo::Settings sceneSettings;
	sceneSettings.width = width;
	sceneSettings.height = height;
	sceneSettings.internalformat = GL_RGBA16F;
	sceneSettings.useDepth = true;
	sceneSettings.depthStencilAsTexture = true;
	sceneSettings.numSamples = 2;
	sceneSettings.minFilter = GL_LINEAR;
	sceneSettings.maxFilter = GL_LINEAR;

	sceneFbo.allocate(sceneSettings);
	sceneFbo.begin();
	ofClear(0, 0, 0, 0);
	sceneFbo.end();

	ofFbo::Settings trailSettings = sceneSettings;
	trailSettings.useDepth = false;
	trailSettings.depthStencilAsTexture = false;
	trailSettings.numSamples = 0;

	trailFbo.allocate(trailSettings);
	trailFbo.begin();
	ofClear(0, 0, 0, 0);
	trailFbo.end();
}

//--------------------------------------------------------------
void ofApp::drawBackgroundGradient(){
	ofPushStyle();
	ofDisableDepthTest();

	ofMesh gradient;
	gradient.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	ofFloatColor top = palette[8];
	ofFloatColor bottom = palette[9];
	top *= 0.35f;
	bottom *= 0.45f;
	top.a = 1.0f;
	bottom.a = 1.0f;

	gradient.addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
	gradient.addColor(top);
	gradient.addVertex(glm::vec3(ofGetWidth(), 0.0f, 0.0f));
	gradient.addColor(top);
	gradient.addVertex(glm::vec3(0.0f, ofGetHeight(), 0.0f));
	gradient.addColor(bottom);
	gradient.addVertex(glm::vec3(ofGetWidth(), ofGetHeight(), 0.0f));
	gradient.addColor(bottom);

	gradient.draw();

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawRibbons(float time){
	for(const auto& ribbon : ribbons){
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

		for(int step = 0; step <= kRibbonSteps; ++step){
			const float pct = static_cast<float>(step) / kRibbonSteps;
			const float height = ofLerp(-ribbon.height * 0.5f, ribbon.height * 0.5f, pct);
			const float radius = ribbon.radius + ofSignedNoise(ribbon.noiseOffset, pct * 2.4f, time * 0.35f) * 110.0f;
			const float angle = ribbon.angle + pct * ribbon.twist * TWO_PI + time * 0.25f;

			glm::vec3 center(
				cos(angle) * radius,
				height + sin(time * 0.6f + pct * TWO_PI) * 28.0f,
				sin(angle) * radius
			);

			glm::vec3 radial(center.x, 0.0f, center.z);
			if(glm::length(radial) > 0.001f){
				radial = glm::normalize(radial);
			}else{
				radial = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			glm::vec3 tangent = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), radial);
			if(glm::length(tangent) > 0.001f){
				tangent = glm::normalize(tangent);
			}else{
				tangent = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			float width = ribbon.width * (0.35f + 0.65f * pct);
			width += ofSignedNoise(ribbon.noiseOffset + pct, time * 0.5f) * 6.0f;
			width = std::max(4.0f, width);

			const glm::vec3 offset = tangent * width;

			ofFloatColor colorA = ribbon.colorA;
			ofFloatColor colorB = ribbon.colorB;

			colorA.a = ofMap(pct, 0.0f, 1.0f, 0.12f, 0.55f, true);
			colorB.a = ofMap(pct, 0.0f, 1.0f, 0.45f, 0.8f, true);

			const ofFloatColor accent = palette[(static_cast<size_t>(pct * palette.size()) + 2) % palette.size()];
			colorB = colorB.getLerped(accent, 0.15f);

			mesh.addVertex(center + offset);
			mesh.addColor(colorA);
			mesh.addVertex(center - offset);
			mesh.addColor(colorB);
		}

		mesh.draw();
	}
}

//--------------------------------------------------------------
void ofApp::drawPortal(float time){
	if(portalLayers.empty()){
		return;
	}

	for(const auto& layer : portalLayers){
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

		for(int segment = 0; segment <= kPortalSegments; ++segment){
			const float pct = static_cast<float>(segment) / kPortalSegments;
			const float angle = pct * TWO_PI;
			const float wobble = ofSignedNoise(layer.noiseOffset, pct * 3.0f, time * 0.65f) * 12.0f;
			const float radius = layer.radius + wobble + sin(time * layer.speed + pct * TWO_PI * 3.0f) * 8.0f;

			glm::vec3 center(
				cos(angle) * radius,
				sin(angle * 3.0f + time * 0.6f) * 42.0f,
				sin(angle) * radius
			);

			glm::vec3 radial = glm::vec3(cos(angle), 0.0f, sin(angle));
			if(glm::length(radial) > 0.001f){
				radial = glm::normalize(radial);
			}else{
				radial = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			glm::vec3 tangent = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), radial);
			if(glm::length(tangent) > 0.001f){
				tangent = glm::normalize(tangent);
			}else{
				tangent = glm::vec3(0.0f, 0.0f, 1.0f);
			}

			const float width = layer.thickness * (0.9f + 0.4f * sin(time * layer.speed + pct * TWO_PI * 2.0f));
			const glm::vec3 offset = tangent * width;

			ofFloatColor inner = layer.color;
			ofFloatColor outer = palette[(static_cast<size_t>(pct * palette.size()) + 1) % palette.size()];
			inner.a = 0.08f;
			outer.a = 0.55f;
			outer = outer.getLerped(layer.color, 0.5f);

			mesh.addVertex(center + offset);
			mesh.addColor(outer);
			mesh.addVertex(center - offset);
			mesh.addColor(inner);
		}

		mesh.draw();
	}
}

//--------------------------------------------------------------
void ofApp::drawSparks(float time){
	for(size_t i = 0; i < sparks.size(); ++i){
		const glm::vec3& base = sparks[i];

		glm::vec3 pos = base;
		pos.x += sin(time * 0.45f + base.y * 0.01f) * 45.0f;
		pos.y += ofSignedNoise(base.x * 0.01f, base.y * 0.005f, time * 0.35f) * 60.0f;
		pos.z += ofSignedNoise(base.z * 0.01f, time * 0.2f) * 80.0f;

		const float flicker = ofNoise(base.x * 0.002f, base.y * 0.002f, time * 0.8f);
		const float size = ofMap(flicker, 0.0f, 1.0f, 1.2f, 4.6f, true);

		ofFloatColor glow = palette[(i * 5) % palette.size()];
		glow.a = 0.12f + 0.35f * flicker;

		ofSetColor(glow);
		ofDrawSphere(pos, size);

		ofFloatColor halo = glow;
		halo.a *= 0.25f;
		ofSetColor(halo);
		ofDrawSphere(pos, size * 2.0f);
	}
}

//--------------------------------------------------------------
void ofApp::drawFogLayers(float time){
	if(fogLayers.empty()){
		return;
	}

	glm::vec3 right = cam.getSideDir();
	glm::vec3 up = cam.getUpDir();
	if(glm::length(right) < 0.001f){
		right = glm::vec3(1.0f, 0.0f, 0.0f);
	}else{
		right = glm::normalize(right);
	}
	if(glm::length(up) < 0.001f){
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}else{
		up = glm::normalize(up);
	}

	for(const auto& fog : fogLayers){
		glm::vec3 position = fog.basePos;
		position.x += ofSignedNoise(fog.noiseOffset, time * 0.25f) * 70.0f;
		position.y += sin(time * fog.speed + fog.noiseOffset) * 160.0f;
		position.z += ofSignedNoise(fog.noiseOffset + 15.0f, time * 0.35f) * 90.0f;

		const float radius = fog.radius * (0.7f + 0.3f * ofNoise(fog.noiseOffset, time * 0.4f));
		const glm::vec3 horiz = right * radius;
		const glm::vec3 vert = up * radius * 0.6f;

		ofFloatColor tint = fog.color;
		tint.a = 0.03f + 0.12f * ofNoise(fog.noiseOffset + 200.0f, time * 0.45f);

		ofMesh quad;
		quad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		quad.addVertex(position - horiz - vert);
		quad.addColor(tint);
		quad.addVertex(position + horiz - vert);
		quad.addColor(tint);
		quad.addVertex(position - horiz + vert);
		quad.addColor(tint);
		quad.addVertex(position + horiz + vert);
		quad.addColor(tint);

		quad.draw();
	}
}
