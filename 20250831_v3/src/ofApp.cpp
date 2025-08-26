#include "ofApp.h"

static const float PHI = 1.61803398875f;

// ---------------- Palette helper ----------------
ofFloatColor ofApp::HEX(int hex){ return ofFloatColor(ofColor::fromHex(hex)); }

// ---------------- Supershape ----------------
float ofApp::supershape(float theta, float m, float n1, float n2, float n3, float a, float b) const {
    float p1 = powf(fabsf(cosf(m * theta * 0.25f) / a), n2);
    float p2 = powf(fabsf(sinf(m * theta * 0.25f) / b), n3);
    float sum = p1 + p2;
    float e = -1.0f / std::max(0.0001f, n1);
    return powf(sum, e);
}

// ---------------- Colors ----------------
ofFloatColor ofApp::bgColor() const { return invert ? ofFloatColor(1.0f) : HEX(0x102a67); }
ofFloatColor ofApp::inkColor() const{ return invert ? ofFloatColor(0.0f) : ofFloatColor(1.0f); }

// ---------------- Setup ----------------
void ofApp::setup(){
    ofSetWindowTitle("Palette Bloom — complex ribbon glow");
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    // theme palette
    palette = {
        HEX(0x5b8bdf), HEX(0x3e79c7), HEX(0x08584f), HEX(0xead3ff), HEX(0x102a67),
        HEX(0x3c7dd1), HEX(0x6556bd), HEX(0x645ebe), HEX(0x152d91), HEX(0x8aa5f6)
    };

    // layers — richer & more complex
    const int L = 10;
    layers.reserve(L);
    for(int i=0;i<L;++i){
        Layer layer{};
        layer.scale     = ofMap(i, 0, L-1, 0.20f, 1.00f);
        layer.baseRot   = ofDegToRad(7.5f * i);

        layer.m   = 4 + (i % 6);                 // 4..9
        layer.n1  = ofMap(i, 0, L-1, 0.30f, 1.60f);
        layer.n2  = ofMap(i, 0, L-1, 0.80f, 2.20f);
        layer.n3  = ofMap(i, 0, L-1, 0.80f, 2.20f);

        layer.m2         = layer.m + 2.0f;
        layer.blendSpeed = 0.20f + 0.035f * i;

        layer.harmK   = 3.0f + (i % 5);          // epicycloid order
        layer.harmAmp = 0.06f + 0.02f * (i%4);   // gentle

        layer.petalFreq = 0.5f + 0.25f * (i % 5);
        layer.petalPow  = 0.65f + 0.08f * i;
        layer.warpAmp   = 0.16f + 0.02f * i;
        layer.warpFreq  = 1.4f  + 0.30f * i;

        layer.wobbleAmp  = 0.10f + 0.045f * i;
        layer.wobbleFreq = 1.5f  + 0.55f  * i;

        layer.baseWidth   = 3.0f + 0.6f * i;     // pixels
        layer.widthJitter = 1.5f + 0.2f * i;
        layer.lineWeight  = 0.8f + 0.18f * i;

        layer.color = palette[i % palette.size()];
        layers.push_back(layer);
    }

    allocateAll(ofGetWidth(), ofGetHeight());
    loadShaders();
    initDust(200);
}

// ---------------- Allocate FBOs ----------------
void ofApp::allocateAll(int w, int h){
    ofFbo::Settings s; s.width=w; s.height=h; s.internalformat=GL_RGBA;
    s.useDepth=false; s.useStencil=false; s.numSamples=0;

    trailFbo.allocate(s);
    layersFbo.allocate(s);
    blurFbo[0].allocate(s);
    blurFbo[1].allocate(s);

    trailFbo.begin(); {
        ofFloatColor fc = bgColor();
        ofClear(to8(fc.r), to8(fc.g), to8(fc.b), 255);
    } trailFbo.end();

    auto clearTransparent = [&](ofFbo& f){ f.begin(); ofClear(0,0,0,0); f.end(); };
    clearTransparent(layersFbo);
    clearTransparent(blurFbo[0]);
    clearTransparent(blurFbo[1]);
}

