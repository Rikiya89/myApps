#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(720, 1080);
    ofSetFrameRate(60);
    ofBackground(10);
    ofSeedRandom(0);

    // allocate FBO at 720×1080
    fbo.allocate(720, 1080, GL_RGBA);
    fbo.begin();
      ofClear(0,0,0, 255);
    fbo.end();
    // pastel palette for modes 0–2
    palette = {
        ofColor(239, 71, 111),
        ofColor(255, 209, 102),
        ofColor(6, 214, 160),
        ofColor(17, 138, 178),
        ofColor(7,  59,  76)
    };

    // sacred-geometry palette for mode 3
    sacredPalette = {
        ofColor(255,  82,  82),
        ofColor(255, 167,  38),
        ofColor(255,235,  59),
        ofColor(102,187, 106),
        ofColor( 33,150,243),
        ofColor(156, 39,176),
        ofColor(255, 64,129)
    };

    // prepare particles for mode 1
    for(int i = 0; i < 400; i++) {
        Particle p;
        p.pos.set(ofRandomWidth(), ofRandomHeight());
        p.vel.set(ofRandom(-1,1), ofRandom(-1,1));
        p.color = ofColor::fromHsb(ofRandom(255), 180, 255);
        particles.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimef();

    // update particles (mode 1)
    for(auto &p : particles) {
        ofVec2f accel;
        accel.x = ofSignedNoise(p.pos.y * 0.004, time * 0.2);
        accel.y = ofSignedNoise(p.pos.x * 0.004, time * 0.2);
        p.vel += accel * 0.15;
        p.vel.limit(2.5);
        p.pos += p.vel;

        // wrap
        if(p.pos.x < 0) p.pos.x += ofGetWidth();
        if(p.pos.x > ofGetWidth()) p.pos.x -= ofGetWidth();
        if(p.pos.y < 0) p.pos.y += ofGetHeight();
        if(p.pos.y > ofGetHeight()) p.pos.y -= ofGetHeight();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw all modes into FBO, with alpha fade for trails
    fbo.begin();
      ofSetColor(10, 10, 10, 30);
      ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
      ofEnableBlendMode(OF_BLENDMODE_ADD);

      if(mode == 0) {
        // flowing noise lines
        ofNoFill();
        ofSetLineWidth(2);
        for(int i = 0; i < 25; i++) {
            float yOff = ofMap(i, 0, 24, 0, ofGetHeight());
            ofPath path;
            for(int x = 0; x <= ofGetWidth(); x += 6) {
                float n = ofSignedNoise(x * 0.004, yOff * 0.004, time * 0.25);
                float y = yOff + n * 120;
                if(x==0) path.moveTo(x,y);
                else       path.lineTo(x,y);
            }
            path.setStrokeColor( ofColor::fromHsb(fmod(i*10 + time*20, 255), 200, 230) );
            path.setStrokeWidth(1.2);
            path.draw();
        }

      } else if(mode == 1) {
        // glowing nebula
        for(auto &p : particles) {
            float speed = p.vel.length();
            float r = ofMap(speed, 0, 2.5, 2, 8);
            // draw halo
            for(int k = 3; k >= 1; k--) {
                float alpha = ofMap(k,3,1,20,120);
                ofSetColor(p.color, alpha);
                ofDrawCircle(p.pos, r * (k*0.6));
            }
        }

      } else if(mode == 2) {
        // pulsing pastel squares
        int cols = 12, rows = 10;
        float w = ofGetWidth() / float(cols);
        float h = ofGetHeight() / float(rows);
        for(int y=0; y<rows; y++){
          for(int x=0; x<cols; x++){
            float cx = x*w + w*0.5;
            float cy = y*h + h*0.5;
            float ang = time*30 + (x+y)*10;
            float sz  = w*0.4 * (0.6 + 0.4*sin(ofDegToRad(ang)));
            ofPushMatrix();
              ofTranslate(cx, cy);
              ofRotateDeg(ang);
              ofSetColor(palette[(x+y)%palette.size()], 180);
              ofDrawRectangle(-sz/2, -sz/2, sz, sz);
            ofPopMatrix();
          }
        }

      } else if(mode == 3) {
        // sacred geometry rings
        float cx = ofGetWidth()*0.5;
        float cy = ofGetHeight()*0.5;
        ofNoFill();
        for(int i = 0; i < (int)sacredPalette.size(); i++){
          float freq = 0.2 + i*0.05;
          float rad  = (min(ofGetWidth(),ofGetHeight())*0.4) * (0.5 + 0.5*sin(time*freq + i));
          ofSetColor(sacredPalette[i], 150);
          ofPushMatrix();
            ofTranslate(cx, cy);
            ofRotateDeg(time*10*(i%2==0?1:-1));
            ofDrawCircle(0, 0, abs(rad)+50);
          ofPopMatrix();
        }
      }

    ofDisableBlendMode();
    fbo.end();

    // finally draw FBO to screen
    ofSetColor(255);
    fbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        mode = (mode + 1) % 4;
        // clear trails on mode change
        fbo.begin();
          ofClear(0,0,0,255);
        fbo.end();
    }
}
