#include "ofApp.h"
#include <random>
#include <numeric>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofSetCircleResolution(120);
    ofBackground(0);

    // Seed and initial grid setup
    std::random_device rd;
    seed = rd();
    regenerate(seed);
    // Default view: show the second pattern (Rose) across the grid
    for (auto &c : cells) c.type = Rose;
}

//--------------------------------------------------------------
void ofApp::update(){
    // Nothing heavy; animation handled in draw via time
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(invertGlobal ? 255 : 0);

    const float w = ofGetWidth();
    const float h = ofGetHeight();
    const float x0 = margin;
    const float y0 = margin;
    const float gridW = w - margin * 2.f;
    const float gridH = h - margin * 2.f;
    const float cellW = gridW / cols;
    const float cellH = gridH / rows;

    if (showGrid) {
        drawGridLines(x0, y0, cellW, cellH);
    }

    // Deterministic time when recording
    float baseTime = recording ? (recFrame / recFps) : ofGetElapsedTimef();
    float t = animate ? baseTime * timeScale : 0.f;

    ofColor baseStroke = invertGlobal ? ofColor::black : ofColor::white;

    // Draw math-driven patterns in each grid cell
    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            int idx = j * cols + i;
            float x = x0 + i * cellW;
            float y = y0 + j * cellH;
            drawPatternInCell(i, j, x, y, cellW, cellH, cells[idx], t, baseStroke);
        }
    }

    // Weave overlay across the whole grid (messy grid wires)
    if (weaveOverlay) {
        ofColor c = baseStroke;
        c.a = 70;
        float tWeave = weaveAnimate ? baseTime * timeScale : 0.f;
        drawWeaveOverlay(x0, y0, gridW, gridH, cellW, cellH, tWeave, c);
    }

    // Sacred geometry overlay (centered)
    if (drawSacred) {
        float cx = w * 0.5f;
        float cy = h * 0.5f;
        float baseR = std::min(w, h) * 0.16f; // tuned for 720x1080
        drawSacredGeometryOverlay(cx, cy, baseR);
    }

    // Save frame if recording
    if (recording) {
        ofDirectory dir(recDir);
        if (!dir.exists()) dir.create(true);
        std::string path = recDir + "/frame_" + ofToString(recFrame, 5, '0') + ".png";
        ofSaveScreen(path);
        recFrame++;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'r':
        case 'R': {
            std::random_device rd;
            regenerate(rd());
            break;
        }
        case 'x':
        case 'X':
            regenerate(seed);
            break;
        case 'g':
        case 'G':
            showGrid = !showGrid;
            break;
        case 'a':
        case 'A':
            animate = !animate;
            break;
        case 'd':
        case 'D':
            drawSacred = !drawSacred;
            break;
        case 'w':
        case 'W':
            weaveOverlay = !weaveOverlay;
            break;
        case 'm':
        case 'M':
            weaveStraight = !weaveStraight; // straight vs messy grid lines
            break;
        case 't':
        case 'T':
            weaveAnimate = !weaveAnimate; // steady/animate grid wires
            break;
        case 'i':
        case 'I':
            invertGlobal = !invertGlobal;
            break;
        case 'n':
        case 'N':
            noiseWarp = !noiseWarp;
            break;
        case '[':
            noiseAmp = std::max(0.f, noiseAmp - 1.0f);
            break;
        case ']':
            noiseAmp = std::min(40.f, noiseAmp + 1.0f);
            break;
        case 'k':
        case 'K':
            noiseScale = std::max(0.001f, noiseScale * 0.8f);
            break;
        case 'l':
        case 'L':
            noiseScale = std::min(0.05f, noiseScale * 1.25f);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5': {
            int sel = key - '1';
            PatternType tSel = static_cast<PatternType>(ofClamp(sel, 0, 4));
            for (auto &c : cells) c.type = tSel;
            break;
        }
        case 's':
        case 'S': {
            std::string ts = ofGetTimestampString("%Y%m%d-%H%M%S-%i");
            ofSaveScreen("capture-" + ts + ".png");
            break;
        }
        case 'v':
        case 'V': {
            if (!recording) {
                // start
                recording = true;
                recFrame = 0;
                recDir = "frames/export-" + ofGetTimestampString("%Y%m%d-%H%M%S");
                ofLogNotice() << "Recording started: " << recDir << " @ " << recFps << " fps";
            } else {
                // stop
                ofLogNotice() << "Recording stopped after " << recFrame << " frames -> " << recDir;
                recording = false;
            }
            break;
        }
        case OF_KEY_RETURN:
            ofToggleFullscreen();
            break;
        case OF_KEY_UP:
            rows = std::max(1, rows + 1);
            regenerate(seed);
            break;
        case OF_KEY_DOWN:
            rows = std::max(1, rows - 1);
            regenerate(seed);
            break;
        case OF_KEY_RIGHT:
            cols = std::max(1, cols + 1);
            regenerate(seed);
            break;
        case OF_KEY_LEFT:
            cols = std::max(1, cols - 1);
            regenerate(seed);
            break;
        default:
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
    // Zoom sacred geometry radius with vertical scroll
    margin = ofClamp(margin + scrollY * 2.f, 10.f, 200.f);
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

