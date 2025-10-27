#include "ofApp.h"

void ofApp::setup(){
    // Performance optimizations
    ofSetVerticalSync(true); // Enable VSync for stable FPS
    ofSetFrameRate(60); // Target 60 FPS
    ofSetBackgroundColor(10, 15, 25);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofSetCircleResolution(16); // Reduced for performance
    ofSetSphereResolution(12); // Reduced for performance

    // Disable auto background clearing for manual control
    ofSetBackgroundAuto(false);

    // Initialize color palette with theme colors
    colorPalette.push_back(ofColor::fromHex(0x362d78));
    colorPalette.push_back(ofColor::fromHex(0x523fa3));
    colorPalette.push_back(ofColor::fromHex(0x916ccc));
    colorPalette.push_back(ofColor::fromHex(0xbda1e5));
    colorPalette.push_back(ofColor::fromHex(0xc8c0e9));
    colorPalette.push_back(ofColor::fromHex(0x84bae7));
    colorPalette.push_back(ofColor::fromHex(0x516ad4));
    colorPalette.push_back(ofColor::fromHex(0x333f87));
    colorPalette.push_back(ofColor::fromHex(0x293039));
    colorPalette.push_back(ofColor::fromHex(0x283631));

    time = 0;
    numParticles = 300; // Reduced for better performance
    colorShiftSpeed = 0.5;
    terrainSize = 30; // Reduced for better performance

    // Setup FBOs for post-processing
    mainFbo.allocate(720, 1080, GL_RGBA);
    blurFbo.allocate(720, 1080, GL_RGBA);
    setupShaders();

    initializeParticles();

    // Initialize terrain
    terrainHeights.resize(terrainSize);
    for(int i = 0; i < terrainSize; i++){
        terrainHeights[i].resize(terrainSize);
    }
    updateTerrain();

    // Initialize galaxy (reduced count)
    for(int i = 0; i < 120; i++){
        GalaxyPoint point;
        point.age = ofRandom(0, 100);
        point.size = ofRandom(1, 4);
        point.color = getRandomPaletteColor();
        galaxyPoints.push_back(point);
    }

    // Initial fractal trees will be generated in draw loop

    cam.setDistance(600);

    // Setup cinematic camera system
    cinematicMode = true;
    currentKeyframe = 0;
    keyframeProgress = 0;
    cameraShakeIntensity = 0; // Start with no shake
    enableOrganicMovement = false; // Disable for ultra-smooth camera
    setupCameraSequence();
}

void ofApp::update(){
    time = ofGetElapsedTimef();
    updateParticles();
    updateTerrain();
    updateGalaxy();

    if(cinematicMode){
        updateCinematicCamera();
    }
}

void ofApp::draw(){
    // Manual background clear to prevent flicker
    ofBackground(10, 15, 25);

    mainFbo.begin();
    ofClear(10, 15, 25, 255);

    if(cinematicMode){
        // Manual camera control for cinematic mode
        ofVec3f shakeOffset = getCameraShake();
        ofVec3f finalPos = currentCamPos + shakeOffset;
        ofVec3f finalTarget = currentCamTarget + shakeOffset * 0.1;

        ofCamera cinematicCam;
        cinematicCam.setPosition(finalPos);
        cinematicCam.lookAt(finalTarget);
        cinematicCam.begin();

        // Optimized rendering order
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        drawTerrain();

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        drawGalaxy();
        drawGenerativeShapes();
        drawFractalTrees();

        ofDisableBlendMode(); // Reset blend mode
        cinematicCam.end();
    } else {
        cam.begin();

        // Optimized rendering order
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        drawTerrain();

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        drawGalaxy();
        drawGenerativeShapes();
        drawFractalTrees();

        ofDisableBlendMode(); // Reset blend mode
        cam.end();
    }
    mainFbo.end();

    // Apply glow effect
    blurFbo.begin();
    ofClear(0, 0, 0, 255);
    if(blurShader.isLoaded()){
        blurShader.begin();
        blurShader.setUniform1f("blurAmount", 3.0);
        blurShader.setUniform2f("textureSize", 720, 1080);
        mainFbo.draw(0, 0);
        blurShader.end();
    } else {
        mainFbo.draw(0, 0);
    }
    blurFbo.end();

    // Draw final composite with proper color reset
    ofSetColor(255, 255, 255, 255);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    mainFbo.draw(0, 0);

    // Add subtle glow layer
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    ofSetColor(120, 120, 200, 80); // Reduced glow intensity
    blurFbo.draw(0, 0);

    // Reset all rendering state
    ofSetColor(255, 255, 255, 255);
    ofDisableBlendMode();
}

