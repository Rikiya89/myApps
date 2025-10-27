#include "ofApp.h"

#include <algorithm>
#include <array>

namespace {
constexpr int kWindowWidth = 720;
constexpr int kWindowHeight = 1080;
}

void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofEnableAntiAliasing();
    ofSetWindowTitle("Chromatic Drift");

    setupPalette();
    setupRibbons();
    setupHalos();
    setupBackgroundDust();
    setupSparkles();
}

void ofApp::setupPalette() {
    palette.clear();
    palette.reserve(10);

    const std::array<uint32_t, 10> hexColors = {
        0x362d78, 0x523fa3, 0x916ccc, 0xbda1e5, 0xc8c0e9,
        0x84bae7, 0x516ad4, 0x333f87, 0x293039, 0x283631
    };

    for (auto hex : hexColors) {
        auto color = ofFloatColor::fromHex(hex);
        color.a = 1.0f;
        palette.push_back(color);
    }

    bgTop = palette[6].getLerped(palette[1], 0.55f);
    bgTop = bgTop.getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f), 0.04f);
    bgTop.a = 1.0f;

    bgMiddle = palette[5].getLerped(palette[3], 0.4f);
    bgMiddle = bgMiddle.getLerped(bgTop, 0.25f);
    bgMiddle.a = 1.0f;

    bgBottom = palette[9].getLerped(palette[8], 0.58f);
    bgBottom = bgBottom.getLerped(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f), 0.08f);
    bgBottom.a = 1.0f;

    radialHighlightColor = palette[5].getLerped(palette[2], 0.6f);
    radialHighlightColor = radialHighlightColor.getLerped(palette[3], 0.35f);
    radialHighlightColor.a = 0.75f;
}

void ofApp::setupRibbons() {
    ribbons.clear();

    const int ribbonCount = 12;
    const float height = static_cast<float>(ofGetHeight());

    for (int i = 0; i < ribbonCount; ++i) {
        RibbonLayer ribbon;
        ribbon.baseY = ofMap(i, 0, ribbonCount - 1, height * 0.12f, height * 0.88f) + ofRandom(-55.0f, 55.0f);
        ribbon.noiseScale = ofRandom(0.0022f, 0.0048f);
        ribbon.noiseSpeed = ofRandom(0.12f, 0.28f);
        ribbon.noiseOffset = ofRandom(1000.0f);
        ribbon.waveAmplitude = ofRandom(70.0f, 190.0f);
        ribbon.thickness = ofRandom(22.0f, 58.0f);

        const auto paletteIndex = static_cast<size_t>((i * 7) % palette.size());
        ribbon.color = palette[paletteIndex].getLerped(palette[(paletteIndex + 3) % palette.size()], ofRandom(0.05f, 0.25f));
        ribbon.color.a = 1.0f;

        ribbons.push_back(ribbon);
    }
}

void ofApp::setupHalos() {
    halos.clear();

    const int randomHalos = 70;
    for (int i = 0; i < randomHalos; ++i) {
        HaloDot halo;
        halo.anchor = {ofRandomWidth(), ofRandomHeight()};
        halo.radius = ofRandom(18.0f, 52.0f);
        halo.noiseOffset = ofRandom(1000.0f);
        halo.noiseSpeed = ofRandom(0.18f, 0.42f);
        halo.color = palette[static_cast<size_t>(ofRandom(palette.size()))];
        halo.color = halo.color.getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f), ofRandom(0.1f, 0.35f));
        halo.color.a = ofRandom(0.18f, 0.35f);
        halos.push_back(halo);
    }

    // Halos aligned with ribbons for subtle glow accents.
    for (const auto &ribbon : ribbons) {
        const int haloPerRibbon = 3;
        for (int j = 0; j < haloPerRibbon; ++j) {
            HaloDot halo;
            float x = ofRandom(60.0f, kWindowWidth - 60.0f);
            float y = ofClamp(ribbon.baseY + ofRandom(-90.0f, 90.0f), 40.0f, kWindowHeight - 40.0f);
            halo.anchor = {x, y};
            halo.radius = ofRandom(28.0f, 76.0f);
            halo.noiseOffset = ofRandom(1000.0f);
            halo.noiseSpeed = ofRandom(0.09f, 0.22f);
            const size_t paletteIndex = static_cast<size_t>((&ribbon - &ribbons.front()) % palette.size());
            halo.color = palette[paletteIndex].getLerped(palette[(paletteIndex + 5) % palette.size()], ofRandom(0.05f, 0.4f));
            halo.color.a = ofRandom(0.12f, 0.28f);
            halos.push_back(halo);
        }
    }
}

