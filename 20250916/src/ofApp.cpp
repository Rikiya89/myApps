#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetBackgroundAuto(false);
	ofBackground(0);
	ofSetCircleResolution(90);

	trailFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	clearTrails();
	buildVignetteMesh(ofGetWidth(), ofGetHeight());
	buildGlowMesh(ofGetWidth(), ofGetHeight());
	buildSacredGeometry(ofGetWidth(), ofGetHeight());
	buildOrbitalTrails(ofGetWidth(), ofGetHeight());
	buildHarmonicRings(ofGetWidth(), ofGetHeight());
	buildRadiantCore(ofGetWidth(), ofGetHeight());
	buildLightRays(ofGetWidth(), ofGetHeight());
	resetParticles();
	lastUpdateTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update(){
	const float width = ofGetWidth();
	const float height = ofGetHeight();
	const float time = ofGetElapsedTimef() * timeScale;
	const glm::vec2 canvasCenter(width * 0.5f, height * 0.5f);
	const float maxDimension = width > height ? width : height;
	const float shorterDimension = width < height ? width : height;

	const float now = ofGetElapsedTimef();
	float deltaTime = now - lastUpdateTime;
	if (lastUpdateTime <= 0.f || deltaTime > 1.f) {
		deltaTime = 1.f / 60.f;
	}
	lastUpdateTime = now;
	updateSacredGeometry(deltaTime);
	updateOrbitalTrails(deltaTime);
	updateHarmonicRings(deltaTime);
	updateLightRays(deltaTime);

	for (auto &particle : particles) {
		particle.previousPosition = particle.position;

		const float sampleX = particle.position.x * flowScale;
		const float sampleY = particle.position.y * flowScale;
		const float angle = (ofNoise(sampleX, sampleY, time + particle.noiseOffset) - 0.5f) * TWO_PI * 3.f;
		const float velocitySample = ofNoise(sampleY + 5.f, sampleX - 7.f, time * 0.8f + particle.noiseOffset);
		const float magnitude = baseStep * (0.45f + velocitySample * 1.4f + particle.speedBias * 0.65f);
		glm::vec2 flowDirection = {cosf(angle), sinf(angle)};

		glm::vec2 toCenter = particle.position - canvasCenter;
		const float distance = glm::length(toCenter) + 0.0001f;
		glm::vec2 swirlDirection(0.f);
		if (distance > 0.f) {
			swirlDirection = glm::normalize(glm::vec2(-toCenter.y, toCenter.x));
		}
		const float swirlAmount = swirlStrength * (0.25f + particle.symmetryBias * 0.85f);

		glm::vec2 radialDirection(0.f);
		if (distance > 0.f) {
			radialDirection = -glm::normalize(toCenter);
		}
		const float radialBlend = ofMap(distance, 0.f, maxDimension, 0.35f, 0.04f, true);

		glm::vec2 combinedDirection = flowDirection * (0.6f + particle.speedBias * 0.22f);
		combinedDirection += swirlDirection * swirlAmount;
		combinedDirection += radialDirection * radialBlend * (0.6f + particle.symmetryBias * 0.4f);

		if (!harmonicRings.empty() && distance > 0.f) {
			float ringAdjustment = 0.f;
			const float damping = shorterDimension * 0.18f;
			const glm::vec2 outward = glm::normalize(toCenter);
			for (const auto &ring : harmonicRings) {
				const float diff = distance - ring.radius;
				const float influence = expf(-fabsf(diff) / (damping * 0.85f)) * (0.6f + ring.weight * 0.4f);
				ringAdjustment += -diff * influence;
			}
			combinedDirection += outward * (ringAdjustment / damping) * ringAttractorStrength;
		}
		if (glm::dot(combinedDirection, combinedDirection) > 0.00001f) {
			combinedDirection = glm::normalize(combinedDirection);
		} else {
			combinedDirection = flowDirection;
		}

		const glm::vec2 velocityTarget = combinedDirection * magnitude;
		particle.velocity = particle.velocity * (1.f - velocityLerp) + velocityTarget * velocityLerp;
		particle.position += particle.velocity;

		bool wrapped = false;
		if (particle.position.x < -wrapMargin) {
			particle.position.x = width + wrapMargin;
			wrapped = true;
		}
		if (particle.position.x > width + wrapMargin) {
			particle.position.x = -wrapMargin;
			wrapped = true;
		}
		if (particle.position.y < -wrapMargin) {
			particle.position.y = height + wrapMargin;
			wrapped = true;
		}
		if (particle.position.y > height + wrapMargin) {
			particle.position.y = -wrapMargin;
			wrapped = true;
		}

		if (wrapped) {
			particle.previousPosition = particle.position;
			particle.velocity = glm::vec2(0.f);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	trailFbo.begin();
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	const float time = ofGetElapsedTimef();
	const float fade = ofClamp(fadeStrength + sinf(time * 0.28f + 1.2f) * 4.f, 9.f, 24.f);
	ofSetColor(0, 0, 0, fade);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(12, 12, 12, 6);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	for (const auto &particle : particles) {
		drawParticleTrail(particle, time);
	}
	ofPopStyle();
	trailFbo.end();

	const float compositeTime = ofGetElapsedTimef();
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	const float zoom = 1.f + compositeZoomStrength * (0.35f + 0.65f * sinf(compositeTime * 0.13f + 0.4f));
	const float driftX = compositeDriftStrength * sinf(compositeTime * 0.07f + 1.2f);
	const float driftY = compositeDriftStrength * cosf(compositeTime * 0.093f + 0.8f);
	const float drawWidth = ofGetWidth() * zoom;
	const float drawHeight = ofGetHeight() * zoom;
	const float drawX = (ofGetWidth() - drawWidth) * 0.5f + driftX;
	const float drawY = (ofGetHeight() - drawHeight) * 0.5f + driftY;
	ofSetColor(255);
	trailFbo.draw(drawX, drawY, drawWidth, drawHeight);
	ofPopStyle();
	drawBloomEcho(compositeTime);

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	const float drawTime = compositeTime;
	drawSoftGlow(drawTime);
	drawSacredGeometry(drawTime);
	drawHarmonicRings(drawTime);
	drawOrbitalTrails(drawTime);
	drawRadiantCore(drawTime);
	drawLightRays(drawTime);
	ofPopStyle();

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofFill();
	vignetteMesh.draw();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r' || key == 'R') {
		resetParticles();
		clearTrails();
		buildSacredGeometry(ofGetWidth(), ofGetHeight());
		buildOrbitalTrails(ofGetWidth(), ofGetHeight());
		buildHarmonicRings(ofGetWidth(), ofGetHeight());
		buildRadiantCore(ofGetWidth(), ofGetHeight());
		buildLightRays(ofGetWidth(), ofGetHeight());
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
	trailFbo.allocate(w, h, GL_RGBA);
	clearTrails();
	buildVignetteMesh(w, h);
	buildGlowMesh(w, h);
	buildSacredGeometry(w, h);
	buildOrbitalTrails(w, h);
	buildHarmonicRings(w, h);
	buildRadiantCore(w, h);
	buildLightRays(w, h);
	resetParticles();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::resetParticles(){
	particles.clear();
	particles.reserve(900);

	for (std::size_t i = 0; i < 900; ++i) {
		FlowParticle particle;
		particle.position = {ofRandomWidth(), ofRandomHeight()};
		particle.previousPosition = particle.position;
		particle.velocity = glm::vec2(0.f);
		particle.noiseOffset = ofRandom(1000.f);
		particle.speedBias = powf(ofRandomuf(), 1.4f);
		particle.width = ofRandom(0.8f, 2.6f);
		particle.symmetryBias = powf(ofRandomuf(), 1.8f);
		particles.push_back(particle);
	}

	lastUpdateTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::clearTrails(){
	trailFbo.begin();
	ofClear(0, 0, 0, 255);
	trailFbo.end();
}

//--------------------------------------------------------------
void ofApp::drawParticleTrail(const FlowParticle &particle, float time){
	ofPushStyle();
	const float lightSample = ofNoise(particle.position.x * flowScale * 0.65f,
					 particle.position.y * flowScale * 0.65f,
					 time * 0.18f + particle.noiseOffset * 0.75f);
	const float brightness = ofMap(lightSample, 0.f, 1.f, 35.f, 210.f, true);
	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);

	const auto mirrorX = [&](const glm::vec2 &v){
		return glm::vec2(center.x - (v.x - center.x), v.y);
	};
	const auto mirrorY = [&](const glm::vec2 &v){
		return glm::vec2(v.x, center.y - (v.y - center.y));
	};

	const auto drawStroke = [&](const glm::vec2 &a, const glm::vec2 &b, float widthScale, float alpha){
		ofSetColor(brightness, brightness, brightness, ofClamp(alpha, 0.f, 1.f) * 255.f);
		ofSetLineWidth(particle.width * widthScale);
		ofDrawLine(a, b);
	};

	const float bias = particle.symmetryBias;
	const float primaryAlpha = 0.82f + bias * 0.13f;
	drawStroke(particle.previousPosition, particle.position, 1.f, primaryAlpha);
	drawStroke(mirrorX(particle.previousPosition), mirrorX(particle.position), 0.9f, 0.48f + bias * 0.28f);
	drawStroke(mirrorY(particle.previousPosition), mirrorY(particle.position), 0.9f, 0.42f + bias * 0.18f);
	drawStroke(mirrorY(mirrorX(particle.previousPosition)), mirrorY(mirrorX(particle.position)), 0.8f, 0.3f + bias * 0.3f);

	if (particle.speedBias > 0.74f) {
		const float pulse = 0.42f + 0.58f * sinf(time * 0.7f + particle.noiseOffset * 3.6f);
		const float radius = 1.2f + pulse * (1.1f + bias * 1.9f);
		ofNoFill();
		ofSetLineWidth(0.5f + bias * 1.1f);
		ofSetColor(255, 255, 255, 28 + bias * 105 + pulse * 40);
		ofDrawCircle(particle.position, radius);
		ofFill();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::buildVignetteMesh(int width, int height){
	vignetteMesh.clear();
	vignetteMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	const glm::vec2 center(width * 0.5f, height * 0.5f);
	const float outerRadius = glm::length(glm::vec2(width, height)) * 0.68f;
	const int segments = 72;

	vignetteMesh.addVertex(glm::vec3(center.x, center.y, 0.f));
	vignetteMesh.addColor(ofFloatColor(0.f, 0.f, 0.f, 0.f));

	for (int i = 0; i <= segments; ++i) {
		const float angle = ofMap(i, 0, segments, 0.f, TWO_PI);
		const glm::vec2 dir(cosf(angle), sinf(angle));
		const glm::vec2 pos = center + dir * outerRadius;
		const float alpha = ofMap(glm::length(pos - center), 0.f, outerRadius, 0.35f, 0.72f, true);
		vignetteMesh.addVertex(glm::vec3(pos.x, pos.y, 0.f));
		vignetteMesh.addColor(ofFloatColor(0.f, 0.f, 0.f, alpha));
	}
}

//--------------------------------------------------------------
void ofApp::buildGlowMesh(int width, int height){
	glowMesh.clear();
	glowMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	const glm::vec2 center(width * 0.5f, height * 0.5f);
	const float shorter = width < height ? width : height;
	const float outerRadius = shorter * 0.55f;
	const int segments = 80;

	glowMesh.addVertex(glm::vec3(center.x, center.y, 0.f));
	glowMesh.addColor(ofFloatColor(1.f, 1.f, 1.f, 0.f));

	for (int i = 0; i <= segments; ++i) {
		const float angle = ofMap(i, 0, segments, 0.f, TWO_PI);
		const glm::vec2 dir(cosf(angle), sinf(angle));
		const glm::vec2 pos = center + dir * outerRadius;
		const float alpha = ofMap(glm::length(pos - center), 0.f, outerRadius, 0.02f, 0.f, true);
		glowMesh.addVertex(glm::vec3(pos.x, pos.y, 0.f));
		glowMesh.addColor(ofFloatColor(1.f, 1.f, 1.f, alpha));
	}
}

//--------------------------------------------------------------
void ofApp::drawSoftGlow(float time){
	if (glowMesh.getNumVertices() == 0) {
		return;
	}
	const float pulse = 0.32f + 0.28f * sinf(time * 0.22f);
	const float alpha = ofClamp(0.06f + pulse * 0.08f, 0.02f, 0.16f);
	glowMesh.setColor(0, ofFloatColor(1.f, 1.f, 1.f, alpha));
	glowMesh.draw();
}

//--------------------------------------------------------------
void ofApp::buildOrbitalTrails(int width, int height){
	orbitalTrails.clear();
	const float shorter = width < height ? width : height;
	const float base = shorter * 0.37f;

	const auto addTrail = [&](float radiusRatio, float speed, float thickness, float arcLength, float brightness){
		OrbitalTrail trail;
		trail.radius = base * radiusRatio;
		trail.speed = speed;
		trail.thickness = thickness;
		trail.length = arcLength;
		trail.brightness = brightness;
		trail.modulation = ofRandom(0.18f, 0.65f);
		trail.angle = ofRandom(TWO_PI);
		trail.noiseOffset = ofRandom(100.f, 800.f);
		orbitalTrails.push_back(trail);
	};

	addTrail(0.85f, 0.22f, 2.4f, 0.38f, 0.95f);
	addTrail(1.14f, -0.16f, 2.1f, 0.45f, 0.8f);
	addTrail(0.63f, 0.31f, 1.6f, 0.52f, 0.7f);
	addTrail(1.42f, -0.095f, 1.9f, 0.6f, 0.55f);
	addTrail(0.46f, 0.42f, 1.3f, 0.72f, 0.5f);
	addTrail(1.78f, 0.08f, 2.6f, 0.28f, 0.65f);
}

//--------------------------------------------------------------
void ofApp::updateOrbitalTrails(float deltaTime){
	for (auto &trail : orbitalTrails) {
		trail.angle += trail.speed * deltaTime * TWO_PI;
	}
}

//--------------------------------------------------------------
void ofApp::drawOrbitalTrails(float time){
	if (orbitalTrails.empty()) {
		return;
	}
	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	const float shimmer = 0.5f + 0.5f * sinf(time * 0.5f);

	for (const auto &trail : orbitalTrails) {
		const float radiusPulse = 0.9f + 0.1f * sinf(time * 0.48f + trail.modulation * TWO_PI);
		const float radiusNoise = ofNoise(trail.noiseOffset, time * 0.12f) * 0.12f;
		const float radius = trail.radius * (radiusPulse + radiusNoise);
		const float arc = ofClamp(trail.length + 0.12f * sinf(time * 0.34f + trail.modulation * 3.7f), 0.18f, 0.95f);
		const float alpha = ofClamp(orbitBaseAlpha + trail.brightness * (0.35f + shimmer * 0.55f), 0.f, 0.92f);
		const float arcAngle = arc * TWO_PI;
		const float baseAngle = trail.angle;

		ofSetColor(255, 255, 255, alpha * 255.f);
		ofSetLineWidth(trail.thickness);
		ofNoFill();

		const auto drawArc = [&](float startAngle){
			ofPolyline poly;
			const int steps = 140;
			for (int i = 0; i <= steps; ++i) {
				const float t = static_cast<float>(i) / static_cast<float>(steps);
				const float angle = startAngle + t * arcAngle;
				const float px = center.x + cosf(angle) * radius;
				const float py = center.y + sinf(angle) * radius;
				poly.addVertex(px, py);
			}
			poly.draw();
		};

		drawArc(baseAngle);
		drawArc(baseAngle + PI);

		ofSetColor(255, 255, 255, alpha * 160.f);
		ofFill();
		const float nodeRadius = 1.4f + 1.2f * sinf(time * 0.6f + trail.modulation * 5.1f);
		const glm::vec2 nodeA(center.x + cosf(baseAngle) * radius,
						center.y + sinf(baseAngle) * radius);
		const glm::vec2 nodeB(center.x + cosf(baseAngle + PI) * radius,
						center.y + sinf(baseAngle + PI) * radius);
		ofDrawCircle(nodeA, nodeRadius);
			ofDrawCircle(nodeB, nodeRadius);
	}
}

//--------------------------------------------------------------
void ofApp::buildHarmonicRings(int width, int height){
	harmonicRings.clear();
	const float shorter = width < height ? width : height;

	const struct RingDef {
		float ratio;
		float weight;
		float brightness;
		float rotationSpeed;
		int petals;
		bool dotted;
	} ringDefs[] = {
		{0.28f, 1.7f, 0.78f,  0.04f, 12, true},
		{0.36f, 1.4f, 0.72f, -0.03f,  8, false},
		{0.46f, 1.1f, 0.65f,  0.02f, 16, true},
		{0.58f, 0.95f,0.55f, -0.018f, 10, false},
		{0.71f, 0.8f, 0.45f,  0.012f, 20, true}
	};

	for (const auto &def : ringDefs) {
		HarmonicRing ring;
		ring.radius = shorter * def.ratio;
		ring.weight = def.weight;
		ring.brightness = def.brightness;
		ring.rotation = ofRandom(TWO_PI);
		ring.rotationSpeed = def.rotationSpeed;
		ring.pulseOffset = ofRandom(TWO_PI);
		ring.petals = def.petals;
		ring.dotted = def.dotted;
		harmonicRings.push_back(ring);
	}
}

//--------------------------------------------------------------
void ofApp::updateHarmonicRings(float deltaTime){
	for (auto &ring : harmonicRings) {
		ring.rotation += ring.rotationSpeed * deltaTime;
	}
}

//--------------------------------------------------------------
void ofApp::drawHarmonicRings(float time){
	if (harmonicRings.empty()) {
		return;
	}
	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	const float breath = 0.5f + 0.5f * sinf(time * 0.23f + 0.9f);
	ofPushStyle();
	ofNoFill();
	for (const auto &ring : harmonicRings) {
		const float pulse = 0.65f + 0.35f * sinf(time * 0.52f + ring.pulseOffset);
		const float radius = ring.radius * (0.96f + 0.04f * pulse);
		const float alpha = ofClamp(ring.brightness * (0.32f + breath * 0.48f), 0.f, 0.9f);
		ofSetColor(255, 255, 255, alpha * 255.f);
		ofSetLineWidth(ring.weight);

		if (ring.dotted) {
			const int dots = std::max(12, ring.petals * 6);
			const float baseSize = 0.9f + ring.weight * 1.3f;
			for (int i = 0; i < dots; ++i) {
				const float angle = ring.rotation + (static_cast<float>(i) / static_cast<float>(dots)) * TWO_PI;
				const glm::vec2 pos(center.x + cosf(angle) * radius,
							 center.y + sinf(angle) * radius);
				ofDrawCircle(pos, baseSize * (0.8f + 0.4f * pulse));
			}
		} else {
			ofDrawCircle(center, radius);
		}

		if (ring.petals > 0) {
			ofSetLineWidth(ring.weight * 0.55f);
			for (int i = 0; i < ring.petals; ++i) {
				const float angle = ring.rotation + (static_cast<float>(i) / static_cast<float>(ring.petals)) * TWO_PI;
				const glm::vec2 dir(cosf(angle), sinf(angle));
				const float inner = radius * (0.25f + 0.12f * pulse);
				const float outer = radius * (1.02f + 0.05f * pulse);
				ofDrawLine(center + dir * inner, center + dir * outer);
			}
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::buildLightRays(int width, int height){
	lightRays.clear();
	const float shorter = width < height ? width : height;
	const float innerBase = shorter * 0.22f;
	const float outerBase = shorter * 0.82f;
	const int count = 16;
	lightRays.reserve(count);

	for (int i = 0; i < count; ++i) {
		LightRay ray;
		ray.angle = ofRandom(TWO_PI);
		ray.speed = ofRandom(-0.12f, 0.12f);
		ray.offset = ofRandom(TWO_PI);
		ray.length = ofRandom(0.25f, 0.58f);
		ray.thickness = ofRandom(0.8f, 2.2f);
		ray.radiusInner = innerBase * ofRandom(0.78f, 1.18f);
		ray.radiusOuter = outerBase * ofRandom(0.58f, 0.92f);
		ray.brightness = ofRandom(0.28f, 0.62f);
		ray.noiseOffset = ofRandom(10.f, 900.f);
		lightRays.push_back(ray);
	}
}

//--------------------------------------------------------------
void ofApp::updateLightRays(float deltaTime){
	for (auto &ray : lightRays) {
		ray.angle += ray.speed * deltaTime;
		ray.offset += deltaTime * 0.6f;
	}
}

//--------------------------------------------------------------
void ofApp::drawLightRays(float time){
	if (lightRays.empty()) {
		return;
	}
	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for (const auto &ray : lightRays) {
		const float wave = sinf(time * 0.6f + ray.offset * 2.3f);
		const float noise = ofNoise(ray.noiseOffset, time * 0.35f);
		const float jitter = (noise - 0.5f) * 0.45f;
		const float angle = ray.angle + wave * 0.32f + jitter;
		const float innerRadius = ray.radiusInner * (0.86f + 0.18f * wave);
		const float outerRadius = ray.radiusOuter * (0.9f + 0.2f * noise);
		const glm::vec2 dir(cosf(angle), sinf(angle));
		const glm::vec2 start = center + dir * innerRadius;
		const glm::vec2 end = center + dir * outerRadius;

		const float alpha = ofClamp(ray.brightness * (0.35f + noise * 0.9f), 0.f, 1.f);
		ofSetColor(255, 255, 255, alpha * 255.f);
		ofSetLineWidth(ray.thickness * (0.9f + 0.5f * wave));
		ofDrawLine(start, end);

		const glm::vec2 tangent(-dir.y, dir.x);
		const glm::vec2 sideStart = start + tangent * 4.5f * ray.length;
		const glm::vec2 sideEnd = end + tangent * 1.8f * ray.length;
		ofSetLineWidth(ray.thickness * 0.45f);
		ofSetColor(255, 255, 255, alpha * 140.f);
		ofDrawLine(sideStart, sideEnd);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawBloomEcho(float time){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	const float pulse = 0.4f + 0.6f * sinf(time * 0.19f + 1.7f);
	const float scales[] = {1.035f, 1.07f, 1.11f};
	for (int i = 0; i < 3; ++i) {
		const float scale = scales[i];
		const float alpha = ofClamp(0.018f + pulse * (0.022f + i * 0.008f), 0.01f, 0.12f);
		const float w = ofGetWidth() * scale;
		const float h = ofGetHeight() * scale;
		const float x = (ofGetWidth() - w) * 0.5f;
		const float y = (ofGetHeight() - h) * 0.5f;
		ofSetColor(255, 255, 255, alpha * 255.f);
		trailFbo.draw(x, y, w, h);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::buildRadiantCore(int width, int height){
	corePetalDirections.clear();
	coreSparklePhase.clear();
	const int petals = 18;
	corePetalDirections.reserve(petals);
	coreSparklePhase.reserve(petals);
	for (int i = 0; i < petals; ++i) {
		const float angle = (static_cast<float>(i) / static_cast<float>(petals)) * TWO_PI;
		corePetalDirections.emplace_back(cosf(angle), sinf(angle));
		coreSparklePhase.push_back(ofRandom(TWO_PI));
	}
}

//--------------------------------------------------------------
void ofApp::drawRadiantCore(float time){
	if (corePetalDirections.empty()) {
		return;
	}

	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	const float shorter = ofGetWidth() < ofGetHeight() ? ofGetWidth() : ofGetHeight();
	const float baseRadius = shorter * 0.105f;
	const float breath = 0.55f + 0.45f * sinf(time * 0.42f + 0.8f);
	const float shimmer = 0.45f + 0.55f * sinf(time * 0.87f + 2.1f);

	ofPushStyle();
	ofNoFill();
	ofSetColor(255, 255, 255, (0.15f + 0.35f * breath) * 255.f);
	ofSetLineWidth(1.6f);
	ofDrawCircle(center, baseRadius * (0.58f + 0.22f * breath));
	ofSetLineWidth(1.f);
	ofSetColor(255, 255, 255, (0.12f + 0.32f * shimmer) * 255.f);
	ofDrawCircle(center, baseRadius * (1.12f + 0.08f * shimmer));
	ofDrawCircle(center, baseRadius * (1.48f + 0.12f * breath));

	ofPushMatrix();
	ofTranslate(center.x, center.y);
	for (std::size_t i = 0; i < corePetalDirections.size(); ++i) {
		const glm::vec2 dir = corePetalDirections[i];
		const float angle = atan2f(dir.y, dir.x);
		const float phase = coreSparklePhase[i];
		const float petalPulse = 0.65f + 0.35f * sinf(time * 0.9f + phase);
		const float stroke = 0.7f + 0.6f * sinf(time * 0.73f + phase * 1.7f);
		const float radius = baseRadius * (1.85f + petalPulse * 0.7f);
		const float width = baseRadius * (0.28f + 0.14f * shimmer);

		ofPushMatrix();
		ofRotateRad(angle);
		ofSetLineWidth(0.6f + stroke * 0.9f);
		ofSetColor(255, 255, 255, (0.16f + 0.42f * petalPulse) * 255.f);
		ofDrawEllipse(0.f, radius * 0.55f, width, radius);
		ofPopMatrix();
	}
	ofPopMatrix();

	ofFill();
	const float corePulse = 0.3f + 0.7f * sinf(time * 1.1f + 0.4f);
	ofSetColor(255, 255, 255, (0.22f + 0.4f * corePulse) * 255.f);
	ofDrawCircle(center, baseRadius * 0.35f * (0.9f + 0.2f * corePulse));
	ofSetColor(255, 255, 255, (0.18f + 0.52f * shimmer) * 255.f);
	ofDrawCircle(center, 1.2f + 1.2f * shimmer);

	for (std::size_t i = 0; i < corePetalDirections.size(); ++i) {
		const glm::vec2 dir = corePetalDirections[i];
		const float phase = coreSparklePhase[i];
		const float sparkle = 0.55f + 0.45f * sinf(time * 1.3f + phase * 2.4f);
		const glm::vec2 pos = center + dir * baseRadius * (1.6f + 0.5f * sparkle);
		ofSetColor(255, 255, 255, (0.12f + 0.5f * sparkle) * 255.f);
		ofDrawCircle(pos, 0.9f + sparkle * 1.4f);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::buildSacredGeometry(int width, int height){
	sacredLayers.clear();
	const float shorter = width < height ? width : height;

	const auto addLayer = [&](int sides, float radiusRatio, float speed, float weight, float bright,
				bool radials, int starStep, bool innerCircle, float innerRatio){
		SacredLayer layer;
		layer.radius = shorter * radiusRatio;
		layer.rotationSpeed = speed;
		layer.lineWeight = weight;
		layer.brightness = bright;
		layer.drawRadials = radials;
		layer.radialWeight = weight * 0.7f;
		layer.starStep = starStep;
		layer.drawInnerCircle = innerCircle;
		layer.innerCircleRatio = innerRatio;
		layer.rotation = ofRandom(TWO_PI);
		layer.unitVertices.reserve(sides);
		for (int i = 0; i < sides; ++i) {
			const float angle = (static_cast<float>(i) / static_cast<float>(sides)) * TWO_PI;
			layer.unitVertices.emplace_back(cosf(angle), sinf(angle));
		}
		sacredLayers.push_back(layer);
	};

	addLayer(3, 0.26f, 0.12f, 1.4f, 0.9f, true, 0, true, 0.58f);
	addLayer(6, 0.34f, -0.08f, 1.15f, 0.8f, true, 2, false, 0.0f);
	addLayer(12, 0.44f, 0.05f, 0.9f, 0.6f, false, 0, true, 0.72f);
	addLayer(8, 0.52f, -0.035f, 0.8f, 0.45f, true, 3, false, 0.0f);
	addLayer(24, 0.61f, 0.018f, 0.65f, 0.35f, false, 4, false, 0.0f);
}

//--------------------------------------------------------------
void ofApp::updateSacredGeometry(float deltaTime){
	for (auto &layer : sacredLayers) {
		layer.rotation += layer.rotationSpeed * deltaTime;
	}
}

//--------------------------------------------------------------
void ofApp::drawSacredGeometry(float time){
	if (sacredLayers.empty()) {
		return;
	}

	const glm::vec2 center(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	const float breath = 0.5f + 0.5f * sinf(time * 0.22f + 1.4f);
	const float shimmer = 0.5f + 0.5f * sinf(time * 0.4f + 2.7f);
	const float globalPulse = geometryPulse + breath * 0.26f + shimmer * 0.18f;

	for (const auto &layer : sacredLayers) {
		if (layer.unitVertices.empty()) {
			continue;
		}

		const float cosR = cosf(layer.rotation);
		const float sinR = sinf(layer.rotation);
		std::vector<glm::vec2> positions;
		positions.reserve(layer.unitVertices.size());

		for (std::size_t i = 0; i < layer.unitVertices.size(); ++i) {
			const glm::vec2 &base = layer.unitVertices[i];
			glm::vec2 rotated(base.x * cosR - base.y * sinR, base.x * sinR + base.y * cosR);
			const float subtle = 1.f + 0.025f * sinf(time * 0.55f + static_cast<float>(i) * 0.9f);
			const glm::vec2 pos = center + rotated * layer.radius * (0.88f + 0.12f * breath) * subtle;
			positions.push_back(pos);
		}

		const float layerAlpha = ofClamp(geometryBaseAlpha + layer.brightness * globalPulse, 0.f, 0.85f);
		ofNoFill();
		ofSetColor(255, 255, 255, layerAlpha * 255.f);
		ofSetLineWidth(layer.lineWeight);
		ofBeginShape();
		for (const auto &p : positions) {
			ofVertex(p.x, p.y);
		}
		ofEndShape(true);

		if (layer.starStep > 0 && positions.size() > 2) {
			ofSetLineWidth(layer.lineWeight * 0.65f);
			for (std::size_t i = 0; i < positions.size(); ++i) {
				const glm::vec2 &a = positions[i];
				const glm::vec2 &b = positions[(i + layer.starStep) % positions.size()];
				ofDrawLine(a, b);
			}
		}

		if (layer.drawRadials) {
			ofSetLineWidth(layer.radialWeight);
			for (const auto &p : positions) {
				ofDrawLine(center, p);
			}
		}

		if (layer.drawInnerCircle) {
			ofSetLineWidth(layer.lineWeight * 0.6f);
			const float circleAlpha = layerAlpha * 0.8f;
			ofSetColor(255, 255, 255, circleAlpha * 255.f);
			ofDrawCircle(center, layer.radius * layer.innerCircleRatio);
		}

		ofFill();
		ofSetColor(255, 255, 255, layerAlpha * 128.f);
		for (const auto &p : positions) {
			const float pointPulse = 0.7f + 0.3f * sinf(time * 0.7f + (p.x + p.y) * 0.002f);
			ofDrawCircle(p, 0.8f + pointPulse * 1.2f);
		}
	}
}