void ofApp::initializeParticles(){
    particles.clear();
    velocities.clear();
    sizes.clear();
    particleColors.clear();

    for(int i = 0; i < numParticles; i++){
        ofVec3f pos(
            ofRandom(-200, 200),
            ofRandom(-300, 300),
            ofRandom(-200, 200)
        );
        particles.push_back(pos);

        ofVec3f vel(
            ofRandom(-0.5, 0.5),
            ofRandom(-0.5, 0.5),
            ofRandom(-0.5, 0.5)
        );
        velocities.push_back(vel);

        sizes.push_back(ofRandom(2, 15));
        particleColors.push_back(getRandomPaletteColor());
    }
}

void ofApp::updateParticles(){
    for(int i = 0; i < particles.size(); i++){
        float noiseX = ofNoise(particles[i].x * 0.01, time * 0.1) * 2 - 1;
        float noiseY = ofNoise(particles[i].y * 0.01, time * 0.1) * 2 - 1;
        float noiseZ = ofNoise(particles[i].z * 0.01, time * 0.1) * 2 - 1;

        velocities[i] += ofVec3f(noiseX, noiseY, noiseZ) * 0.1;
        velocities[i] *= 0.98;

        particles[i] += velocities[i];

        if(particles[i].x > 300) particles[i].x = -300;
        if(particles[i].x < -300) particles[i].x = 300;
        if(particles[i].y > 400) particles[i].y = -400;
        if(particles[i].y < -400) particles[i].y = 400;
        if(particles[i].z > 300) particles[i].z = -300;
        if(particles[i].z < -300) particles[i].z = 300;
    }
}