void ofApp::setupBackgroundDust() {
    backgroundDust.clear();
    const int dotCount = 220;
    backgroundDust.reserve(dotCount);

    for (int i = 0; i < dotCount; ++i) {
        TextureDot dot;
        dot.position = {ofRandom(kWindowWidth), ofRandom(kWindowHeight)};
        dot.size = ofRandom(1.1f, 3.6f);
        dot.alpha = ofRandom(0.04f, 0.12f);
        dot.noiseOffset = ofRandom(1000.0f);
        size_t paletteIndex = static_cast<size_t>(ofRandom(palette.size()));
        dot.color = palette[paletteIndex].getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f), ofRandom(0.25f, 0.6f));
        dot.color.a = 1.0f;
        backgroundDust.push_back(dot);
    }
}

void ofApp::setupSparkles() {
    sparkles.clear();
    const int sparkleCount = 44;
    sparkles.reserve(sparkleCount);

    for (int i = 0; i < sparkleCount; ++i) {
        Sparkle sparkle;
        sparkle.position = {ofRandom(kWindowWidth), ofRandom(kWindowHeight)};
        sparkle.baseRadius = ofRandom(1.6f, 3.8f);
        sparkle.noiseOffset = ofRandom(1000.0f);
        sparkle.noiseSpeed = ofRandom(0.32f, 0.68f);
        size_t paletteIndex = static_cast<size_t>((i * 3 + static_cast<int>(ofRandom(palette.size()))) % palette.size());
        sparkle.color = palette[paletteIndex].getLerped(palette[(paletteIndex + 4) % palette.size()], ofRandom(0.2f, 0.55f));
        sparkle.color.a = 1.0f;
        sparkles.push_back(sparkle);
    }
}

void ofApp::update() {
    time = ofGetElapsedTimef();
    backgroundPulse = ofMap(sinf(time * 0.18f), -1.0f, 1.0f, 0.0f, 1.0f);
}

void ofApp::draw() {
    drawBackground();
    drawBackgroundDust();

    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    drawRibbons();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    drawHalos();
    ofDisableBlendMode();
    ofPopStyle();

    drawSparkles();
}

void ofApp::drawBackground() {
    const float width = static_cast<float>(ofGetWidth());
    const float height = static_cast<float>(ofGetHeight());

    ofMesh gradient;
    gradient.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    gradient.addVertex(glm::vec3(0.0f, 0.0f, 0.0f));
    gradient.addColor(bgTop);
    gradient.addVertex(glm::vec3(width, 0.0f, 0.0f));
    gradient.addColor(bgTop);
    gradient.addVertex(glm::vec3(0.0f, height * 0.5f, 0.0f));
    gradient.addColor(bgMiddle);
    gradient.addVertex(glm::vec3(width, height * 0.5f, 0.0f));
    gradient.addColor(bgMiddle);
    gradient.addVertex(glm::vec3(0.0f, height, 0.0f));
    gradient.addColor(bgBottom);
    gradient.addVertex(glm::vec3(width, height, 0.0f));
    gradient.addColor(bgBottom);
    gradient.draw();

    glm::vec2 glowCenter(width * 0.52f, height * 0.42f);
    ofMesh glow;
    glow.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    ofFloatColor centerColor = radialHighlightColor;
    centerColor.a *= ofLerp(0.6f, 1.25f, backgroundPulse);
    glow.addVertex(glm::vec3(glowCenter.x, glowCenter.y, 0.0f));
    glow.addColor(centerColor);

    const int glowResolution = 80;
    float highlightRadius = std::max(width, height) * ofLerp(0.55f, 0.9f, backgroundPulse);
    for (int i = 0; i <= glowResolution; ++i) {
        float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(glowResolution);
        glm::vec2 pos = glowCenter + glm::vec2(cosf(angle), sinf(angle)) * highlightRadius;
        ofFloatColor edgeColor = radialHighlightColor;
        edgeColor.a = 0.0f;
        glow.addVertex(glm::vec3(pos.x, pos.y, 0.0f));
        glow.addColor(edgeColor);
    }

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glow.draw();
    ofDisableBlendMode();

    ofMesh mist;
    mist.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    float mistTopY = height * 0.3f;
    float mistMidY = height * 0.58f;
    float mistBottomY = height * 0.82f;
    ofFloatColor mistLow = palette[5];
    mistLow.a = 0.0f;
    ofFloatColor mistHigh = palette[5].getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f), 0.45f);
    mistHigh.a = 0.12f;
    mist.addVertex(glm::vec3(-40.0f, mistTopY, 0.0f));
    mist.addColor(mistLow);
    mist.addVertex(glm::vec3(width + 40.0f, mistTopY, 0.0f));
    mist.addColor(mistLow);
    mist.addVertex(glm::vec3(-40.0f, mistMidY, 0.0f));
    mist.addColor(mistHigh);
    mist.addVertex(glm::vec3(width + 40.0f, mistMidY, 0.0f));
    mist.addColor(mistHigh);
    mist.addVertex(glm::vec3(-40.0f, mistBottomY, 0.0f));
    mist.addColor(mistLow);
    mist.addVertex(glm::vec3(width + 40.0f, mistBottomY, 0.0f));
    mist.addColor(mistLow);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    mist.draw();

    // Subtle vignette for depth.
    ofMesh vignette;
    vignette.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    ofFloatColor vignetteCenterColor = ofFloatColor(0.0f, 0.0f, 0.0f, 0.0f);
    ofFloatColor edgeColor = ofFloatColor(0.0f, 0.0f, 0.0f, 0.32f);

    const glm::vec2 center(width * 0.5f, height * 0.5f);
    vignette.addVertex(glm::vec3(center.x, center.y, 0.0f));
    vignette.addColor(vignetteCenterColor);

    const int fanResolution = 60;
    const float radius = glm::length(glm::vec2(width, height));
    for (int i = 0; i <= fanResolution; ++i) {
        float angle = ofMap(i, 0, fanResolution, 0.0f, TWO_PI);
        glm::vec2 pos = center + glm::vec2(cosf(angle), sinf(angle)) * radius;
        vignette.addVertex(glm::vec3(pos.x, pos.y, 0.0f));
        vignette.addColor(edgeColor);
    }
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    vignette.draw();
    ofDisableBlendMode();
}