// ---------------- Shaders ----------------
void ofApp::loadShaders(){
    blurShader.load("shaders/passthrough.vert", "shaders/blur.frag");
}

// ---------------- Dust ----------------
void ofApp::initDust(int count){
    dust.clear(); dust.reserve(count);
    float R = std::min(ofGetWidth(), ofGetHeight()) * 0.5f;
    for(int i=0;i<count;++i){
        Dust d;
        d.r = ofRandom(R*0.07f, R*0.96f);
        d.a = ofRandom(TWO_PI);
        d.size = ofRandom(1.0f, 3.0f);
        d.w = ofRandom(-0.28f, 0.28f);
        d.c = palette[i % palette.size()];
        d.c.a = ofRandom(0.18f, 0.85f);
        dust.push_back(d);
    }
}
void ofApp::updateDust(float dt){
    for(auto& d : dust){
        d.a += d.w * dt;
        d.r += 0.10f * sinf(t*0.5f + d.a*0.7f);
    }
}
void ofApp::drawDust(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f);
    for(const auto& d : dust){
        float x = d.r * cosf(d.a);
        float y = d.r * sinf(d.a);
        ofSetColor(to8(d.c.r), to8(d.c.g), to8(d.c.b), to8(d.c.a));
        ofDrawCircle(x, y, d.size);
    }
    ofPopMatrix();
}

// ---------------- Update ----------------
void ofApp::update(){
    float dt = ofGetLastFrameTime();
    if(!paused) t += dt * speed;
    updateDust(dt);
}