void ofApp::drawGenerativeShapes(){
    ofPushMatrix();
    ofRotateYDeg(time * 5);

    // Generate fractal trees dynamically
    for(int i = 0; i < 3; i++){
        float treeX = cos(i * TWO_PI / 3 + time * 0.1) * 250;
        float treeZ = sin(i * TWO_PI / 3 + time * 0.1) * 250;
        ofVec3f start(treeX, 150, treeZ);
        ofVec3f direction(0, -1, 0);
        generateFractalTree(start, direction, 60 + sin(time + i) * 20, 6, 0, 5);
    }

    // Optimized particle connections (reduced layers and frequency)
    ofSetLineWidth(1.0);
    for(int layer = 0; layer < 2; layer++){ // Reduced from 3 to 2 layers
        for(int i = 0; i < particles.size(); i += 2){ // Skip every other particle
            for(int j = i + layer * 30; j < particles.size(); j += 4){ // Larger steps
                if(j >= particles.size()) break;
                float distance = particles[i].distance(particles[j]);
                if(distance < 100 - layer * 25){ // Shorter connection distance
                    float alpha = ofMap(distance, 0, 100 - layer * 25, 120 - layer * 30, 0);
                    ofColor lineColor = getInterpolatedColor(sin(time * 0.2 + i * 0.05) * 0.5 + 0.5);
                    lineColor.a = alpha * 0.2;
                    ofSetColor(lineColor);
                    ofDrawLine(particles[i], particles[j]);
                }
            }
        }
    }

    // Enhanced particles with multiple visual layers
    for(int i = 0; i < particles.size(); i++){
        ofPushMatrix();
        ofTranslate(particles[i]);

        float pulseSize = sizes[i] + sin(time * 1.5 + i * 0.08) * 3;
        float brightness = sin(time * 2 + i * 0.1) * 0.3 + 0.7;

        // Core particle
        ofColor coreColor = particleColors[i];
        coreColor.setBrightness(coreColor.getBrightness() * brightness);
        ofSetColor(coreColor, 220);
        ofDrawSphere(0, 0, 0, pulseSize * 0.6);

        // Glow halo
        ofColor haloColor = coreColor;
        haloColor.a = 80;
        ofSetColor(haloColor);
        ofDrawSphere(0, 0, 0, pulseSize * 1.5);

        // Tiny orbiting satellites (reduced frequency)
        if(i % 20 == 0 && i < 100){ // Only for first 100 particles, every 20th
            for(int sat = 0; sat < 2; sat++){ // Reduced from 3 to 2 satellites
                float orbitAngle = time * 2 + sat * PI;
                float orbitRadius = pulseSize * 1.5;
                ofVec3f satPos(cos(orbitAngle) * orbitRadius, sin(orbitAngle * 1.2) * orbitRadius * 0.4, sin(orbitAngle) * orbitRadius);
                ofSetColor(getInterpolatedColor(sin(time * 0.5 + sat) * 0.5 + 0.5), 120);
                ofDrawSphere(satPos, 1.5);
            }
        }

        ofPopMatrix();
    }

    // Enhanced central geometric installation
    ofPushMatrix();
    ofRotateXDeg(time * 12);
    ofRotateZDeg(time * 8);

    for(int ring = 0; ring < 3; ring++){
        float ringRadius = 80 + ring * 40;
        int shapesInRing = 6 + ring * 2;

        for(int i = 0; i < shapesInRing; i++){
            ofPushMatrix();
            ofRotateYDeg(i * 360.0 / shapesInRing + time * 15 * (ring + 1));
            ofTranslate(ringRadius, 0, 0);
            ofRotateXDeg(time * 30 + ring * 45);

            float shapeSize = 15 + sin(time * 2 + i + ring) * 8;
            ofColor shapeColor = getInterpolatedColor((float)(i + ring * 2) / (shapesInRing + 6));
            shapeColor.a = 180;
            ofSetColor(shapeColor);

            // Vary shape types
            if(ring == 0) ofDrawBox(shapeSize);
            else if(ring == 1) ofDrawSphere(0, 0, 0, shapeSize * 0.7);
            else ofDrawCone(shapeSize * 0.8, shapeSize * 1.2);

            ofPopMatrix();
        }
    }

    ofPopMatrix();

    // Optimized flowing ribbons (reduced count and complexity)
    ofSetLineWidth(2);
    for(int ribbon = 0; ribbon < 3; ribbon++){ // Reduced from 5 to 3
        ofBeginShape();
        ofNoFill();

        float ribbonSpeed = 0.4 + ribbon * 0.3;
        float helixRadius = 140 + ribbon * 40;

        for(int i = 0; i < 80; i++){ // Reduced from 150 to 80 points
            float t = i * 0.1 + time * ribbonSpeed;
            float x = cos(t) * helixRadius * (1 + sin(t * 0.2) * 0.15);
            float y = i * 4 - 160 + sin(t * 1.5) * 30;
            float z = sin(t) * helixRadius * (1 + cos(t * 0.2) * 0.15);

            float colorT = sin(t * 0.3 + ribbon) * 0.5 + 0.5;
            ofColor ribbonColor = getInterpolatedColor(colorT);
            ribbonColor.a = ofMap(i, 0, 80, 180, 40);

            ofSetColor(ribbonColor);
            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    // Floating energy orbs (reduced count)
    for(int orb = 0; orb < 5; orb++){ // Reduced from 8 to 5
        float orbTime = time * 0.3 + orb * 0.8;
        float x = cos(orbTime) * 200 + sin(orbTime * 0.7) * 100;
        float y = sin(orbTime * 1.2) * 150;
        float z = sin(orbTime) * 200 + cos(orbTime * 0.7) * 100;

        ofColor orbColor = getInterpolatedColor(sin(orbTime) * 0.5 + 0.5);
        orbColor.a = 120;
        ofSetColor(orbColor);

        ofPushMatrix();
        ofTranslate(x, y, z);
        float orbSize = 12 + sin(orbTime * 4) * 6;
        ofDrawSphere(0, 0, 0, orbSize);

        // Energy trails
        ofSetColor(orbColor, 60);
        ofDrawSphere(0, 0, 0, orbSize * 2);
        ofPopMatrix();
    }

    ofPopMatrix();
}

void ofApp::setupShaders(){
    string blurVertexShader = R"(
        #version 150
        uniform mat4 modelViewProjectionMatrix;
        in vec4 position;
        in vec2 texcoord;
        out vec2 texCoordVarying;
        void main(){
            texCoordVarying = texcoord;
            gl_Position = modelViewProjectionMatrix * position;
        }
    )";

    string blurFragmentShader = R"(
        #version 150
        uniform sampler2D tex0;
        uniform float blurAmount;
        uniform vec2 textureSize;
        in vec2 texCoordVarying;
        out vec4 outputColor;
        void main(){
            vec2 texelSize = 1.0 / textureSize;
            vec4 color = vec4(0.0);
            for(int x = -2; x <= 2; x++){
                for(int y = -2; y <= 2; y++){
                    vec2 offset = vec2(float(x), float(y)) * texelSize * blurAmount;
                    color += texture(tex0, texCoordVarying + offset);
                }
            }
            outputColor = color / 25.0;
        }
    )";

    blurShader.setupShaderFromSource(GL_VERTEX_SHADER, blurVertexShader);
    blurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, blurFragmentShader);
    blurShader.linkProgram();
}

