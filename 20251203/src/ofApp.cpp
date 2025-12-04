#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofSetCircleResolution(64);

    palette.push_back(ofColor::fromHex(0x362d78));
    palette.push_back(ofColor::fromHex(0x523fa3));
    palette.push_back(ofColor::fromHex(0x916ccc));
    palette.push_back(ofColor::fromHex(0xbda1e5));
    palette.push_back(ofColor::fromHex(0xc8c0e9));
    palette.push_back(ofColor::fromHex(0x84bae7));
    palette.push_back(ofColor::fromHex(0x516ad4));
    palette.push_back(ofColor::fromHex(0x333f87));
    palette.push_back(ofColor::fromHex(0x293039));
    palette.push_back(ofColor::fromHex(0x283631));

    int numParticles = 2000;
    for(int i = 0; i < numParticles; i++){
        Particle p;

        float layer = i / (float)numParticles;
        float theta = ofRandom(TWO_PI);
        float phi = ofRandom(TWO_PI);
        float radius = ofRandom(80, 460);

        p.position.x = radius * sin(theta) * cos(phi);
        p.position.y = radius * sin(theta) * sin(phi);
        p.position.z = radius * cos(theta);

        p.velocity = glm::vec3(ofRandom(-0.2, 0.2), ofRandom(-0.2, 0.2), ofRandom(-0.2, 0.2));

        int colorIndex = (int)(layer * palette.size()) % palette.size();
        p.color = palette[colorIndex];

        p.size = ofRandom(0.5, 6);
        p.phase = ofRandom(TWO_PI);
        p.speed = ofRandom(0.2, 2.2);
        p.orbitRadius = ofRandom(40, 320);
        p.orbitAngle = ofRandom(TWO_PI);
        p.orbitSpeed = ofRandom(0.15, 0.9);
        p.wobblePhase = ofRandom(TWO_PI);
        p.rotationSpeed = ofRandom(0.8, 2.5);
        p.energy = ofRandom(0.5, 1.0);

        particles.push_back(p);
    }

    time = 0;
    ofBackground(20, 18, 25);
}

