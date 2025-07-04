// ofApp.cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glEnable(GL_POINT_SMOOTH);

    // Chakra palette (root → crown)
    chakraColors = {
        ofColor::fromHex(0xE52B50),
        ofColor::fromHex(0xFF8300),
        ofColor::fromHex(0xFFD700),
        ofColor::fromHex(0x00A86B),
        ofColor::fromHex(0x0000FF),
        ofColor::fromHex(0x4B0082),
        ofColor::fromHex(0x8A2BE2)
    };

    // Define three sphere layers
    int layersCount = 3;
    float radii[]      = {150.0f, 200.0f, 250.0f};
    float scales[]     = {0.4f,   0.6f,   0.8f};
    float speeds[]     = {1.0f,   1.2f,   1.4f};

    const int latSteps = 30;
    const int lonSteps = 60;

    for(int i = 0; i < layersCount; i++){
        ofVboMesh mesh;
        mesh.setMode(OF_PRIMITIVE_POINTS);

        // store original unit‐directions
        vector<glm::vec3> dirs;
        for(int lat = 0; lat <= latSteps; lat++){
            float theta = ofMap(lat, 0, latSteps, 0, PI);
            for(int lon = 0; lon < lonSteps; lon++){
                float phi = ofMap(lon, 0, lonSteps, 0, TWO_PI);
                glm::vec3 d = {
                    sin(theta)*cos(phi),
                    cos(theta),
                    sin(theta)*sin(phi)
                };
                dirs.push_back(d);
                mesh.addVertex(d * radii[i]);
                mesh.addColor(ofColor::white);
            }
        }

        layers.push_back(mesh);
        baseDirs.push_back(dirs);
        baseRadii.push_back(radii[i]);
        noiseScales.push_back(scales[i]);
        pulseSpeeds.push_back(speeds[i]);
    }

    // start time
    startTime = ofGetElapsedTimef();

    // disable manual cam control
    cam.setAutoDistance(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    float t = ofGetElapsedTimef() - startTime;

    // update each layer
    for(int i = 0; i < layers.size(); i++){
        auto& mesh = layers[i];
        float scale = noiseScales[i];
        float speed = pulseSpeeds[i];
        float radius= baseRadii[i];
        auto& dirs = baseDirs[i];

        // for each vertex...
        for(int j = 0; j < mesh.getNumVertices(); j++){
            glm::vec3 dir = dirs[j];
            // 4D noise: direction + time
            float n = ofNoise(dir.x*scale, dir.y*scale, dir.z*scale, t*0.2f*speed);

            // pulsing radius
            float pulse = sin(t*speed + n*PI) * 0.2f + 1.0f;
            mesh.setVertex(j, dir * (radius * pulse));

            // smooth color blend between chakras
            float cpos = n * (chakraColors.size() - 1);
            int   idx  = floor(cpos);
            float frac = cpos - idx;
            ofColor c1 = chakraColors[idx];
            ofColor c2 = chakraColors[min(idx+1, (int)chakraColors.size()-1)];
            mesh.setColor(j, c1.getLerped(c2, frac));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    float t = ofGetElapsedTimef() - startTime;

    // auto-orbit camera
    float camAngle = t * 10.0f;
    float camX     = cos(camAngle * DEG_TO_RAD) * 600.0f;
    float camZ     = sin(camAngle * DEG_TO_RAD) * 600.0f;
    float camY     = sin(t * 0.3f) * 200.0f + 100.0f;
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(glm::vec3(0));

    cam.begin();
        glPointSize(3.0f);
        for(auto& mesh : layers){
            mesh.draw();
        }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        ofToggleFullscreen();
    }
}