void ofApp::generateFractalTree(ofVec3f start, ofVec3f direction, float length, float thickness, int generation, int maxGen){
    if(generation > maxGen || length < 5) return;

    ofVec3f end = start + direction * length;

    FractalBranch branch;
    branch.start = start;
    branch.end = end;
    branch.thickness = thickness;
    branch.generation = generation;
    branch.color = getInterpolatedColor((float)generation / maxGen);
    fractalBranches.push_back(branch);

    if(generation < maxGen){
        float angleVariation = 30 + sin(time * 0.1 + generation) * 10;
        float lengthDecay = 0.7 + sin(time * 0.05) * 0.1;

        // Create two new branches
        ofVec3f newDir1 = direction.getRotated(angleVariation, ofVec3f(1, 0, 0));
        ofVec3f newDir2 = direction.getRotated(-angleVariation, ofVec3f(0, 0, 1));

        generateFractalTree(end, newDir1, length * lengthDecay, thickness * 0.7, generation + 1, maxGen);
        generateFractalTree(end, newDir2, length * lengthDecay, thickness * 0.7, generation + 1, maxGen);
    }
}

void ofApp::drawFractalTrees(){
    for(auto& branch : fractalBranches){
        ofSetColor(branch.color, 150);
        ofSetLineWidth(branch.thickness * (1 + sin(time + branch.generation) * 0.2));
        ofDrawLine(branch.start, branch.end);

        // Add glowing endpoints
        ofSetColor(branch.color, 200);
        ofDrawSphere(branch.end, branch.thickness * 0.5);
    }
    fractalBranches.clear(); // Clear and regenerate each frame for animation
}

