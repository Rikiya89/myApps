#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetBackgroundAuto(true);
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    // Optimized font size for Instagram Reels vertical format
    font.load("verdana.ttf", 72, true, true);
    if(!font.isLoaded()){
        font.load(OF_TTF_SANS, 72, true, true);
    }
    
    time = 0;
    animationTime = 0;
    isAnimating = false;
    showTrails = true;
    isInputMode = false;
    animationState = IDLE;
    animationMode = 0;
    mouseInfluenceRadius = 120; // Adjusted for narrower width
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
    fbo.begin();
    ofClear(0, 0, 0, 255);
    fbo.end();
    
    trailFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
    trailFbo.begin();
    ofClear(0, 0, 0, 0);
    trailFbo.end();
    
    // Perfect texts for Instagram Reels cycling
    instagramTexts = {
        "KINETIC",
        "TYPOGRAPHY", 
        "CREATIVE",
        "DIGITAL ART",
        "MOTION",
        "DESIGN"
    };
    currentTextIndex = 0;
    customText = "";
    isUsingCustomText = false;
    currentText = instagramTexts[currentTextIndex];
    inputText = "";
    createTextParticles(currentText);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += ofGetLastFrameTime();
    animationTime += ofGetLastFrameTime();
    
    float mouseDistance = 0;
    ofVec2f mousePos(ofGetMouseX(), ofGetMouseY());
    
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        mouseDistance = particle.position.distance(mousePos);
        float mouseInfluence = 1.0 - ofClamp(mouseDistance / mouseInfluenceRadius, 0, 1);
        mouseInfluence = pow(mouseInfluence, 2);
        
        switch(animationState){
            case FORMING:
                {
                    float t = ofClamp(animationTime / 2.5f, 0, 1);
                    float eased = easeOutElastic(t);
                    
                    particle.position = particle.position.getInterpolated(particle.targetPosition, 0.08f);
                    particle.scale = ofLerp(0.1, 1.0, eased);
                    particle.rotation += particle.rotationSpeed * 0.1;
                    particle.alpha = ofClamp(255 * eased, 0, 255);
                    
                    particle.offset.x = sin(time * 3 + particle.animationPhase) * 2 * mouseInfluence;
                    particle.offset.y = cos(time * 2 + particle.animationPhase * 0.5) * 1 * mouseInfluence;
                }
                break;
                
            case DISPLAYING:
                {
                    switch(animationMode % 3){
                        case 0: waveAnimation(); break;
                        case 1: spiralAnimation(); break;
                        case 2: morphAnimation(); break;
                    }
                    
                    particle.scale = 1.0 + mouseInfluence * 0.3;
                    particle.alpha = 255;
                }
                break;
                
            case SPIRALING:
                spiralAnimation();
                break;
                
            case WAVING:
                waveAnimation();
                break;
                
            case MORPHING:
                morphAnimation();
                break;
                
            case EXPLODING:
                {
                    particle.velocity += ofVec2f(ofRandom(-0.3, 0.3), ofRandom(-0.3, 0.3));
                    particle.velocity *= 1.015;
                    particle.position += particle.velocity;
                    particle.rotation += particle.rotationSpeed;
                    particle.rotationSpeed *= 1.02;
                    particle.scale *= 0.99;
                    particle.alpha = ofClamp(particle.alpha - 2, 0, 255);
                    
                    if(particle.alpha <= 0 || particle.position.x < -100 || particle.position.x > ofGetWidth() + 100 ||
                       particle.position.y < -100 || particle.position.y > ofGetHeight() + 100){
                        particle.isActive = false;
                    }
                }
                break;
                
            case REFORMING:
                {
                    float t = ofClamp(animationTime / 2.0f, 0, 1);
                    float eased = easeInOutBack(t);
                    
                    ofVec2f force = particle.targetPosition - particle.position;
                    force *= 0.12 * eased;
                    particle.velocity += force;
                    particle.velocity *= 0.82;
                    particle.position += particle.velocity;
                    
                    particle.rotation *= 0.95;
                    particle.scale = ofLerp(particle.scale, 1.0, 0.05);
                    particle.alpha = ofClamp(particle.alpha + 6, 0, 255);
                }
                break;
        }
    }
    
    if(animationState == FORMING && animationTime > 3.0){
        animationState = DISPLAYING;
        animationTime = 0;
    }
    else if(animationState == DISPLAYING && animationTime > 6.0){
        animationMode = (animationMode + 1) % 3;
        animationState = EXPLODING;
        animationTime = 0;
        explodeText();
    }
    else if(animationState == EXPLODING && animationTime > 2.5){
        animationState = REFORMING;
        animationTime = 0;
        reformText();
    }
    else if(animationState == REFORMING && animationTime > 2.5){
        if(isUsingCustomText){
            // Keep using custom text, just restart the animation cycle
            animationState = DISPLAYING;
            animationTime = 0;
        } else {
            // Cycle to next default Instagram text
            currentTextIndex = (currentTextIndex + 1) % instagramTexts.size();
            currentText = instagramTexts[currentTextIndex];
            createTextParticles(currentText);
            animationState = FORMING;
            animationTime = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(showTrails){
        trailFbo.begin();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(0, 0, 0, 8);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for(const auto& particle : textParticles){
            if(!particle.isActive) continue;
            
            ofPushMatrix();
            ofTranslate(particle.position.x + particle.offset.x, particle.position.y + particle.offset.y);
            ofRotateDeg(particle.rotation);
            ofScale(particle.scale, particle.scale);
            
            ofSetColor(255, particle.alpha * 0.7);
            font.drawString(ofToString(particle.character), 
                          -font.stringWidth(ofToString(particle.character))/2, 
                          font.stringHeight(ofToString(particle.character))/2);
            ofPopMatrix();
        }
        ofDisableBlendMode();
        trailFbo.end();
        
        ofSetColor(255);
        trailFbo.draw(0, 0);
    } else {
        ofBackground(0);
    }
    
    for(const auto& particle : textParticles){
        if(!particle.isActive) continue;
        
        ofPushMatrix();
        ofTranslate(particle.position.x + particle.offset.x, particle.position.y + particle.offset.y);
        ofRotateDeg(particle.rotation);
        ofScale(particle.scale, particle.scale);
        
        float glowAlpha = particle.alpha * 0.3;
        for(int i = 0; i < 3; i++){
            ofSetColor(255, glowAlpha / (i + 1));
            font.drawString(ofToString(particle.character), 
                          -font.stringWidth(ofToString(particle.character))/2 + ofRandom(-i, i), 
                          font.stringHeight(ofToString(particle.character))/2 + ofRandom(-i, i));
        }
        
        ofSetColor(255, particle.alpha);
        font.drawString(ofToString(particle.character), 
                      -font.stringWidth(ofToString(particle.character))/2, 
                      font.stringHeight(ofToString(particle.character))/2);
        ofPopMatrix();
    }
    
    // Minimal UI for Instagram Reels - only show essential info
    ofSetColor(255, 150); // More subtle UI
    if(isInputMode){
        ofDrawBitmapString("Enter text: " + inputText + "_", 10, ofGetHeight() - 60);
        ofDrawBitmapString("ENTER: animate | ESC: cancel", 10, ofGetHeight() - 40);
    } else {
        // Clean minimal UI positioned at bottom
        ofDrawBitmapString("@yourhandle", 10, ofGetHeight() - 100);
        
        // Show current text mode
        if(isUsingCustomText){
            ofSetColor(255, 255, 100, 150); // Yellow tint for custom mode
            ofDrawBitmapString("CUSTOM: " + customText, 10, ofGetHeight() - 80);
            ofSetColor(255, 150);
        } else {
            ofDrawBitmapString("AUTO CYCLING: " + currentText, 10, ofGetHeight() - 80);
        }
        
        ofDrawBitmapString("SPACE: new text | D: default mode", 10, ofGetHeight() - 60);
        ofDrawBitmapString("T: trails | 1-3: animation modes", 10, ofGetHeight() - 40);
        
        // Show current animation mode at top
        string modeText = "";
        switch(animationState){
            case FORMING: modeText = "FORMING"; break;
            case DISPLAYING: 
                switch(animationMode % 3){
                    case 0: modeText = "WAVE MODE"; break;
                    case 1: modeText = "SPIRAL MODE"; break;
                    case 2: modeText = "MORPH MODE"; break;
                }
                break;
            case EXPLODING: modeText = "EXPLODING"; break;
            case REFORMING: modeText = "REFORMING"; break;
            case SPIRALING: modeText = "SPIRAL MODE"; break;
            case WAVING: modeText = "WAVE MODE"; break;
            case MORPHING: modeText = "MORPH MODE"; break;
        }
        if(!modeText.empty()){
            ofSetColor(255, 200);
            float textWidth = modeText.length() * 8;
            ofDrawBitmapString(modeText, (ofGetWidth() - textWidth) / 2, 30);
        }
    }
}

//--------------------------------------------------------------
void ofApp::createTextParticles(string text){
    textParticles.clear();
    
    float totalWidth = font.stringWidth(text);
    float startX = (ofGetWidth() - totalWidth) / 2;
    float centerY = ofGetHeight() / 2;
    
    float charX = startX;
    int charIndex = 0;
    
    for(int i = 0; i < text.length(); i++){
        if(text[i] == ' '){
            charX += font.stringWidth(" ");
            continue;
        }
        
        TextParticle particle;
        particle.character = text[i];
        particle.targetPosition.set(charX, centerY);
        particle.originalPosition = particle.targetPosition;
        particle.characterIndex = charIndex++;
        
        particle.position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        particle.velocity.set(0, 0);
        particle.alpha = 0;
        particle.size = 64;
        particle.scale = 0.1;
        particle.targetScale = 1.0;
        particle.rotation = ofRandom(-180, 180);
        particle.rotationSpeed = ofRandom(-2, 2);
        particle.isActive = true;
        particle.animationPhase = ofRandom(TWO_PI);
        particle.life = 255;
        particle.maxLife = 255;
        particle.offset.set(0, 0);
        particle.springForce = 0.1;
        particle.damping = 0.9;
        
        textParticles.push_back(particle);
        
        charX += font.stringWidth(ofToString(text[i]));
    }
    
    animationState = FORMING;
    animationTime = 0;
}

//--------------------------------------------------------------
void ofApp::explodeText(){
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        ofVec2f center(ofGetWidth()/2, ofGetHeight()/2);
        ofVec2f direction = particle.position - center;
        direction.normalize();
        
        particle.velocity = direction * ofRandom(4, 12);
        particle.velocity += ofVec2f(ofRandom(-3, 3), ofRandom(-3, 3));
        particle.rotationSpeed = ofRandom(-5, 5);
    }
}