//--------------------------------------------------------------
void ofApp::regenerate(uint32_t newSeed){
    seed = newSeed;
    rng.seed(seed);

    cells.clear();
    cells.resize(rows * cols);

    std::uniform_int_distribution<int> distType(0, 4);
    std::uniform_int_distribution<int> distFreq(1, 6);   // 1..6
    std::uniform_real_distribution<float> distPhase(0.f, TWO_PI);
    std::uniform_real_distribution<float> distWeight(0.7f, 2.4f);
    std::uniform_real_distribution<float> distAng(-22.f, 22.f);
    std::uniform_real_distribution<float> dist01(0.f, 1.f);
    std::uniform_int_distribution<int> distInt3(2, 9);

    for (auto &c : cells) {
        c.type = static_cast<PatternType>(distType(rng));
        c.weight = distWeight(rng);
        c.phase = distPhase(rng);
        c.angleDeg = distAng(rng);
        c.invert = dist01(rng) < 0.18f; // ~18% inverted tiles

        // Lissajous
        c.a = distFreq(rng);
        c.b = distFreq(rng);
        if (c.a == c.b) c.b = std::max(1, (c.b % 6) + 1);

        // Rose: pick small ratios
        c.kDen = ofClamp(distInt3(rng), 2, 7);
        c.kNum = ofClamp(distInt3(rng), 1, 9);

        // Hypotrochoid: keep integers small for closure
        c.ht_R = ofClamp(distInt3(rng), 3, 9);
        c.ht_r = ofClamp(distInt3(rng), 2, 8);
        if (c.ht_R == c.ht_r) c.ht_r = std::max(2, (c.ht_r % 8) + 1);
        c.ht_d = ofMap(dist01(rng), 0, 1, 1.0f, 6.0f);

        // Superformula: pick pleasant ranges
        c.sf_m = ofClamp(distInt3(rng), 3, 9);
        c.sf_n1 = ofMap(dist01(rng), 0, 1, 0.2f, 4.0f);
        c.sf_n2 = ofMap(dist01(rng), 0, 1, 0.2f, 4.0f);
        c.sf_n3 = ofMap(dist01(rng), 0, 1, 0.2f, 4.0f);
        c.sf_a = 1.f; c.sf_b = 1.f;
    }
}