void ofApp::updateTerrain(){
    for(int x = 0; x < terrainSize; x++){
        for(int z = 0; z < terrainSize; z++){
            float height = 0;
            height += ofNoise(x * 0.05, z * 0.05, time * 0.1) * 50;
            height += ofNoise(x * 0.1, z * 0.1, time * 0.2) * 25;
            height += ofNoise(x * 0.2, z * 0.2, time * 0.3) * 12;
            terrainHeights[x][z] = height - 100;
        }
    }
}

void ofApp::drawTerrain(){
    terrainMesh.clear();
    terrainMesh.setMode(OF_PRIMITIVE_TRIANGLES);

    for(int x = 0; x < terrainSize - 1; x++){
        for(int z = 0; z < terrainSize - 1; z++){
            float worldX1 = (x - terrainSize/2) * 20;
            float worldZ1 = (z - terrainSize/2) * 20;
            float worldX2 = (x + 1 - terrainSize/2) * 20;
            float worldZ2 = (z + 1 - terrainSize/2) * 20;

            ofVec3f p1(worldX1, terrainHeights[x][z], worldZ1);
            ofVec3f p2(worldX2, terrainHeights[x+1][z], worldZ1);
            ofVec3f p3(worldX1, terrainHeights[x][z+1], worldZ2);
            ofVec3f p4(worldX2, terrainHeights[x+1][z+1], worldZ2);

            float colorFactor = (terrainHeights[x][z] + 100) / 150.0;
            ofColor terrainColor = getInterpolatedColor(colorFactor);
            terrainColor.a = 120;

            terrainMesh.addVertex(p1);
            terrainMesh.addColor(terrainColor);
            terrainMesh.addVertex(p2);
            terrainMesh.addColor(terrainColor);
            terrainMesh.addVertex(p3);
            terrainMesh.addColor(terrainColor);

            terrainMesh.addVertex(p2);
            terrainMesh.addColor(terrainColor);
            terrainMesh.addVertex(p4);
            terrainMesh.addColor(terrainColor);
            terrainMesh.addVertex(p3);
            terrainMesh.addColor(terrainColor);
        }
    }

    terrainMesh.draw();
}

void ofApp::updateGalaxy(){
    for(auto& point : galaxyPoints){
        point.age += 0.5;

        // Spiral motion
        float angle = point.age * 0.1;
        float radius = point.age * 2;
        float spiralX = cos(angle) * radius;
        float spiralZ = sin(angle) * radius;
        float spiralY = sin(point.age * 0.05) * 30;

        point.position = ofVec3f(spiralX, spiralY, spiralZ);

        // Reset when too far
        if(radius > 400){
            point.age = 0;
            point.color = getRandomPaletteColor();
            point.size = ofRandom(1, 4);
        }
    }
}

void ofApp::drawGalaxy(){
    for(auto& point : galaxyPoints){
        ofPushMatrix();
        ofTranslate(point.position);

        float alpha = ofMap(point.age, 0, 200, 255, 0);
        ofColor glowColor = point.color;
        glowColor.a = alpha * 0.8;

        ofSetColor(glowColor);
        ofDrawSphere(0, 0, 0, point.size);

        // Draw trailing effect
        ofSetColor(glowColor, alpha * 0.3);
        ofDrawSphere(0, 0, 0, point.size * 2);

        ofPopMatrix();
    }
}

ofColor ofApp::getRandomPaletteColor(){
    int index = (int)ofRandom(colorPalette.size());
    return colorPalette[index];
}

ofColor ofApp::getInterpolatedColor(float t){
    t = ofClamp(t + sin(time * colorShiftSpeed) * 0.3, 0, 1);

    float scaledT = t * (colorPalette.size() - 1);
    int index1 = (int)scaledT;
    int index2 = (index1 + 1) % colorPalette.size();
    float blend = scaledT - index1;

    return colorPalette[index1].getLerped(colorPalette[index2], blend);
}

