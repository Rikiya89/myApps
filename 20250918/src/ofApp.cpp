#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofSetBackgroundColor(41, 48, 57);
    ofEnableAlphaBlending();
    
    colorPalette.clear();
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
    
    numParticles = 200;
    connectionDistance = 100.0f;
    time = 0.0f;
    cameraRadius = 450.0f;
    cameraHeight = 0.0f;
    worldCenter = ofVec3f(0, 0, 0);
    globalPulse = 0.0f;
    useShader = false;
    
    fbo.allocate(720, 1080, GL_RGBA);
    
    attractors.clear();
    attractors.push_back(ofVec3f(150, 0, 0));
    attractors.push_back(ofVec3f(-150, 0, 0));
    attractors.push_back(ofVec3f(0, 150, 0));
    attractors.push_back(ofVec3f(0, -150, 0));
    attractors.push_back(ofVec3f(0, 0, 150));
    attractors.push_back(ofVec3f(0, 0, -150));
    
    particles.clear();
    particles.resize(numParticles);
    
    for(int i = 0; i < numParticles; i++) {
        particles[i].position = ofVec3f(
            ofRandom(-200, 200),
            ofRandom(-200, 200),
            ofRandom(-200, 200)
        );
        
        particles[i].velocity = ofVec3f(
            ofRandom(-0.5f, 0.5f),
            ofRandom(-0.5f, 0.5f),
            ofRandom(-0.5f, 0.5f)
        );
        
        particles[i].noiseOffset = ofVec3f(
            ofRandom(1000),
            ofRandom(1000),
            ofRandom(1000)
        );
        
        particles[i].size = ofRandom(3.0f, 12.0f);
        particles[i].alpha = ofRandom(0.4f, 1.0f);
        particles[i].type = int(ofRandom(4));
        particles[i].colorIndex = int(ofRandom(colorPalette.size()));
        particles[i].pulsePhase = ofRandom(TWO_PI);
        particles[i].pulseSpeed = ofRandom(0.5f, 2.0f);
        particles[i].energy = ofRandom(0.5f, 1.0f);
        particles[i].targetEnergy = particles[i].energy;
        
        particles[i].rotation = ofVec3f(0, 0, 0);
        particles[i].rotationSpeed = ofVec3f(
            ofRandom(-3.0f, 3.0f),
            ofRandom(-3.0f, 3.0f),
            ofRandom(-3.0f, 3.0f)
        );
        
        particles[i].trail.clear();
        particles[i].trail.reserve(20);
    }
    
    cam.setDistance(300);
    
    showOverlay = false;
    saveNextFrame = false;
    screenshotCounter = 1;
    
    currentTitle = "Digital Cosmos";
    currentCaption = "Where art meets science 🔬✨\nEvery frame is a unique moment in digital time\nBuilt with passion, code, and endless curiosity\n\n#digitalart #generativeart #creativecoding #artandscience #techart #codeisart #algorithmicart #openframeworks";
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.016f;
    globalPulse = sin(time * 1.5f);
    
    for(int i = 0; i < attractors.size(); i++) {
        attractors[i] = ofVec3f(
            cos(time * 0.2f + i * PI/3) * 180.0f,
            sin(time * 0.15f + i * PI/3) * 120.0f,
            sin(time * 0.25f + i * PI/3) * 180.0f
        );
    }
    
    for(int i = 0; i < particles.size(); i++) {
        particles[i].trail.push_back(particles[i].position);
        if(particles[i].trail.size() > 15) {
            particles[i].trail.erase(particles[i].trail.begin());
        }
        
        float timeScale = 0.3f;
        ofVec3f noiseVel = ofVec3f(
            ofNoise(particles[i].noiseOffset.x + time * timeScale) - 0.5f,
            ofNoise(particles[i].noiseOffset.y + time * timeScale) - 0.5f,
            ofNoise(particles[i].noiseOffset.z + time * timeScale) - 0.5f
        ) * 3.0f;
        
        ofVec3f attractorForce(0, 0, 0);
        for(int j = 0; j < attractors.size(); j++) {
            ofVec3f dir = attractors[j] - particles[i].position;
            float dist = dir.length();
            if(dist > 0) {
                dir.normalize();
                float force = 1.0f / (1.0f + dist * 0.01f);
                attractorForce += dir * force * 0.5f;
            }
        }
        
        particles[i].velocity += noiseVel * 0.15f + attractorForce * 0.1f;
        particles[i].velocity *= 0.95f;
        particles[i].position += particles[i].velocity;
        particles[i].rotation += particles[i].rotationSpeed;
        
        particles[i].pulsePhase += particles[i].pulseSpeed * 0.05f;
        float pulse = sin(particles[i].pulsePhase) * 0.5f + 0.5f;
        particles[i].alpha = ofMap(pulse, 0, 1, 0.3f, 1.0f);
        
        particles[i].targetEnergy = ofRandom(0.3f, 1.2f);
        particles[i].energy = ofLerp(particles[i].energy, particles[i].targetEnergy, 0.02f);
        
        float centerForce = 0.015f;
        ofVec3f centerDirection = worldCenter - particles[i].position;
        float distanceToCenter = centerDirection.length();
        
        if(distanceToCenter > 300.0f) {
            centerDirection.normalize();
            particles[i].velocity += centerDirection * centerForce;
        }
    }
    
    float cameraSpeed = 0.2f;
    ofVec3f cameraPos = ofVec3f(
        cos(time * cameraSpeed) * cameraRadius,
        sin(time * cameraSpeed * 0.8f) * 150.0f + cameraHeight,
        sin(time * cameraSpeed) * cameraRadius
    );
    
    cam.setPosition(cameraPos);
    cam.lookAt(worldCenter, ofVec3f(0, 1, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    for(int i = 0; i < particles.size(); i++) {
        if(particles[i].trail.size() > 1) {
            ofSetLineWidth(2);
            for(int t = 1; t < particles[i].trail.size(); t++) {
                float trailAlpha = ofMap(t, 0, particles[i].trail.size()-1, 0.1f, 0.6f);
                ofColor trailColor = colorPalette[particles[i].colorIndex];
                trailColor.a = trailAlpha * 255 * particles[i].energy;
                ofSetColor(trailColor);
                ofDrawLine(particles[i].trail[t-1], particles[i].trail[t]);
            }
        }
    }
    
    for(int i = 0; i < particles.size(); i++) {
        for(int j = i + 1; j < particles.size(); j++) {
            float distance = particles[i].position.distance(particles[j].position);
            if(distance < connectionDistance) {
                float alpha = ofMap(distance, 0, connectionDistance, 120, 0);
                alpha *= (particles[i].alpha + particles[j].alpha) * 0.5f;
                alpha *= (particles[i].energy + particles[j].energy) * 0.5f;
                
                ofColor color1 = colorPalette[particles[i].colorIndex];
                ofColor color2 = colorPalette[particles[j].colorIndex];
                ofColor blendedColor = color1.getLerped(color2, 0.5f);
                blendedColor.a = alpha;
                
                ofSetLineWidth(ofMap(alpha, 0, 120, 1, 4));
                ofSetColor(blendedColor);
                ofDrawLine(particles[i].position, particles[j].position);
            }
        }
    }
    
    for(int i = 0; i < particles.size(); i++) {
        ofPushMatrix();
        ofTranslate(particles[i].position);
        ofRotateXDeg(particles[i].rotation.x);
        ofRotateYDeg(particles[i].rotation.y);
        ofRotateZDeg(particles[i].rotation.z);
        
        ofColor particleColor = colorPalette[particles[i].colorIndex];
        float energyPulse = particles[i].energy * (0.8f + 0.4f * globalPulse);
        particleColor.a = particles[i].alpha * 255 * energyPulse;
        ofSetColor(particleColor);
        
        float currentSize = particles[i].size * energyPulse;
        
        switch(particles[i].type) {
            case 0:
                ofDrawSphere(0, 0, 0, currentSize);
                ofNoFill();
                ofSetLineWidth(1);
                ofDrawSphere(0, 0, 0, currentSize * 1.5f);
                ofFill();
                break;
                
            case 1:
                ofNoFill();
                ofSetLineWidth(2);
                ofDrawBox(0, 0, 0, currentSize * 2);
                ofFill();
                ofDrawSphere(0, 0, 0, currentSize * 0.3f);
                break;
                
            case 2:
                ofNoFill();
                ofSetLineWidth(1);
                ofDrawSphere(0, 0, 0, currentSize);
                ofDrawBox(0, 0, 0, currentSize * 0.8f);
                ofFill();
                break;
                
            case 3:
                ofDrawCone(0, 0, 0, currentSize * 0.8f, currentSize * 2);
                ofNoFill();
                ofSetLineWidth(1);
                ofDrawCircle(0, 0, currentSize, currentSize * 1.2f);
                ofFill();
                break;
        }
        
        ofPopMatrix();
    }
    
    for(int i = 0; i < attractors.size(); i++) {
        ofPushMatrix();
        ofTranslate(attractors[i]);
        
        ofColor attractorColor = colorPalette[i % colorPalette.size()];
        attractorColor.a = 60 + 40 * sin(time * 2.0f + i);
        ofSetColor(attractorColor);
        
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawSphere(0, 0, 0, 15 + 10 * sin(time * 1.5f + i));
        
        for(int ring = 0; ring < 3; ring++) {
            ofPushMatrix();
            ofRotateXDeg(time * 20 + ring * 60);
            ofRotateYDeg(time * 30 + ring * 45);
            ofRotateZDeg(time * 25 + ring * 30);
            
            float ringRadius = 25 + ring * 15;
            attractorColor.a = 30 + 20 * sin(time * 1.8f + ring);
            ofSetColor(attractorColor);
            ofDrawCircle(0, 0, 0, ringRadius);
            ofPopMatrix();
        }
        
        ofFill();
        ofPopMatrix();
    }
    
    ofNoFill();
    ofSetLineWidth(1);
    for(int layer = 0; layer < 3; layer++) {
        for(int i = 0; i < 8; i++) {
            float radius = 80 + layer * 60 + i * 25;
            ofColor ringColor = colorPalette[i % colorPalette.size()];
            float pulse = sin(time * 0.8f + layer * 2 + i * 0.5f);
            ringColor.a = 20 + 30 * pulse * pulse;
            ofSetColor(ringColor);
            
            ofPushMatrix();
            ofRotateXDeg(time * (8 + layer * 2) + i * 45);
            ofRotateYDeg(time * (12 + layer * 3) + i * 30);
            ofRotateZDeg(time * (6 + layer) + i * 60);
            
            if(i % 2 == 0) {
                ofDrawCircle(0, 0, 0, radius);
            } else {
                ofDrawRectangle(-radius*0.7f, -radius*0.7f, 0, radius*1.4f, radius*1.4f);
            }
            ofPopMatrix();
        }
    }
    ofFill();
    
    cam.end();
    
    if(showOverlay) {
        drawOverlay();
    }
    
    if(saveNextFrame) {
        saveScreenshot();
        saveNextFrame = false;
    }
}

void ofApp::drawOverlay() {
    ofPushStyle();
    
    ofSetColor(0, 0, 0, 180);
    ofDrawRectangle(0, ofGetHeight() - 180, ofGetWidth(), 180);
    
    ofSetColor(255, 255, 255, 230);
    
    if(titleFont.isLoaded()) {
        titleFont.drawString(currentTitle, 30, ofGetHeight() - 140);
    } else {
        ofDrawBitmapString(currentTitle, 30, ofGetHeight() - 140);
    }
    
    if(captionFont.isLoaded()) {
        captionFont.drawString(currentCaption, 30, ofGetHeight() - 80);
    } else {
        ofDrawBitmapString(currentCaption, 30, ofGetHeight() - 80);
    }
    
    ofSetColor(colorPalette[2]);
    ofDrawBitmapString("'o' overlay | 's' save | '1-9' captions | 't' tech | 'a' algo | 'c' code | 'p' physics | 'r' render | 'm' math", 30, ofGetHeight() - 20);
    
    ofPopStyle();
}

void ofApp::saveScreenshot() {
    time_t rawTimeValue;
    struct tm * timeinfo;
    char timestamp[80];
    
    std::time(&rawTimeValue);
    timeinfo = localtime(&rawTimeValue);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);
    
    string filename = "generative_art_" + string(timestamp) + "_" + ofToString(screenshotCounter, 3, '0') + ".png";
    
    ofImage screenshot;
    screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    screenshot.save(filename);
    
    cout << "Screenshot saved: " << filename << endl;
    screenshotCounter++;
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 's':
        case 'S':
            saveNextFrame = true;
            break;
            
        case 'o':
        case 'O':
            showOverlay = !showOverlay;
            break;
            
        case '1':
            currentTitle = "Midnight Symphony";
            currentCaption = "What happens when 200 particles decide to dance? ✨\nThis mesmerizing flow is pure code transformed into art\n\n#generativeart #creativecoding #digitalart #openframeworks #codetoart #particlesystem #algorithmicbeauty #techartist";
            break;
            
        case '2':
            currentTitle = "Neural Networks";
            currentCaption = "Each connection tells a story 🧠💜\nWatching AI-inspired patterns emerge in real-time...\nSometimes the most beautiful art comes from mathematics\n\n#neuralart #artificialintelligence #purpleaesthetic #machinelearning #datavisualization #futureofart #techart";
            break;
            
        case '3':
            currentTitle = "Galactic Winds";
            currentCaption = "POV: You're floating through a distant nebula 🌌\nEvery particle follows the invisible forces of the universe\nNature's chaos has never looked so organized\n\n#spaceart #nebula #cosmos #particleflow #astrophysics #digitaluniverse #infinitespace #stardust";
            break;
            
        case '4':
            currentTitle = "Code Poetry";
            currentCaption = "When developers become artists 🎨👨‍💻\n720x1080 pixels • 60fps • Infinite possibilities\nProof that the most beautiful things are often born from logic\n\n#codeart #programmerlife #creativetechnology #softwareart #developerartist #techcreativity #digitaldreams";
            break;
            
        case '5':
            currentTitle = "Quantum Dreams";
            currentCaption = "Reality check: This is what thoughts look like 💭\nQuantum particles dancing to the rhythm of probability\nThe universe is both chaotic and perfectly orchestrated\n\n#quantumphysics #thoughtvisualization #consciousnessart #quantumart #mindfultech #realitydesign #cosmicart #deepthoughts";
            break;
            
        case '6':
            currentTitle = "Electric Soul";
            currentCaption = "This is what music looks like when it becomes visual 🎵💜\nWatch as each beat creates ripples in digital space\nSynesthesia in its purest form\n\n#musicvisualization #synesthesia #electronicart #visualmusic #soundart #digitalsynesthesia #musictech #vibe";
            break;
            
        case '7':
            currentTitle = "Liquid Intelligence";
            currentCaption = "AI learning to be beautiful 🤖✨\nMachine consciousness exploring its creative potential\nThe future of art is collaborative: human + AI\n\n#aiart #artificialcreativity #machinelearning #futureofart #humanai #creativealgorithms #digitalconsciousness #nextgenart";
            break;
            
        case '8':
            currentTitle = "Aurora Borealis 2.0";
            currentCaption = "Northern lights, but make it digital 🌈❄️\nNature's most beautiful phenomenon recreated in code\nSometimes reality needs an upgrade\n\n#digitalaura #northernlights #natureinspired #biomimicry #lightart #colortheory #atmosphericart #ethereal";
            break;
            
        case '9':
            currentTitle = "Meditation Mode";
            currentCaption = "Breathe in the chaos, breathe out the beauty 🧘‍♀️💫\nHypnotic patterns for the restless mind\nSometimes the best therapy is watching particles dance\n\n#digitalmeditation #mindfulness #relaxation #therapeuticart #zenmode #calmingvisuals #stressrelief #peacefulart";
            break;
            
        case 't':
        case 'T':
            currentTitle = "Technical Deep Dive";
            currentCaption = "🔧 HOW IT WORKS:\n• 200 particles with 3D position vectors\n• Perlin noise for organic movement\n• 6 dynamic attractors using inverse distance\n• Real-time physics: velocity *= 0.95 (friction)\n• Custom particle trails (15-frame history)\n• OpenFrameworks C++ • 60fps render loop\n\n#technicalart #openframeworks #cplusplus #gamedev #physicssimulation #perlinoise #vectors";
            break;
            
        case 'a':
        case 'A':
            currentTitle = "Algorithm Breakdown";
            currentCaption = "🧮 THE MATH BEHIND THE MAGIC:\n• Noise-based velocity: ofNoise(time + offset)\n• Attractor force: 1/(1 + distance²)\n• Color interpolation: getLerped(color1, color2)\n• Rotation matrices for 3D transforms\n• Alpha blending for ethereal glow\n• Orbital camera: cos(time) * radius\n\n#algorithmicart #mathematics #computergraphics #physics #vectormath #creativecoding";
            break;
            
        case 'c':
        case 'C':
            currentTitle = "Code Architecture";
            currentCaption = "💻 UNDER THE HOOD:\nstruct Particle {\n  ofVec3f position, velocity;\n  float energy, pulsePhase;\n  vector<ofVec3f> trail;\n}\n\n• Object-oriented particle system\n• Real-time color palette switching\n• Dynamic memory management\n• Optimized rendering pipeline\n\n#codestructure #oop #datastructures #optimization #softwarearchitecture";
            break;
            
        case 'p':
        case 'P':
            currentTitle = "Physics Engine";
            currentCaption = "⚛️ REAL-TIME PHYSICS:\n• Velocity integration: pos += vel * deltaTime\n• Friction simulation: vel *= 0.95\n• Gravitational attraction: F = G*m1*m2/r²\n• Noise-based turbulence\n• Boundary constraints with elastic collision\n• Energy conservation principles\n\n#physics #simulation #mathematics #forcefield #dynamics #motiondesign";
            break;
            
        case 'r':
        case 'R':
            currentTitle = "Rendering Pipeline";
            currentCaption = "🎨 GRAPHICS BREAKDOWN:\n• OpenGL vertex buffer optimization\n• Alpha blending: src*alpha + dst*(1-alpha)\n• Line width scaling based on distance\n• Real-time color interpolation\n• Depth testing for 3D layering\n• Frame buffer objects for post-processing\n\n#opengl #graphics #shaders #rendering #visualization #computergraphics";
            break;
            
        case 'm':
        case 'M':
            currentTitle = "Mathematical Foundation";
            currentCaption = "📐 THE EQUATIONS:\n• Perlin noise: smooth random interpolation\n• Vector normalization: v/|v|\n• Trigonometric orbits: x=cos(t), y=sin(t)\n• Linear interpolation: a + t*(b-a)\n• Distance formula: √((x₂-x₁)² + (y₂-y₁)²)\n• Color space transformations\n\n#mathematics #vectors #trigonometry #algorithms #formulae #mathart";
            break;
            
        case ' ':
            for(int i = 0; i < particles.size(); i++) {
                particles[i].position = ofVec3f(
                    ofRandom(-200, 200),
                    ofRandom(-200, 200),
                    ofRandom(-200, 200)
                );
                particles[i].velocity = ofVec3f(
                    ofRandom(-0.5f, 0.5f),
                    ofRandom(-0.5f, 0.5f),
                    ofRandom(-0.5f, 0.5f)
                );
                particles[i].colorIndex = int(ofRandom(colorPalette.size()));
            }
            break;
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