// ---------------- Build curvature-adaptive ribbon (triangle strip) ----------------
void ofApp::buildLayerRibbon(ofMesh& mesh, const Layer& L, float time) const {
    // 1) Build base polyline points (complex radius)
    std::vector<glm::vec2> pts; pts.reserve(samples+1);

    const float S = std::min(ofGetWidth(), ofGetHeight()) * 0.5f * L.scale;
    float wBlend = 0.5f + 0.5f * sinf(time * L.blendSpeed + L.m * 0.37f);
    float rot    = L.baseRot + 0.32f * sinf(time * (0.55f + 0.12f * L.m));
    float n1t    = L.n1 + 0.24f * sinf(time * 0.61f + 0.6f * L.m);

    for(int i=0;i<=samples;++i){
        float theta  = ofMap(i, 0, samples, 0.0f, TWO_PI);
        float thetap = theta + L.warpAmp * sinf(theta * L.warpFreq + time * 0.9f);

        // two supershapes
        float r1 = supershape(thetap,       L.m,  std::max(0.05f, n1t), L.n2, L.n3);
        float r2 = supershape(thetap * PHI, L.m2, std::max(0.05f, n1t), L.n2, L.n3);
        float r  = ofLerp(r1, r2, wBlend);

        // epicycloid-ish accent (adds gentle petals)
        r *= (1.0f + L.harmAmp * cosf(L.harmK * thetap + 0.35f * sinf(time*0.7f)));

        // petal pinch
        float pinch = powf(fabsf(cosf(L.petalFreq * thetap * 0.5f)), L.petalPow);
        r *= (0.80f + 0.20f * pinch);

        // subtle radial wobble
        float wobK = L.wobbleAmp * (0.65f + 0.35f * sinf(time * 0.84f + 1.3f * L.m));
        float wobble = 1.0f + wobK * sinf(L.wobbleFreq * thetap + time * 1.2f + L.m);

        float R = S * r * wobble;

        float x = R * cosf(theta);
        float y = R * sinf(theta);

        // rotate
        float xr =  x * cosf(rot) - y * sinf(rot);
        float yr =  x * sinf(rot) + y * cosf(rot);

        pts.emplace_back(xr, yr);
    }

    // 2) Resample & smooth for elegant curve
    ofPolyline poly;
    for(auto& p: pts) poly.addVertex(glm::vec3(p, 0.0f));
    poly.close();

    auto resampled = poly.getResampledByCount(samples/2);
    auto smooth    = resampled.getSmoothed(9);
    const auto& v  = smooth.getVertices();
    const int N    = (int)v.size();

    // 3) Build ribbon using vertex normals & curvature
    //    curvature ~ angle change; thinner at sharp turns
    auto dirAt = [&](int idx)->glm::vec2{
        int i0 = (idx-1+N)%N, i1 = (idx+1)%N;
        glm::vec2 a(v[i0].x, v[i0].y), b(v[i1].x, v[i1].y);
        glm::vec2 d = glm::normalize(b - a);
        return d;
    };
    auto normalAt = [&](int idx)->glm::vec2{
        glm::vec2 d = dirAt(idx);
        return glm::vec2(-d.y, d.x);
    };
    auto curvatureAt = [&](int idx)->float{
        glm::vec2 d0 = dirAt((idx-1+N)%N);
        glm::vec2 d1 = dirAt((idx+1)%N);
        float c = 0.5f*(1.0f - glm::dot(d0, d1)); // 0 (straight) .. 1 (sharp turn)
        return ofClamp(c, 0.0f, 1.0f);
    };

    // 4) Create triangle strip
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    for(int i=0;i<N;++i){
        glm::vec2 p(v[i].x, v[i].y);
        glm::vec2 n = normalAt(i);

        float curv = curvatureAt(i);
        float width = L.baseWidth * ofLerp(1.2f, 0.55f, curv); // thinner at high curvature
        width += L.widthJitter * sinf(0.8f*i + 0.35f*t + L.m); // micro variation

        glm::vec2 pL = p - n * width;
        glm::vec2 pR = p + n * width;

        // gradient along strip: inner/outer slight alpha difference
        float u = (float)i / (float)(N-1);
        float aInner = 0.85f + 0.15f * sinf(6.2831f*u + 0.7f*t + L.m);
        float aOuter = 0.55f + 0.35f * sinf(6.2831f*u + 1.4f*t + L.m*0.5f);

        ofFloatColor cIn = L.color;  cIn.a  = aInner;
        ofFloatColor cOut= L.color;  cOut.a = aOuter;

        // left
        mesh.addVertex(glm::vec3(pL, 0.0f));
        mesh.addColor(cOut);
        // right
        mesh.addVertex(glm::vec3(pR, 0.0f));
        mesh.addColor(cIn);
    }
}

// ---------------- Draw layers ----------------
void ofApp::drawLayers(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f);

    for(const auto& L : layers){
        ofMesh ribbon; buildLayerRibbon(ribbon, L, t);

        // ribbon body (additive)
        ofSetColor(255,255,255,255);
        ribbon.draw();

        // gentle outline to crispen (wire overlay from midpoints)
        ofMesh wire; wire.setMode(OF_PRIMITIVE_LINE_STRIP);
        const auto& verts = ribbon.getVertices();
        // sample every 2 to approximate centerline
        for(size_t i=0; i+1<verts.size(); i+=2){
            glm::vec3 mid = 0.5f*(verts[i] + verts[i+1]);
            wire.addVertex(mid);
            wire.addColor(ofFloatColor(1.0f)); // color unused; we'll set global
        }
        ofSetLineWidth(L.lineWeight);
        ofFloatColor cc = L.color; cc.a = 0.35f;
        ofSetColor(to8(cc.r), to8(cc.g), to8(cc.b), to8(cc.a));
        wire.draw();
    }

    ofPopMatrix();
}