//--------------------------------------------------------------
void ofApp::update(){
    time += 0.012;

    for(int i = 0; i < particles.size(); i++){
        particles[i].orbitAngle += particles[i].orbitSpeed * 0.02;
        particles[i].wobblePhase += particles[i].rotationSpeed * 0.015;

        float wave1 = sin(time * particles[i].speed + particles[i].phase);
        float wave2 = cos(time * particles[i].speed * 0.7 + particles[i].phase);
        float wave3 = sin(time * 0.5 + particles[i].phase * 2);

        float orbitX = cos(particles[i].orbitAngle) * particles[i].orbitRadius * 0.3;
        float orbitY = sin(particles[i].orbitAngle) * particles[i].orbitRadius * 0.3;
        float orbitZ = sin(particles[i].orbitAngle * 2) * particles[i].orbitRadius * 0.2;

        float spiralAngle = time * 0.25 + i * 0.008;
        float spiralRadius = sin(time * 0.15 + i * 0.015) * 25;

        float wobbleX = cos(particles[i].wobblePhase) * particles[i].energy * 15;
        float wobbleY = sin(particles[i].wobblePhase * 1.3) * particles[i].energy * 15;
        float wobbleZ = sin(particles[i].wobblePhase * 0.7) * particles[i].energy * 15;

        particles[i].position.x += particles[i].velocity.x + wave1 * 0.6 + cos(spiralAngle) * spiralRadius * 0.08 + orbitX * 0.01 + wobbleX * 0.02;
        particles[i].position.y += particles[i].velocity.y + wave2 * 0.6 + sin(spiralAngle) * spiralRadius * 0.08 + orbitY * 0.01 + wobbleY * 0.02;
        particles[i].position.z += particles[i].velocity.z + wave3 * 0.6 + orbitZ * 0.01 + wobbleZ * 0.02;

        glm::vec3 currentPos = particles[i].position;
        float dist = glm::length(currentPos);

        float targetRadius = 300 + sin(time * 0.3 + particles[i].phase) * 80 + cos(time * 0.2 + particles[i].wobblePhase) * 40;
        glm::vec3 toTarget = glm::normalize(currentPos) * targetRadius - currentPos;
        particles[i].velocity += toTarget * 0.008;

        for(int j = i + 1; j < particles.size(); j++){
            if(j % 50 == 0){
                float particleDist = glm::distance(particles[i].position, particles[j].position);
                if(particleDist < 60 && particleDist > 0.1){
                    glm::vec3 force = glm::normalize(particles[i].position - particles[j].position);
                    float strength = (60 - particleDist) / 60.0 * 0.15;
                    particles[i].velocity += force * strength;
                    particles[j].velocity -= force * strength;
                }
            }
        }

        particles[i].velocity *= 0.985;

        float maxDist = 480;
        if(dist > maxDist){
            particles[i].position = glm::normalize(particles[i].position) * maxDist;
            particles[i].velocity *= -0.85;
        }

        if(dist < 80){
            particles[i].position = glm::normalize(particles[i].position) * 80;
            particles[i].velocity *= -0.5;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();

    ofPushMatrix();
    ofRotateDeg(time * 5, 0, 1, 0);
    ofRotateDeg(sin(time * 0.18) * 25, 1, 0, 0);
    ofRotateDeg(cos(time * 0.12) * 12, 0, 0, 1);

    ofEnableBlendMode(OF_BLENDMODE_ADD);

    for(int layer = 0; layer < 16; layer++){
        float layerRadius = 100 + layer * 35;
        int numPoints = 120;
        float thickness = ofMap(layer, 0, 15, 3.5, 0.3);
        ofSetLineWidth(thickness);

        float hue = ofMap(layer, 0, 15, 0, palette.size() - 1);
        int colorIndex = (int)hue % palette.size();
        ofColor layerColor = palette[colorIndex];

        float layerAlpha = ofMap(sin(time * 0.4 + layer * 0.25), -1, 1, 30, 110);
        layerColor.a = layerAlpha;
        ofSetColor(layerColor);

        ofNoFill();
        ofBeginShape();
        for(int i = 0; i <= numPoints; i++){
            float angle = (i / float(numPoints)) * TWO_PI;
            float wobble = sin(time * 0.5 + angle * 5 + layer * 0.4) * 30;
            float wobble2 = cos(time * 0.35 + angle * 3) * 18;
            float wobble3 = sin(time * 0.25 + angle * 6 + layer) * 12;
            float wobble4 = cos(time * 0.45 + angle * 4 + layer * 0.3) * 8;

            float effectiveRadius = layerRadius + wobble + wobble2 + wobble3 + wobble4;
            float rotationOffset = sin(time * 0.15 + layer * 0.2) * 0.8;
            float x = cos(angle + time * 0.25 + layer * 0.1 + rotationOffset) * effectiveRadius;
            float y = sin(angle + time * 0.25 + layer * 0.1 + rotationOffset) * effectiveRadius;
            float z = sin(time * 0.4 + angle * 6) * 100 + cos(time * 0.2 + layer) * 50 + sin(angle * 8) * 20 + cos(time * 0.3 + angle * 4) * 15;
            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    ofFill();
    for(int i = 0; i < particles.size(); i++){
        float pulsate = ofMap(sin(time * particles[i].speed * 2.0 + particles[i].phase), -1, 1, 0.4, 1.8);
        float dist = glm::length(particles[i].position);
        float alpha = ofMap(dist, 70, 490, 300, 50);
        float brightness = ofMap(sin(time * 1.2 + particles[i].phase), -1, 1, 0.5, 1.5);

        ofColor c = particles[i].color;
        c.a = alpha * brightness;

        float distanceGlow = ofMap(dist, 90, 420, 1.6, 0.7);
        c.setBrightness(c.getBrightness() * distanceGlow);

        float colorShift = ofMap(sin(time * 0.3 + particles[i].phase), -1, 1, 0.9, 1.1);
        c.setSaturation(c.getSaturation() * colorShift);

        ofSetColor(c);

        ofPushMatrix();
        ofTranslate(particles[i].position);

        float finalSize = particles[i].size * pulsate;
        ofDrawSphere(0, 0, 0, finalSize);

        if(particles[i].size > 3){
            ofColor glowColor = c;
            glowColor.a *= 0.25;
            ofSetColor(glowColor);
            ofDrawSphere(0, 0, 0, finalSize * 2.2);

            if(particles[i].size > 4.5){
                glowColor.a *= 0.4;
                ofSetColor(glowColor);
                ofDrawSphere(0, 0, 0, finalSize * 3.5);
            }
        }

        ofPopMatrix();

        if(i % 2 == 0 && i < particles.size() - 2){
            int next = (i + 2) % particles.size();
            float lineDist = glm::distance(particles[i].position, particles[next].position);
            if(lineDist < 140){
                ofColor lineColor1 = particles[i].color;
                ofColor lineColor2 = particles[next].color;
                float blend = ofMap(sin(time + i * 0.1), -1, 1, 0.3, 0.7);
                ofColor lineColor = lineColor1.getLerped(lineColor2, blend);

                lineColor.a = ofMap(lineDist, 0, 140, 200, 5);
                ofSetColor(lineColor);
                ofSetLineWidth(ofMap(lineDist, 0, 140, 3.5, 0.3));
                ofDrawLine(particles[i].position, particles[next].position);
            }
        }

        if(i % 3 == 0 && particles[i].size > 2.5){
            for(int k = i + 1; k < particles.size(); k += 20){
                float connectionDist = glm::distance(particles[i].position, particles[k].position);
                if(connectionDist < 100){
                    ofColor connColor = particles[i].color.getLerped(particles[k].color, 0.5);
                    connColor.a = ofMap(connectionDist, 0, 100, 120, 2);
                    ofSetColor(connColor);
                    ofSetLineWidth(ofMap(connectionDist, 0, 100, 2.0, 0.15));

                    glm::vec3 mid = (particles[i].position + particles[k].position) * 0.5f;
                    glm::vec3 perpOffset = glm::vec3(
                        sin(time * 0.5 + i * 0.05) * 8,
                        cos(time * 0.5 + k * 0.05) * 8,
                        sin(time * 0.3 + i * 0.03 + k * 0.03) * 8
                    );
                    mid += perpOffset;

                    ofDrawLine(particles[i].position, mid);
                    ofDrawLine(mid, particles[k].position);
                }
            }
        }
    }

    for(int trail = 0; trail < particles.size(); trail += 12){
        if(particles[trail].size > 4){
            int trailLength = 7;
            for(int t = 1; t < trailLength; t++){
                int prevIndex = (trail - t * 12 + particles.size()) % particles.size();
                float trailAlpha = ofMap(t, 0, trailLength, 100, 3);
                ofColor trailColor = particles[trail].color;
                trailColor.a = trailAlpha;
                ofSetColor(trailColor);
                ofSetLineWidth(ofMap(t, 0, trailLength, 1.2, 0.2));
                ofDrawLine(particles[trail].position, particles[prevIndex].position);
            }
        }
    }

    for(int i = 0; i < particles.size(); i += 25){
        if(particles[i].energy > 0.7){
            float glowSize = particles[i].size * 4.5;
            float glowAlpha = ofMap(sin(time * 1.5 + particles[i].phase), -1, 1, 10, 35);
            ofColor glowColor = particles[i].color;
            glowColor.a = glowAlpha;
            ofSetColor(glowColor);

            ofPushMatrix();
            ofTranslate(particles[i].position);
            ofDrawSphere(0, 0, 0, glowSize);
            ofPopMatrix();
        }
    }

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    for(int ring = 0; ring < 6; ring++){
        float ringRadius = 120 + ring * 100;
        float ringAlpha = ofMap(sin(time * 0.3 + ring), -1, 1, 18, 45);
        ofSetColor(palette[(ring + 6) % palette.size()], ringAlpha);
        ofNoFill();
        ofSetLineWidth(ofMap(ring, 0, 5, 1.0, 0.3));

        ofPushMatrix();
        ofRotateDeg(sin(time * 0.2 + ring) * 20, 1, 0, 0);
        ofRotateDeg(cos(time * 0.15 + ring) * 15, 0, 1, 0);
        ofDrawSphere(0, 0, 0, ringRadius);
        ofPopMatrix();
    }

    for(int spiral = 0; spiral < 3; spiral++){
        ofNoFill();
        ofBeginShape();
        int numSpiralPoints = 200;
        for(int i = 0; i < numSpiralPoints; i++){
            float t = (i / float(numSpiralPoints)) * TWO_PI * 4;
            float spiralRadius = 80 + i * 1.5;
            float spiralHeight = sin(t + time * 0.4 + spiral * TWO_PI / 3) * 120;

            float x = cos(t + time * 0.3 + spiral * TWO_PI / 3) * spiralRadius;
            float y = sin(t + time * 0.3 + spiral * TWO_PI / 3) * spiralRadius;
            float z = spiralHeight;

            float colorProgress = i / float(numSpiralPoints);
            int spiralColorIndex = (int)(colorProgress * palette.size()) % palette.size();
            ofColor spiralColor = palette[spiralColorIndex];
            spiralColor.a = ofMap(i, 0, numSpiralPoints, 60, 15);
            ofSetColor(spiralColor);
            ofSetLineWidth(ofMap(i, 0, numSpiralPoints, 2.5, 0.4));

            ofVertex(x, y, z);
        }
        ofEndShape();
    }

    ofPopMatrix();

    cam.end();
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