//--------------------------------------------------------------
void ofApp::reformText(){
    for(auto &particle : textParticles){
        if(!particle.isActive){
            particle.isActive = true;
            particle.position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
            particle.alpha = 0;
        }
        particle.velocity.set(0, 0);
        particle.rotation = ofRandom(-45, 45);
        particle.scale = ofRandom(0.5, 1.5);
    }
}

//--------------------------------------------------------------
void ofApp::spiralAnimation(){
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        // Adjusted for vertical format - smaller horizontal movement
        float spiralRadius = 25 + sin(time * 0.5 + particle.characterIndex * 0.2) * 12;
        float spiralAngle = time * 1.5 + particle.characterIndex * 0.3;
        
        particle.offset.x = cos(spiralAngle) * spiralRadius * 0.8; // Reduced horizontal movement
        particle.offset.y = sin(spiralAngle) * spiralRadius;
        particle.rotation = spiralAngle * 8;
        particle.scale = 1.0 + sin(time * 2 + particle.animationPhase) * 0.25;
    }
}

//--------------------------------------------------------------
void ofApp::waveAnimation(){
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        // Optimized for vertical Instagram format
        float waveHeight = 50; // Increased for better visibility
        float waveFreq = 0.015; // Adjusted frequency
        float waveSpeed = 2.5;
        
        particle.offset.x = sin(time * waveSpeed + particle.characterIndex * 0.5) * 8; // Reduced horizontal
        particle.offset.y = sin(particle.targetPosition.x * waveFreq + time * waveSpeed) * waveHeight;
        particle.rotation = sin(time * 2 + particle.animationPhase) * 6;
        particle.scale = 1.0 + cos(time * 3 + particle.characterIndex * 0.1) * 0.2;
    }
}