// ---------------- Draw ----------------
void ofApp::draw(){
    // 1) TRAILS (fade + draw ribbons)
    trailFbo.begin(); {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        // background + soft radial wash
        ofFloatColor fc = bgColor();
        ofSetColor(to8(fc.r), to8(fc.g), to8(fc.b), 255);
        ofDrawRectangle(0,0, trailFbo.getWidth(), trailFbo.getHeight());
        {
            glm::vec2 c(ofGetWidth()*0.5f, ofGetHeight()*0.5f);
            float R = std::sqrt(c.x*c.x + c.y*c.y);
            ofFloatColor wash = invert ? ofFloatColor(0.94f) : ofFloatColor(0.06f,0.10f,0.20f,1.0f);
            for(int i=6;i>=1;--i){
                float k = i/6.0f;
                unsigned char a = (unsigned char)(24*k);
                ofSetColor(to8(wash.r), to8(wash.g), to8(wash.b), a);
                ofDrawCircle(c.x, c.y, R*k);
            }
        }

        // trails fade
        ofSetColor(to8(fc.r), to8(fc.g), to8(fc.b), clamp8(trailFade));
        ofDrawRectangle(0,0, trailFbo.getWidth(), trailFbo.getHeight());

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        drawLayers();
    } trailFbo.end();

    // 2) Clean layer pass (for glow, includes dust)
    layersFbo.begin(); {
        ofClear(0,0,0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        drawLayers();
        drawDust();
    } layersFbo.end();

    // 3) Blur H/V
    int w = blurFbo[0].getWidth(), h = blurFbo[0].getHeight();
    float texelX = 1.0f / std::max(1, w);
    float texelY = 1.0f / std::max(1, h);

    blurFbo[0].begin(); {
        ofClear(0,0,0,0);
        blurShader.begin();
        blurShader.setUniformTexture("uTex0", layersFbo.getTexture(), 0);
        blurShader.setUniform2f("uTexel", texelX, texelY);
        blurShader.setUniform2f("uDir", 1.0f, 0.0f);
        layersFbo.getTexture().draw(0,0, w,h);
        blurShader.end();
    } blurFbo[0].end();

    blurFbo[1].begin(); {
        ofClear(0,0,0,0);
        blurShader.begin();
        blurShader.setUniformTexture("uTex0", blurFbo[0].getTexture(), 0);
        blurShader.setUniform2f("uTexel", texelX, texelY);
        blurShader.setUniform2f("uDir", 0.0f, 1.0f);
        blurFbo[0].getTexture().draw(0,0, w,h);
        blurShader.end();
    } blurFbo[1].end();

    // 4) Present
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255,255,255,255);
    trailFbo.draw(0,0);

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255,255,255,215);
    blurFbo[1].draw(0,0);

    if(showHud){
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofFloatColor ic = inkColor();
        ofSetColor(to8(ic.r), to8(ic.g), to8(ic.b), 200);
        ofDrawBitmapStringHighlight(
            "Palette Bloom — complex ribbon glow\n"
            "[H] hide HUD  |  [Space] pause  |  [T] trails  |  [+/-] speed  |  [I] invert  |  [S] save PNG",
            16, 24);
    }
}

// ---------------- Events ----------------
void ofApp::keyPressed(int key){
    switch(key){
        case ' ': paused = !paused; break;
        case 'h': case 'H': showHud = !showHud; break;
        case 't': case 'T': trailFade = (trailFade < 254 ? 255.0f : 16.0f); break;
        case '+': speed = std::min(3.0f, speed + 0.05f); break;
        case '-': speed = std::max(0.02f, speed - 0.05f); break;
        case 'i': case 'I': {
            invert = !invert;
            trailFbo.begin(); {
                ofFloatColor fc = bgColor();
                ofClear(to8(fc.r), to8(fc.g), to8(fc.b), 255);
            } trailFbo.end();
        } break;
        case 's': case 'S': {
            std::string ts = ofGetTimestampString("%Y%m%d-%H%M%S");
            ofSaveScreen("palette_bloom_ribbon_"+ts+".png");
        } break;
        default: break;
    }
}

void ofApp::windowResized(int w, int h){ allocateAll(w, h); }