void ofApp::drawBackgroundDust() {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (const auto &dot : backgroundDust) {
        float flicker = ofNoise(dot.noiseOffset + time * 0.6f);
        glm::vec2 offset(
            ofSignedNoise(dot.noiseOffset * 0.73f, time * 0.32f) * 16.0f,
            ofSignedNoise(dot.noiseOffset * 1.12f, time * 0.29f) * 14.0f);
        glm::vec2 pos = dot.position + offset;
        float radius = dot.size * ofLerp(0.8f, 1.9f, flicker);
        ofFloatColor color = dot.color;
        color.a = dot.alpha * ofLerp(0.5f, 1.35f, flicker);
        ofSetColor(color);
        ofDrawCircle(pos, radius);
    }
    ofDisableBlendMode();
    ofPopStyle();
}

void ofApp::drawRibbons() {
    const float width = static_cast<float>(ofGetWidth());

    for (size_t idx = 0; idx < ribbons.size(); ++idx) {
        const auto &ribbon = ribbons[idx];

        ofPolyline rawPath;
        const float spacing = 16.0f;
        for (float x = -80.0f; x <= width + 80.0f; x += spacing) {
            float noiseSample = ribbon.noiseScale * x + time * ribbon.noiseSpeed + ribbon.noiseOffset;
            float noiseValue = ofNoise(noiseSample);
            float signedNoise = ofSignedNoise(noiseSample * 0.5f, time * 0.18f);
            float wave = ribbon.waveAmplitude * (noiseValue - 0.5f) * 2.0f;
            wave += signedNoise * ribbon.waveAmplitude * 0.35f;
            wave += sinf(time * 0.32f + x * 0.012f + idx) * 18.0f;
            rawPath.addVertex(x, ribbon.baseY + wave, 0.0f);
        }

        ofPolyline smoothed = rawPath.getResampledBySpacing(6.0f).getSmoothed(8);
        const auto &points = smoothed.getVertices();
        if (points.size() < 2) {
            continue;
        }

        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        ofPolyline highlightPath;

        for (size_t i = 0; i < points.size(); ++i) {
            glm::vec2 current = points[i];
            size_t prevIndex = (i == 0) ? 0 : i - 1;
            size_t nextIndex = (i == points.size() - 1) ? points.size() - 1 : i + 1;
            glm::vec2 prev = points[prevIndex];
            glm::vec2 next = points[nextIndex];
            glm::vec2 tangent = next - prev;
            if (glm::length(tangent) > 0.0001f) {
                tangent = glm::normalize(tangent);
            } else {
                tangent = {1.0f, 0.0f};
            }
            glm::vec2 normal(-tangent.y, tangent.x);

            float thicknessNoise = ofNoise(ribbon.noiseOffset + i * 0.15f + time * 0.25f);
            float thickness = ribbon.thickness * ofLerp(0.55f, 1.18f, thicknessNoise);
            float taper = ofMap(i, 0, points.size() - 1, 0.75f, 1.0f, true);
            thickness *= taper;

            glm::vec2 upper = current + normal * (thickness * 0.5f);
            glm::vec2 lower = current - normal * (thickness * 0.5f);

            ofFloatColor upperColor = ribbon.color;
            ofFloatColor lowerColor = ribbon.color;
            float fade = ofMap(i, 0, points.size() - 1, 0.82f, 0.45f, true);
            upperColor.a *= fade;
            lowerColor.a *= fade * 0.7f;

            float noiseSample = ribbon.noiseScale * current.x + time * ribbon.noiseSpeed + ribbon.noiseOffset;
            float signedNoiseValue = ofSignedNoise(noiseSample * 0.5f, time * 0.18f);
            upperColor = upperColor.getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, upperColor.a), 0.15f * signedNoiseValue);

            mesh.addVertex(glm::vec3(upper.x, upper.y, 0.0f));
            mesh.addColor(upperColor);
            mesh.addVertex(glm::vec3(lower.x, lower.y, 0.0f));
            mesh.addColor(lowerColor);

            glm::vec2 highlightPoint = current + normal * (thickness * 0.18f);
            highlightPath.addVertex(highlightPoint.x, highlightPoint.y, 0.0f);
        }

        mesh.draw();

        ofPushStyle();
        ofSetLineWidth(2.4f);
        ofFloatColor outlineColor = ribbon.color.getLerped(palette[(idx + 2) % palette.size()], 0.45f);
        outlineColor = outlineColor.getLerped(ofFloatColor(1.0f, 1.0f, 1.0f, outlineColor.a), 0.2f);
        outlineColor.a *= 0.48f;
        ofSetColor(outlineColor);
        highlightPath = highlightPath.getSmoothed(12);
        highlightPath.draw();
        ofPopStyle();
    }
}

