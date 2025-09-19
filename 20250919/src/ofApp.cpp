#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    gridCols = 24;
    gridRows = 36;
    cellWidth = ofGetWidth() / gridCols;
    cellHeight = ofGetHeight() / gridRows;
    
    time = 0;
    showGrid = false;
    patternType = 1;
    maxPatterns = 6;
    
    noiseOffset.resize(gridCols);
    cellStates.resize(gridCols);
    for(int i = 0; i < gridCols; i++){
        noiseOffset[i].resize(gridRows);
        cellStates[i].resize(gridRows);
        for(int j = 0; j < gridRows; j++){
            noiseOffset[i][j] = ofRandom(1000);
            cellStates[i][j] = false;
        }
    }
}

void ofApp::update(){
    time += 0.01;
    
    for(int i = 0; i < gridCols; i++){
        for(int j = 0; j < gridRows; j++){
            float noise = ofNoise(i * 0.1, j * 0.1, time + noiseOffset[i][j]);
            cellStates[i][j] = noise > 0.5;
        }
    }
}

void ofApp::draw(){
    if(patternType == 1){
        ofSetColor(0, 8);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    ofSetColor(255);
    
    if(patternType == 0){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth;
                float y = j * cellHeight;
                
                if(cellStates[i][j]){
                    ofDrawRectangle(x, y, cellWidth, cellHeight);
                }
            }
        }
    }
    else if(patternType == 1){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth + cellWidth/2;
                float y = j * cellHeight + cellHeight/2;
                
                float noise1 = ofNoise(i * 0.08, j * 0.08, time * 0.5 + noiseOffset[i][j]);
                float noise2 = ofNoise(i * 0.15, j * 0.15, time * 0.3 + noiseOffset[i][j] + 100);
                float noise3 = ofNoise(i * 0.05, j * 0.05, time * 0.8 + noiseOffset[i][j] + 200);
                
                float baseRadius = min(cellWidth, cellHeight) * 0.35;
                float radius = baseRadius * (0.3 + noise1 * 0.7);
                
                if(noise2 > 0.4){
                    float alpha = ofMap(noise3, 0, 1, 0.6, 1.0);
                    ofSetColor(255, 255 * alpha);
                    
                    ofPushMatrix();
                    ofTranslate(x, y);
                    
                    float wobbleX = sin(time * 2.0 + i * 0.3) * 2.0;
                    float wobbleY = cos(time * 1.5 + j * 0.4) * 2.0;
                    ofTranslate(wobbleX, wobbleY);
                    
                    if(noise1 > 0.7){
                        ofNoFill();
                        ofSetLineWidth(2.0);
                        ofDrawCircle(0, 0, radius);
                        ofDrawCircle(0, 0, radius * 0.6);
                    } else {
                        ofFill();
                        ofDrawCircle(0, 0, radius);
                        
                        if(noise3 > 0.8){
                            ofSetColor(0);
                            ofDrawCircle(0, 0, radius * 0.4);
                        }
                    }
                    
                    ofPopMatrix();
                }
            }
        }
    }
    else if(patternType == 2){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth + cellWidth/2;
                float y = j * cellHeight + cellHeight/2;
                
                float noise1 = ofNoise(i * 0.1, j * 0.1, time * 0.4 + noiseOffset[i][j]);
                float noise2 = ofNoise(i * 0.2, j * 0.2, time * 0.6 + noiseOffset[i][j] + 50);
                
                if(noise1 > 0.3){
                    float radius = min(cellWidth, cellHeight) * 0.4 * noise2;
                    float brightness = ofMap(noise2, 0, 1, 100, 255);
                    
                    ofSetColor(brightness);
                    ofNoFill();
                    ofSetLineWidth(1.0);
                    
                    for(int rings = 0; rings < 3; rings++){
                        float ringRadius = radius * (0.3 + rings * 0.3);
                        float phase = time * (1.0 + rings * 0.5) + i * 0.1 + j * 0.1;
                        float wobble = sin(phase) * 3.0;
                        
                        ofSetColor(brightness * (1.0 - rings * 0.3));
                        ofDrawCircle(x + wobble, y - wobble, ringRadius);
                    }
                }
            }
        }
    }
    else if(patternType == 3){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth;
                float y = j * cellHeight;
                
                if(cellStates[i][j]){
                    ofDrawLine(x, y, x + cellWidth, y + cellHeight);
                    ofDrawLine(x + cellWidth, y, x, y + cellHeight);
                }
            }
        }
    }
    else if(patternType == 3){
        for(int i = 0; i < gridCols - 1; i++){
            for(int j = 0; j < gridRows - 1; j++){
                float x = i * cellWidth;
                float y = j * cellHeight;
                
                if(cellStates[i][j] && cellStates[i+1][j]){
                    ofDrawLine(x + cellWidth/2, y + cellHeight/2, 
                              x + cellWidth + cellWidth/2, y + cellHeight/2);
                }
                if(cellStates[i][j] && cellStates[i][j+1]){
                    ofDrawLine(x + cellWidth/2, y + cellHeight/2, 
                              x + cellWidth/2, y + cellHeight + cellHeight/2);
                }
            }
        }
    }
    else if(patternType == 4){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth + cellWidth/2;
                float y = j * cellHeight + cellHeight/2;
                
                float noise1 = ofNoise(i * 0.06, j * 0.06, time * 0.2 + noiseOffset[i][j]);
                float noise2 = ofNoise(i * 0.12, j * 0.12, time * 0.7 + noiseOffset[i][j] + 300);
                
                if(noise1 > 0.45){
                    float baseRadius = min(cellWidth, cellHeight) * 0.25;
                    float scale = 0.5 + noise2 * 1.5;
                    float radius = baseRadius * scale;
                    
                    float pulse = sin(time * 3.0 + noise1 * 10.0) * 0.2 + 0.8;
                    float alpha = ofMap(noise2, 0, 1, 0.4, 0.9) * pulse;
                    
                    ofSetColor(255, 255 * alpha);
                    ofFill();
                    ofDrawCircle(x, y, radius);
                    
                    ofNoFill();
                    ofSetLineWidth(1.5);
                    ofSetColor(255, 255 * alpha * 0.5);
                    ofDrawCircle(x, y, radius * 1.4);
                }
            }
        }
    }
    else if(patternType == 5){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                float x = i * cellWidth + cellWidth/2;
                float y = j * cellHeight + cellHeight/2;
                
                float centerX = ofGetWidth() / 2;
                float centerY = ofGetHeight() / 2;
                float distance = ofDist(x, y, centerX, centerY);
                float maxDist = ofDist(0, 0, centerX, centerY);
                float normalizedDist = distance / maxDist;
                
                float spiralAngle = atan2(y - centerY, x - centerX);
                float spiralRadius = distance * 0.01;
                float spiralNoise = ofNoise(spiralRadius + time * 0.3, spiralAngle * 2.0 + time * 0.1);
                
                if(spiralNoise > (0.3 + normalizedDist * 0.4)){
                    float radius = min(cellWidth, cellHeight) * 0.2 * (1.0 - normalizedDist * 0.5);
                    float brightness = ofMap(spiralNoise, 0, 1, 150, 255) * (1.0 - normalizedDist * 0.3);
                    
                    float rotation = time * 0.5 + spiralAngle;
                    float wobbleX = cos(rotation + distance * 0.01) * 1.5;
                    float wobbleY = sin(rotation + distance * 0.01) * 1.5;
                    
                    ofSetColor(brightness);
                    ofFill();
                    ofDrawCircle(x + wobbleX, y + wobbleY, radius);
                    
                    if(spiralNoise > 0.8){
                        ofNoFill();
                        ofSetLineWidth(1.0);
                        ofSetColor(brightness * 0.7);
                        ofDrawCircle(x + wobbleX, y + wobbleY, radius * 1.8);
                    }
                }
            }
        }
    }
    
    if(showGrid){
        ofSetColor(64);
        for(int i = 0; i <= gridCols; i++){
            ofDrawLine(i * cellWidth, 0, i * cellWidth, ofGetHeight());
        }
        for(int j = 0; j <= gridRows; j++){
            ofDrawLine(0, j * cellHeight, ofGetWidth(), j * cellHeight);
        }
    }
    
