#include "ofApp.h"
#include <algorithm>

// ---------- helpers ----------
ofFloatColor ofApp::HEX(int hex){
    return ofFloatColor(ofColor::fromHex(hex));
}

// Superformula (Gielis)
float ofApp::super(float theta, float m, float n1, float n2, float n3) const {
    n1 = std::max(0.0001f, n1);
    float a = 1.f, b = 1.f;
    float t = theta;
    float p1 = powf(fabsf(cosf(m * t * 0.25f) / a), n2);
    float p2 = powf(fabsf(sinf(m * t * 0.25f) / b), n3);
    float r  = powf(p1 + p2, -1.f / n1);
    return r;
}

ofFloatColor ofApp::paletteSample(float t) const {
    if(palette.empty()) return ofFloatColor::white;
    float u = ofClamp(t - floorf(t), 0.f, 0.99999f);
    float seg = 1.f / palette.size();
    int i0 = std::min(int(u / seg), (int)palette.size()-1);
    int i1 = (i0 + 1) % palette.size();
    float local = (u - i0*seg) / seg;
    return palette[i0].getLerped(palette[i1], local);
}

void ofApp::rotatePalette() {
    if(palette.empty()) return;
    std::rotate(palette.begin(), palette.begin()+1, palette.end());
}

// ---------- app ----------
void ofApp::setup(){
    ofSetWindowTitle("Supershape 3D — Glow/Rim Edition");
    ofSetFrameRate(60);
    ofBackground(bg);
    ofEnableDepthTest();
    ofEnableAntiAliasing();
    ofEnableSmoothing();

    // Provided theme colors
    int hexes[] = {
        0x5b8bdf, 0x3e79c7, 0x08584f, 0xead3ff, 0x102a67,
        0x3c7dd1, 0x6556bd, 0x645ebe, 0x152d91, 0x8aa5f6
    };
    for(int h : hexes) palette.push_back(HEX(h));

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    points.setMode(OF_PRIMITIVE_POINTS);

    cam.setDistance(520.f);
    cam.setNearClip(0.1f);
    cam.setFarClip(5000.f);

    // load shaders (inside bin/data/shaders)
    meshShader.load("shaders/mesh.vert", "shaders/mesh.frag");
    pointShader.load("shaders/points.vert", "shaders/points.frag");

    rebuildMesh(0.f);
}

void ofApp::update(){
    float t = paused ? 0.f : ofGetElapsedTimef() * timeScale;
    rebuildMesh(t);
}

