#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);  // pure black background
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnableSmoothing();
    ofSetCircleResolution(64);

    // Create nodes
    for (int i = 0; i < numNodes; i++) {
        Node node;
        node.position = glm::vec3(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
        node.velocity = glm::vec3(ofRandom(-0.7, 0.7), ofRandom(-0.7, 0.7), ofRandom(-0.7, 0.7));
        nodes.push_back(node);
    }

    // Create star field
    for (int i = 0; i < 300; i++) {
        stars.push_back(glm::vec3(ofRandom(-1000, 1000), ofRandom(-1000, 1000), ofRandom(-1000, 1000)));
    }
}

void ofApp::update(){
    time = ofGetElapsedTimef();

    for (auto &node : nodes) {
        node.position += node.velocity;

        if (abs(node.position.x) > 300) node.velocity.x *= -1;
        if (abs(node.position.y) > 300) node.velocity.y *= -1;
        if (abs(node.position.z) > 300) node.velocity.z *= -1;
    }
}

void ofApp::draw(){
    // Orbiting camera
    float camX = cos(time * orbitSpeed) * orbitRadius;
    float camZ = sin(time * orbitSpeed) * orbitRadius;
    ofVec3f camPos = ofVec3f(camX, 0, camZ);
    ofCamera cam;
    cam.setPosition(camPos);
    cam.lookAt(glm::vec3(0, 0, 0));
    cam.begin();

    // Starfield (brighter and bigger)
    for (auto &s : stars) {
        ofSetColor(100 + 100 * sin(time + s.x), 180, 255, 120);  // brighter stars
        ofDrawSphere(s, 2.0);
    }

    float pulse = sin(time * 2.0) * 0.5 + 0.5;

    // Connections with aura and thicker glow
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            float dist = glm::distance(nodes[i].position, nodes[j].position);
            if (dist < connectDistance) {
                float alpha = ofMap(dist, 0, connectDistance, 255, 0) * pulse;
                float thickness = ofMap(dist, 0, connectDistance, 2.2, 0.3);

                ofColor color = ofColor::fromHsb(fmod(i * 5 + time * 10, 255), 255, 255);
                color.a = alpha;

                glm::vec3 start = nodes[i].position;
                glm::vec3 end = nodes[j].position;
                glm::vec3 control = (start + end) / 2 + glm::vec3(0, 30 * sin(time + i), 0);

                // Outer glow aura
                ofPath aura;
                aura.setFilled(false);
                aura.setStrokeColor(ofColor(color.r, color.g, color.b, alpha * 0.5));
                aura.setStrokeWidth(thickness + 3.0);
                aura.moveTo(start);
                aura.bezierTo(control, control, end);
                aura.draw();

                // Main curve
                ofPath bezier;
                bezier.setFilled(false);
                bezier.setStrokeColor(color);
                bezier.setStrokeWidth(thickness + 0.5);
                bezier.moveTo(start);
                bezier.bezierTo(control, control, end);
                bezier.draw();
            }
        }
    }

    // Nodes with aura + glow + core
    for (int i = 0; i < nodes.size(); i++) {
        float hue = fmod(i * 10 + time * 40, 255);
        ofColor glowColor = ofColor::fromHsb(hue, 255, 255);
        glowColor.a = 80;

        ofColor auraColor = ofColor::fromHsb(hue, 180, 255);
        auraColor.a = 50;

        ofSetColor(auraColor);  // outer aura
        ofDrawSphere(nodes[i].position, 12.0);

        ofSetColor(glowColor);  // mid glow
        ofDrawSphere(nodes[i].position, 8.5);

        ofSetColor(255);  // core
        ofDrawSphere(nodes[i].position, 2.5);
    }

    cam.end();
}