void ofApp::drawHalos() {
    const float timeFactor = time * 0.35f;
    ofPushStyle();
    for (const auto &halo : halos) {
        float wobbleX = ofSignedNoise(halo.noiseOffset + timeFactor) * 38.0f;
        float wobbleY = ofSignedNoise(halo.noiseOffset + 93.0f + timeFactor * 1.1f) * 32.0f;
        float pulsate = ofNoise(halo.noiseOffset * 1.7f + time * halo.noiseSpeed);
        float radius = halo.radius * ofLerp(0.55f, 1.45f, pulsate);

        ofFloatColor innerColor = halo.color;
        innerColor.a *= ofLerp(0.65f, 1.4f, pulsate);

        ofFloatColor outerColor = halo.color;
        outerColor.a *= 0.12f;

        ofMesh blob;
        blob.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        glm::vec2 center = halo.anchor + glm::vec2(wobbleX, wobbleY);
        blob.addVertex(glm::vec3(center.x, center.y, 0.0f));
        blob.addColor(innerColor);

        const int resolution = 32;
        for (int i = 0; i <= resolution; ++i) {
            float angle = TWO_PI * static_cast<float>(i) / static_cast<float>(resolution);
            glm::vec2 pos = center + glm::vec2(cosf(angle), sinf(angle)) * radius;
            blob.addVertex(glm::vec3(pos.x, pos.y, 0.0f));
            blob.addColor(outerColor);
        }

        blob.draw();

        ofFloatColor coreColor = innerColor;
        coreColor.a *= 0.8f;
        ofSetColor(coreColor);
        ofDrawCircle(center, radius * 0.28f);
    }
    ofPopStyle();
}

void ofApp::drawSparkles() {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetLineWidth(1.2f);
    for (const auto &sparkle : sparkles) {
        float twinkle = ofNoise(sparkle.noiseOffset + time * sparkle.noiseSpeed);
        glm::vec2 jitter(
            ofSignedNoise(sparkle.noiseOffset + time * 0.42f) * 9.0f,
            ofSignedNoise(sparkle.noiseOffset + 37.0f + time * 0.36f) * 7.5f);
        glm::vec2 pos = sparkle.position + jitter;
        float radius = sparkle.baseRadius * ofLerp(0.5f, 1.9f, twinkle);

        ofFloatColor inner = sparkle.color;
        inner.a *= ofLerp(0.45f, 1.45f, twinkle);
        ofFloatColor outer = inner;
        outer.a *= 0.25f;

        ofSetColor(outer);
        ofDrawCircle(pos, radius * 1.6f);

        ofSetColor(inner);
        ofDrawCircle(pos, radius * 0.8f);

        float cross = radius * 1.7f;
        ofDrawLine(pos.x - cross, pos.y, pos.x + cross, pos.y);
        ofDrawLine(pos.x, pos.y - cross, pos.x, pos.y + cross);
    }
    ofDisableBlendMode();
    ofPopStyle();
}

void ofApp::keyPressed(int key) {
    if (key == 's' || key == 'S') {
        const auto timestamp = ofGetTimestampString("%Y%m%d-%H%M%S");
        ofSaveScreen("captures/chromatic_drift-" + timestamp + ".png");
    }
}