//--------------------------------------------------------------
void ofApp::morphAnimation(){
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        // Optimized morph for vertical format
        float morphIntensityX = 35; // Reduced horizontal morphing
        float morphIntensityY = 60; // Enhanced vertical morphing
        float noiseScale = 0.008;
        
        float noiseX = ofNoise(particle.targetPosition.x * noiseScale, time * 0.4) - 0.5;
        float noiseY = ofNoise(particle.targetPosition.y * noiseScale + 1000, time * 0.4) - 0.5;
        
        particle.offset.x = noiseX * morphIntensityX;
        particle.offset.y = noiseY * morphIntensityY;
        particle.rotation = noiseX * 15;
        particle.scale = 1.0 + noiseY * 0.35;
    }
}

//--------------------------------------------------------------
float ofApp::easeInOutCubic(float t){
    return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}

//--------------------------------------------------------------
float ofApp::easeOutElastic(float t){
    const float c4 = (2 * PI) / 3;
    return t == 0 ? 0 : t == 1 ? 1 : pow(2, -10 * t) * sin((t * 10 - 0.75) * c4) + 1;
}

//--------------------------------------------------------------
float ofApp::easeInOutBack(float t){
    const float c1 = 1.70158;
    const float c2 = c1 * 1.525;
    return t < 0.5
        ? (pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
        : (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(isInputMode){
        if(key == OF_KEY_RETURN){
            if(!inputText.empty()){
                customText = inputText;
                currentText = customText;
                isUsingCustomText = true;
                createTextParticles(currentText);
                inputText = "";
                isInputMode = false;
            }
        }
        else if(key == OF_KEY_ESC){
            inputText = "";
            isInputMode = false;
        }
        else if(key == OF_KEY_BACKSPACE){
            if(!inputText.empty()){
                inputText.pop_back();
            }
        }
        else if(key >= 32 && key <= 126){
            inputText += char(key);
        }
    }
    else {
        if(key == 't' || key == 'T'){
            showTrails = !showTrails;
            if(!showTrails){
                fbo.begin();
                ofClear(0, 0, 0, 255);
                fbo.end();
            }
        }
        
        if(key == 'r' || key == 'R'){
            animationState = REFORMING;
            animationTime = 0;
            reformText();
        }
        
        if(key == 'e' || key == 'E'){
            animationState = EXPLODING;
            animationTime = 0;
            explodeText();
        }
        
        if(key == ' '){
            isInputMode = true;
            inputText = "";
        }
        
        if(key == '1'){
            animationMode = 0;
            animationState = WAVING;
            animationTime = 0;
        }
        
        if(key == '2'){
            animationMode = 1;
            animationState = SPIRALING;
            animationTime = 0;
        }
        
        if(key == '3'){
            animationMode = 2;
            animationState = MORPHING;
            animationTime = 0;
        }
        
        if(key == 'd' || key == 'D'){
            // Return to default cycling mode
            isUsingCustomText = false;
            customText = "";
            currentText = instagramTexts[currentTextIndex];
            createTextParticles(currentText);
        }
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
    for(auto &particle : textParticles){
        if(!particle.isActive) continue;
        
        float distance = ofDist(x, y, particle.position.x, particle.position.y);
        if(distance < 100){
            ofVec2f force = ofVec2f(x, y) - particle.position;
            force.normalize();
            particle.velocity += force * 0.5;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if(button == 0){
        animationState = EXPLODING;
        animationTime = 0;
        explodeText();
    }
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