//--------------------------------------------------------------
void ofApp::drawGridLines(float x, float y, float cw, float ch){
    ofPushStyle();
    ofSetColor(invertGlobal ? 0 : 255, 40); // subtle grid (grayscale + alpha)
    ofSetLineWidth(1);

    // Verticals
    for (int i = 0; i <= cols; ++i) {
        float gx = x + i * cw;
        ofDrawLine(gx, y, gx, y + ch * rows);
    }
    // Horizontals
    for (int j = 0; j <= rows; ++j) {
        float gy = y + j * ch;
        ofDrawLine(x, gy, x + cw * cols, gy);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawPatternInCell(int ci, int cj, float x, float y, float cw, float ch, const CellParams& p, float t, const ofColor& stroke){
    ofPushMatrix();
    ofPushStyle();

    // Per-cell invert background (cover full tile, no border)
    bool inv = p.invert;
    if (inv) {
        ofFill();
        ofSetColor(stroke); // base stroke as background when inverted
        ofDrawRectangle(x, y, cw, ch);
    }

    // Stroke color after filling background
    ofNoFill();
    ofColor cellStroke = inv ? (invertGlobal ? ofColor::white : ofColor::black)
                             : stroke;
    ofSetColor(cellStroke);
    ofSetLineWidth(p.weight);

    ofTranslate(x + cw * 0.5f, y + ch * 0.5f);
    if (p.angleDeg != 0) ofRotateDeg(p.angleDeg);

    bool drewImmediate = false;
    ofPolyline poly;
    switch (p.type) {
        case Lissajous:
            drawLissajousInCell(cw, ch, p, t, poly);
            break;
        case Rose:
            drawRoseInCell(cw, ch, p, t, poly);
            break;
        case Hypotrochoid:
            drawHypotrochoidInCell(cw, ch, p, t, poly);
            break;
        case Superformula:
            drawSuperformulaInCell(cw, ch, p, t, poly);
            break;
        case Truchet:
            drawTruchetInCell(cw, ch, ci, cj, p, t);
            drewImmediate = true;
            break;
    }

    if (!drewImmediate) {
        poly.setClosed(true);
        poly.draw();
    }

    // Removed per-cell framing circle for cleaner look

    ofPopStyle();
    ofPopMatrix();
}

void ofApp::drawLissajousInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly){
    float rx = cw * 0.42f;
    float ry = ch * 0.42f;

    int L = std::lcm(std::max(1, p.a), std::max(1, p.b));
    int steps = std::max(curveSamples, 200) * L;
    float maxU = TWO_PI * L;

    // Preallocation optional; skip for maximum oF compatibility
    for (int i = 0; i <= steps; ++i) {
        float u = ofMap(i, 0, steps, 0.f, maxU, true);
        float px = rx * sinf(p.a * u + p.phase + t * 0.7f);
        float py = ry * sinf(p.b * u + t * 0.9f);
        glm::vec2 v(px, py);
        if (noiseWarp) v = warpPoint(v, t, p.phase);
        poly.addVertex(v.x, v.y);
    }
}

void ofApp::drawRoseInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly){
    float r0 = std::min(cw, ch) * 0.42f;
    float k = (float)p.kNum / (float)p.kDen;
    int L = p.kDen; // close after denominator cycles
    int steps = std::max(curveSamples, 300) * L;
    float maxU = TWO_PI * L;
    // Preallocation optional; skip for maximum oF compatibility
    for (int i = 0; i <= steps; ++i) {
        float u = ofMap(i, 0, steps, 0.f, maxU, true);
        float rr = r0 * cosf(k * u + p.phase * 0.25f + t * 0.4f);
        float px = rr * cosf(u);
        float py = rr * sinf(u);
        glm::vec2 v(px, py);
        if (noiseWarp) v = warpPoint(v, t, p.phase);
        poly.addVertex(v.x, v.y);
    }
}

void ofApp::drawHypotrochoidInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly){
    float rbase = std::min(cw, ch) * 0.40f;
    int R = std::max(2, p.ht_R);
    int r = std::max(1, p.ht_r);
    float d = p.ht_d;

    int L = std::lcm(R, r) / r; // cycles to close
    int steps = std::max(curveSamples, 320) * L;
    float maxU = TWO_PI * L;

    // Normalization to fit within radius
    float maxRad = (R - r) + d;
    float s = rbase / std::max(1.f, maxRad);

    // Preallocation optional; skip for maximum oF compatibility
    for (int i = 0; i <= steps; ++i) {
        float u = ofMap(i, 0, steps, 0.f, maxU, true) + t * 0.25f;
        float px = s * ((R - r) * cosf(u) + d * cosf(((R - r) / (float)r) * u + p.phase));
        float py = s * ((R - r) * sinf(u) - d * sinf(((R - r) / (float)r) * u + p.phase));
        glm::vec2 v(px, py);
        if (noiseWarp) v = warpPoint(v, t, p.phase);
        poly.addVertex(v.x, v.y);
    }
}