void ofApp::rebuildMesh(float t){
    mesh.clear();
    points.clear();

    // Animate parameters gently
    float m1 = 4.f + 2.f * sinf(t * 0.9f);   // longitude symmetry
    float m2 = 6.f + 2.f * cosf(t * 0.7f);   // latitude symmetry

    float n1a = 0.6f + 0.8f * (0.5f + 0.5f * sinf(t * 0.43f));
    float n2a = 0.25f + 1.5f * (0.5f + 0.5f * cosf(t * 0.37f));
    float n3a = 0.25f + 1.5f * (0.5f + 0.5f * sinf(t * 0.29f + 1.2f));

    float n1b = 0.6f + 0.8f * (0.5f + 0.5f * cosf(t * 0.51f));
    float n2b = 0.25f + 1.5f * (0.5f + 0.5f * sinf(t * 0.33f + 0.7f));
    float n3b = 0.25f + 1.5f * (0.5f + 0.5f * cosf(t * 0.27f + 0.3f));

    // Build vertex grid (wrap U)
    int cols = resU + 1;
    int rows = resV + 1;
    mesh.getVertices().reserve(cols * rows);
    mesh.getNormals().reserve(cols * rows);
    mesh.getColors().reserve(cols * rows);
    points.getVertices().reserve(cols * rows);
    points.getColors().reserve(cols * rows);

    auto idx = [cols](int u, int v){ return v * cols + u; };

    for(int v = 0; v < rows; ++v){
        float phi   = ofMap(v, 0, rows-1, -HALF_PI, HALF_PI); // [-π/2, π/2]
        float r2    = super(phi, m2, n1b, n2b, n3b);
        for(int u = 0; u < cols; ++u){
            float theta = ofMap(u, 0, cols-1, -PI, PI);       // [-π, π]
            float r1    = super(theta, m1, n1a, n2a, n3a);

            // Parametric position
            float x = r1 * cosf(theta) * r2 * cosf(phi);
            float y = r1 * sinf(theta) * r2 * cosf(phi);
            float z = r2 * sinf(phi);
            glm::vec3 pos = radius * glm::vec3(x, y, z);

            // Swirl + noise displacement
            float tnow = paused ? 0.f : ofGetElapsedTimef() * timeScale;
            float swirl = 0.18f * sinf(tnow * 1.2f + phi * 2.0f);
            float cx = cosf(swirl), sx = sinf(swirl);
            glm::vec3 rp(cx*pos.x + sx*pos.z, pos.y, -sx*pos.x + cx*pos.z);

            glm::vec3 nrm = glm::normalize(rp);
            float n = ofNoise(0.7f*theta + 1.3f, 0.7f*phi - 2.1f, tnow*0.8f);
            float disp = ofMap(n, 0, 1, -14.f, 26.f);
            rp += nrm * disp;

            // Color banding along longitude with time wobble
            float band = ofWrap((float)u/(cols-1) + 0.15f * sinf(tnow + phi*0.7f), 0.f, 1.f);
            ofFloatColor col = paletteSample(band);
            col *= 0.85f + 0.15f * (0.5f + 0.5f * sinf(tnow*2.0f + band * TWO_PI));

            mesh.addVertex(rp);
            mesh.addNormal(nrm);
            mesh.addColor(col);

            points.addVertex(rp);
            points.addColor(ofFloatColor(col.r, col.g, col.b, 0.7f));
        }
    }

    // Tri indices
    mesh.getIndices().reserve(resU * resV * 6);
    for(int v = 0; v < resV; ++v){
        for(int u = 0; u < resU; ++u){
            int i00 = idx(u,   v);
            int i10 = idx(u+1, v);
            int i01 = idx(u,   v+1);
            int i11 = idx(u+1, v+1);
            mesh.addIndex(i00); mesh.addIndex(i10); mesh.addIndex(i11);
            mesh.addIndex(i00); mesh.addIndex(i11); mesh.addIndex(i01);
        }
    }
}

void ofApp::draw(){
    ofBackgroundGradient(ofColor(6,10,20), bg, OF_GRADIENT_CIRCULAR);

    cam.begin();

    float t = ofGetElapsedTimef();
    ofPushMatrix();

    if(autoRotate){
        ofRotateDeg(15.f * sinf(t * 0.35f), 0, 1, 0);
        ofRotateDeg(10.f * cosf(t * 0.27f), 1, 0, 0);
    }

    // Faces (with rim shader)
    if(drawFaces){
        meshShader.begin();
        meshShader.setUniform1f("uTime", t);
        meshShader.setUniform1f("uRimStrength", rimStrength);
        meshShader.setUniform3f("uRimColor", rimColor.r, rimColor.g, rimColor.b);
        mesh.draw();           // uses per-vertex color, rim added in shader
        meshShader.end();
    }

    // Optional wire
    if(drawWire){
        ofSetColor(255, 60);
        mesh.drawWireframe();
    }

    // Glowing points layer
    if(glowOn){
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        pointShader.begin();
        pointShader.setUniform1f("uPointSize", pointSize);
        pointShader.setUniform1f("uTime", t);
        points.draw();
        pointShader.end();
        ofDisableBlendMode();
    }

    ofPopMatrix();
    cam.end();

    // HUD
//    ofSetColor(235);
//    ofDrawBitmapStringHighlight(
//        "Supershape 3D — Glow/Rim Edition\n"
//        "[w] wire  [f] faces  [p] pause  [r] rotate palette\n"
//        "[g] glow points  [+/-] point size  [a] auto-rotate",
//        14, 24, ofColor(0, 0, 0, 160), ofColor::white
//    );
}

void ofApp::keyPressed(int key){
    if(key == 'w' || key == 'W') drawWire = !drawWire;
    else if(key == 'f' || key == 'F') drawFaces = !drawFaces;
    else if(key == 'p' || key == 'P') paused = !paused;
    else if(key == 'r' || key == 'R') rotatePalette();
    else if(key == 'g' || key == 'G') glowOn = !glowOn;
    else if(key == '+' || key == '=') pointSize = std::min(12.0f, pointSize + 0.5f);
    else if(key == '-' || key == '_') pointSize = std::max(1.0f, pointSize - 0.5f);
    else if(key == 'a' || key == 'A') autoRotate = !autoRotate;
}