void ofApp::setupCameraSequence(){
    cameraKeyframes.clear();

    // Opening shot - Distant overview
    CameraKeyframe frame1;
    frame1.position = ofVec3f(0, 200, 800);
    frame1.target = ofVec3f(0, 0, 0);
    frame1.duration = 4.0;
    frame1.easeType = 1; // ease-in-out
    cameraKeyframes.push_back(frame1);

    // Dive into galaxy center
    CameraKeyframe frame2;
    frame2.position = ofVec3f(100, 50, 300);
    frame2.target = ofVec3f(0, 0, 0);
    frame2.duration = 3.5;
    frame2.easeType = 2; // ease-in
    cameraKeyframes.push_back(frame2);

    // Spiral around central installation
    CameraKeyframe frame3;
    frame3.position = ofVec3f(-200, 100, 200);
    frame3.target = ofVec3f(0, 0, 0);
    frame3.duration = 3.0;
    frame3.easeType = 0; // linear
    cameraKeyframes.push_back(frame3);

    // Low angle terrain flyover
    CameraKeyframe frame4;
    frame4.position = ofVec3f(300, -50, 100);
    frame4.target = ofVec3f(-200, -80, -100);
    frame4.duration = 4.0;
    frame4.easeType = 1; // ease-in-out
    cameraKeyframes.push_back(frame4);

    // High above looking down at particles
    CameraKeyframe frame5;
    frame5.position = ofVec3f(0, 400, 200);
    frame5.target = ofVec3f(0, 0, 0);
    frame5.duration = 3.0;
    frame5.easeType = 3; // ease-out
    cameraKeyframes.push_back(frame5);

    // Close-up on fractal trees
    CameraKeyframe frame6;
    frame6.position = ofVec3f(250, 120, 250);
    frame6.target = ofVec3f(250, 50, 250);
    frame6.duration = 3.5;
    frame6.easeType = 1; // ease-in-out
    cameraKeyframes.push_back(frame6);

    // Dramatic side angle
    CameraKeyframe frame7;
    frame7.position = ofVec3f(500, 0, 0);
    frame7.target = ofVec3f(0, 0, 0);
    frame7.duration = 3.0;
    frame7.easeType = 2; // ease-in
    cameraKeyframes.push_back(frame7);

    // Sweeping orbit
    CameraKeyframe frame8;
    frame8.position = ofVec3f(-300, 150, 400);
    frame8.target = ofVec3f(0, 50, 0);
    frame8.duration = 4.5;
    frame8.easeType = 0; // linear
    cameraKeyframes.push_back(frame8);

    // Inside the particle cloud
    CameraKeyframe frame9;
    frame9.position = ofVec3f(50, 20, 80);
    frame9.target = ofVec3f(-50, 30, -80);
    frame9.duration = 2.5;
    frame9.easeType = 3; // ease-out
    cameraKeyframes.push_back(frame9);

    // Final pullback to overview
    CameraKeyframe frame10;
    frame10.position = ofVec3f(200, 300, 600);
    frame10.target = ofVec3f(0, 0, 0);
    frame10.duration = 4.0;
    frame10.easeType = 1; // ease-in-out
    cameraKeyframes.push_back(frame10);

    currentCamPos = cameraKeyframes[0].position;
    currentCamTarget = cameraKeyframes[0].target;
}