void ofApp::drawSuperformulaInCell(float cw, float ch, const CellParams& p, float t, ofPolyline& poly){
    float R0 = std::min(cw, ch) * 0.42f;
    int steps = std::max(curveSamples, 360);
    // Preallocation optional; skip for maximum oF compatibility
    for (int i = 0; i <= steps; ++i) {
        float u = ofMap(i, 0, steps, -PI, PI, true);
        float m = p.sf_m;
        float a = p.sf_a, b = p.sf_b;
        float t1 = powf(fabsf(cosf(m * u / 4.f) / a), p.sf_n2);
        float t2 = powf(fabsf(sinf(m * u / 4.f) / b), p.sf_n3);
        float r = powf(t1 + t2, -1.f / std::max(0.001f, p.sf_n1));
        float px = R0 * r * cosf(u + p.phase * 0.2f + t * 0.2f);
        float py = R0 * r * sinf(u + p.phase * 0.2f + t * 0.2f);
        glm::vec2 v(px, py);
        if (noiseWarp) v = warpPoint(v, t, p.phase);
        poly.addVertex(v.x, v.y);
    }
}

glm::vec2 ofApp::warpPoint(const glm::vec2& pt, float t, float phase) const {
    float s = noiseScale;
    float tx = t * noiseTime + phase * 0.11f;
    float nx = ofNoise(pt.x * s + 11.0f, pt.y * s - 7.0f, tx);
    float ny = ofNoise(pt.x * s - 3.0f, pt.y * s + 5.0f, tx + 13.37f);
    glm::vec2 off((nx - 0.5f) * 2.f * noiseAmp, (ny - 0.5f) * 2.f * noiseAmp);
    return pt + off;
}

void ofApp::drawTruchetInCell(float cw, float ch, int ci, int cj, const CellParams& p, float t){
    // Draw two quarter-ellipses that connect midpoints, classic Truchet tile.
    float rx = cw * 0.5f;
    float ry = ch * 0.5f;
    int segs = std::max(28, (int)std::round(std::min(cw, ch) * 0.35f));

    // Alternate orientation by checkerboard parity
    bool flip = (((ci + cj) & 1) ^ (p.invert ? 1 : 0)) != 0;

    auto arc = [&](float cx, float cy, float a0Deg, float a1Deg){
        ofPolyline poly;
        for (int i = 0; i <= segs; ++i) {
            float u = ofMap(i, 0, segs, a0Deg, a1Deg, true) * DEG_TO_RAD;
            float x = cx + rx * cosf(u);
            float y = cy + ry * sinf(u);
            poly.addVertex(x, y);
        }
        poly.draw();
    };

    // In local coordinates (we are centered at tile center)
    float x0 = -cw * 0.5f;
    float y0 = -ch * 0.5f;
    float x1 = +cw * 0.5f;
    float y1 = +ch * 0.5f;

    if (!flip) {
        // Top-left corner arc: top-mid -> left-mid
        arc(x0, y0, 0, 90);
        // Bottom-right corner arc: bottom-mid -> right-mid
        arc(x1, y1, 180, 270);
    } else {
        // Top-right corner arc: top-mid -> right-mid
        arc(x1, y0, 180, 90); // draw backwards (will still render fine)
        // Bottom-left corner arc: bottom-mid -> left-mid
        arc(x0, y1, 0, -90);
    }
}