//    ofSetColor(255);
//    string info = "SPACE/ENTER: Toggle grid\n";
//    info += "1-6: Change patterns (1:Squares 2:Organic 3:Ripples 4:Crosses 5:Bubbles 6:Galaxy)\n";
//    info += "R: Randomize noise\n";
//    info += "Pattern: " + ofToString(patternType + 1);
//    ofDrawBitmapString(info, 10, 20);
}

void ofApp::keyPressed(int key){
    cout << "Key pressed: " << key << " (char: " << (char)key << ")" << endl;
    
    if(key == ' ' || key == 32){
        showGrid = !showGrid;
        cout << "Grid toggled: " << showGrid << endl;
    }
    if(key >= '1' && key <= '6'){
        patternType = key - '1';
        cout << "Pattern changed to: " << (patternType + 1) << endl;
    }
    if(key == 'r' || key == 'R'){
        for(int i = 0; i < gridCols; i++){
            for(int j = 0; j < gridRows; j++){
                noiseOffset[i][j] = ofRandom(1000);
            }
        }
        cout << "Noise randomized" << endl;
    }
    
    switch(key){
        case OF_KEY_F1:
        case '1':
            patternType = 0;
            cout << "Pattern 1: Rectangles" << endl;
            break;
        case OF_KEY_F2:
        case '2':
            patternType = 1;
            cout << "Pattern 2: Circles" << endl;
            break;
        case OF_KEY_F3:
        case '3':
            patternType = 2;
            cout << "Pattern 3: Crosses" << endl;
            break;
        case OF_KEY_F4:
        case '4':
            patternType = 3;
            cout << "Pattern 4: Lines" << endl;
            break;
        case '5':
            patternType = 4;
            cout << "Pattern 5: Bubbles" << endl;
            break;
        case '6':
            patternType = 5;
            cout << "Pattern 6: Galaxy" << endl;
            break;
        case OF_KEY_RETURN:
            showGrid = !showGrid;
            break;
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

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