void ofApp::updateCinematicCamera(){
    if(cameraKeyframes.empty()) return;

    float deltaTime = ofGetLastFrameTime();
    keyframeProgress += deltaTime;

    CameraKeyframe& currentFrame = cameraKeyframes[currentKeyframe];

    if(keyframeProgress >= currentFrame.duration){
        // Move to next keyframe
        currentKeyframe = (currentKeyframe + 1) % cameraKeyframes.size();
        keyframeProgress = 0;

        // Very subtle shake only on transitions (optional)
        // cameraShakeIntensity = 0.5;
    }

    // Interpolate between current and next keyframe
    int nextKeyframe = (currentKeyframe + 1) % cameraKeyframes.size();
    CameraKeyframe& nextFrame = cameraKeyframes[nextKeyframe];

    float t = keyframeProgress / currentFrame.duration;

    // Apply easing
    float easedT = t;
    switch((int)currentFrame.easeType){
        case 1: easedT = easeInOutCubic(t); break;
        case 2: easedT = easeInCubic(t); break;
        case 3: easedT = easeOutCubic(t); break;
        default: easedT = t; break; // linear
    }

    // Smooth interpolation
    currentCamPos = currentFrame.position.getInterpolated(nextFrame.position, easedT);
    currentCamTarget = currentFrame.target.getInterpolated(nextFrame.target, easedT);

    // Add very subtle organic movement (only if enabled)
    if(enableOrganicMovement){
        float organicTime = time * 0.15; // Slower movement
        currentCamPos += ofVec3f(
            sin(organicTime) * 1.5, // Much smaller movement
            cos(organicTime * 1.3) * 1.0,
            sin(organicTime * 0.7) * 1.2
        );
    }

    // Reduce shake over time more aggressively
    cameraShakeIntensity *= 0.85;
    if(cameraShakeIntensity < 0.01) cameraShakeIntensity = 0;
}

float ofApp::easeInOutCubic(float t){
    if(t < 0.5){
        return 4 * t * t * t;
    } else {
        float p = 2 * t - 2;
        return 1 + p * p * p / 2;
    }
}

float ofApp::easeInCubic(float t){
    return t * t * t;
}

float ofApp::easeOutCubic(float t){
    float p = t - 1;
    return p * p * p + 1;
}

ofVec3f ofApp::getCameraShake(){
    if(cameraShakeIntensity <= 0.05) return ofVec3f(0, 0, 0);

    float shakeTime = time * 25; // Reduced frequency
    return ofVec3f(
        sin(shakeTime) * cameraShakeIntensity * 0.3, // Much reduced intensity
        cos(shakeTime * 1.3) * cameraShakeIntensity * 0.3,
        sin(shakeTime * 0.7) * cameraShakeIntensity * 0.3
    );
}

void ofApp::keyPressed(int key){
    if(key == ' '){
        // Regenerate particles with explosion effect
        initializeParticles();
        for(int i = 0; i < particles.size(); i++){
            velocities[i] *= 5; // Explosion effect
        }
    }
    if(key == 'r' || key == 'R'){
        // Reset color shift speed
        colorShiftSpeed = ofRandom(0.1, 2.0);
    }
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
    if(key == 'c' || key == 'C'){
        // Toggle between cinematic and manual camera
        cinematicMode = !cinematicMode;
        if(cinematicMode){
            setupCameraSequence();
        }
    }
    if(key == 'n' || key == 'N'){
        // Skip to next camera keyframe
        if(cinematicMode && !cameraKeyframes.empty()){
            currentKeyframe = (currentKeyframe + 1) % cameraKeyframes.size();
            keyframeProgress = 0;
            // No shake on manual keyframe skip
        }
    }
    if(key == 's' || key == 'S'){
        // Add subtle shake effect (reduced intensity)
        cameraShakeIntensity = 2.0;
    }
    if(key == 'o' || key == 'O'){
        // Toggle organic camera movement
        enableOrganicMovement = !enableOrganicMovement;
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){
    // Mouse influence on particles
    ofVec3f mouseWorld = cam.screenToWorld(ofVec3f(x, y, 0));
    float mouseInfluence = ofMap(mouseX, 0, ofGetWidth(), 0.5, 2.0);

    for(int i = 0; i < particles.size(); i++){
        float distance = particles[i].distance(mouseWorld);
        if(distance < 200){
            ofVec3f force = (particles[i] - mouseWorld).normalize() * mouseInfluence * 0.1;
            velocities[i] += force;
        }
    }
}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::gotMessage(ofMessage msg){

}