//--------------------------------------------------------------
void ofApp::drawSacredGeometryOverlay(float cx, float cy, float baseR){
    ofPushStyle();
    ofNoFill();
    ofSetColor(invertGlobal ? 0 : 255, 90);
    ofSetLineWidth(1.6f);

    // Seed of Life: 1 center + 6 around
    ofDrawCircle(cx, cy, baseR);
    for (int k = 0; k < 6; ++k) {
        float ang = k * TWO_PI / 6.f;
        float ox = cx + cosf(ang) * baseR;
        float oy = cy + sinf(ang) * baseR;
        ofDrawCircle(ox, oy, baseR);
    }

    // Hexagon connecting the 6 centers
    ofSetColor(invertGlobal ? 0 : 255, 140);
    ofPolyline hex;
    for (int k = 0; k < 6; ++k) {
        float ang = k * TWO_PI / 6.f + TWO_PI / 12.f; // slightly rotated
        float ox = cx + cosf(ang) * baseR;
        float oy = cy + sinf(ang) * baseR;
        hex.addVertex(ox, oy);
    }
    hex.setClosed(true);
    hex.draw();

    // Radial lines (metatron-esque spokes)
    ofSetColor(invertGlobal ? 0 : 255, 60);
    for (int k = 0; k < 12; ++k) {
        float ang = k * TWO_PI / 12.f;
        float ox = cx + cosf(ang) * baseR * 2.f;
        float oy = cy + sinf(ang) * baseR * 2.f;
        ofDrawLine(cx, cy, ox, oy);
    }

    ofPopStyle();
}

void ofApp::drawWeaveOverlay(float x, float y, float gridW, float gridH, float cw, float ch, float t, const ofColor& c){
    ofPushStyle();
    ofNoFill();
    ofSetColor(c);
    ofSetLineWidth(1.0f);

    if (weaveStraight) {
        // Straight grid lines: draw on cell boundaries
        // Verticals
        for (int i = 0; i <= cols; ++i) {
            float gx = x + i * cw;
            ofDrawLine(gx, y, gx, y + gridH);
        }
        // Horizontals
        for (int j = 0; j <= rows; ++j) {
            float gy = y + j * ch;
            ofDrawLine(x, gy, x + gridW, gy);
        }
    } else {
        // Messy (wavy) grid lines
        int vSegs = std::max(12, rows * 14);
        int hSegs = std::max(12, cols * 14);
        float ampX = cw * 0.12f;
        float ampY = ch * 0.12f;

        // Vertical waves per column
        for (int i = 0; i <= cols; ++i) {
            float baseX = x + i * cw;
            ofPolyline poly;
            float freq = 0.012f + 0.008f * ofNoise(i * 0.21f + t * 0.15f);
            float a = ampX * ofMap(ofNoise(i * 0.11f + t * 0.2f), 0, 1, 0.55f, 1.1f);
            for (int s = 0; s <= vSegs; ++s) {
                float yy = ofMap(s, 0, vSegs, y, y + gridH, true);
                float wobS = sinf(yy * freq + i * 0.7f + t * 0.9f) * a;
                float wobN = (ofNoise(yy * 0.010f + i * 0.33f + t * 0.4f) - 0.5f) * 2.f * a * 0.35f;
                float wob = wobS + wobN;
                poly.addVertex(baseX + wob, yy);
            }
            poly.draw();
        }

        // Horizontal waves per row
        for (int j = 0; j <= rows; ++j) {
            float baseY = y + j * ch;
            ofPolyline poly;
            float freq = 0.012f + 0.008f * ofNoise(j * 0.23f + t * 0.12f + 3.7f);
            float a = ampY * ofMap(ofNoise(j * 0.15f + t * 0.18f), 0, 1, 0.55f, 1.1f);
            for (int s = 0; s <= hSegs; ++s) {
                float xx = ofMap(s, 0, hSegs, x, x + gridW, true);
                float wobS = sinf(xx * freq + j * 0.8f + t * 0.8f + 1.57f) * a;
                float wobN = (ofNoise(xx * 0.010f + j * 0.29f + t * 0.36f) - 0.5f) * 2.f * a * 0.35f;
                float wob = wobS + wobN;
                poly.addVertex(xx, baseY + wob);
            }
            poly.draw();
        }
    }

    ofPopStyle();
}
